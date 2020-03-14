#ifndef __PARSY_a_
#define __PARSY_a_

#define ADDRESS_SIZE 8

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <fstream>
#include <FlexLexer.h>
#include "../tokens.h"
#include "../exceptions.cpp"

using namespace std;
using LabelAddressMap = unordered_map<string, int>;

typedef struct Instructio
{
    string binaryCode;
    string labelName;
    bool isLabel;
    bool needsLabelAddress;
    int lineNumber;

    Instructio()
    {
        binaryCode = "";
        labelName = "";
        lineNumber = 1;
        isLabel = false;
        needsLabelAddress = false;
    }

} Instruction;

class Parser
{

public:
    vector<Instruction> Program();
    vector<Instruction> Instructions();
    Instruction InstructionPrime();
    Instruction InstructionC();
    Instruction R_Format();
    Instruction Branch_Format();
    Instruction I_Format();
    string Operation();
    string Constant();

    Parser(std::istream &input) : in(input)
    {
        lexer = new yyFlexLexer(&in);

        lookAhead = getNextToken();
    }

    Token getNextToken()
    {
        return (Token)lexer->yylex();
    }

    void dumpBinary(string fileName, const vector<Instruction> &instructions)
    {
        fileName = fileName.substr(0, fileName.find('.')); //Get rid of the extension of file
        fileName += ".hex";

        ofstream MyFile(fileName);

        MyFile << "v2.0 raw\n";

        for (Instruction intr : instructions)
        {
            int integer = stoi(intr.binaryCode, 0, 2); //bin2int
            std::stringstream stream;
            stream << std::hex << integer; //int2hex
            std::string hexValue(stream.str());

            MyFile << hexValue.c_str() << '\n';
        }

        MyFile.close();
        cout << "\"Binary\" code dumped to file: " << fileName << endl;
    }

    vector<Instruction> ConvertLabels_ToAddress(const vector<Instruction> &instructions)
    {
        LabelAddressMap labelsMap;
        int addressCount = 0;

        /* 
            Search through the instructions to see if its just a label, if it is,  
            get the address of that label 
        */
        for (Instruction instruction : instructions)
        {
            if (instruction.isLabel)
            {
                /* Check if the label is repeated */
                if (labelsMap.find(instruction.labelName) != labelsMap.end())
                {
                    cout << " Error! Label: \"" << instruction.labelName << "\" is declared multiple times. It means that this label's name is repeated. Dont ok :D? Give this label another name please" << endl;
                    throw std::exception();
                }

                pair<string, int> newLabelAddress(instruction.labelName, addressCount);
                labelsMap.insert(newLabelAddress);
            }
            else
            {
                addressCount++;
            }
        }

        vector<Instruction> newInstructions;

        /*
            Now search through the instructions and check if they need the label's address
        */

        for (Instruction instruction : instructions)
        {
            if (instruction.isLabel)
                continue;

            if (instruction.needsLabelAddress)
            {
                if (labelsMap.find(instruction.labelName) == labelsMap.end())
                {
                    cout << "Error: Label \"" << instruction.labelName << "\" was not found. You are using this label but forgot declare it (or mistyped it?)" << endl;
                    cout << "Error ocurred near line: " << instruction.lineNumber << endl;
                    throw std::exception();
                }

                int labelAddress = labelsMap[instruction.labelName];
                string address = encodeAddress(labelAddress);

                for (int i = 0; i < ADDRESS_SIZE; i++)
                {
                    instruction.binaryCode[i] = address[i];
                }
            }
            newInstructions.push_back(instruction);
        }

        return newInstructions;
    }

    vector<Instruction> beginParsing()
    {

        vector<Instruction> instructions = Program();

        instructions = ConvertLabels_ToAddress(instructions);

        cout << "Printing out the instructions: " << endl;
        cout << "   \tImmediate\t\tAluOp\t\twEnable\t\trAddr1\t\trAddr2\t\twAddr\t\tBEQ\t\tisJump\t\ti-Sel" << endl;

        int count = 1;
        for (Instruction intr : instructions)
        {
            cout << "I" << count << "\t";
            for (int i = 0; i < 26; i++)
            {

                switch (i)
                {
                case 8:
                case 11:
                case 12:
                case 15:
                case 18:
                case 21:
                case 22:
                case 23:
                    cout << "\t\t";
                    break;
                }

                cout << intr.binaryCode[i];
            }

            count++;
            cout << endl;
        }

        cout << endl;

        return instructions;
    }

    ~Parser()
    {
        delete lexer;
    }

private:
    string encodeAddress(string address)
    {
        address = address.substr(1, address.length()); //Remove the 'L' character

        int integer = stoi(address);

        if (integer <= 0)
        {
            cout << "Error: Address starts at address 1. (i.e, L0 is not valid, it needs to start at L1)"
                 << ", at line: " << this->lexer->lineno() << endl;
            throw std::exception();
        }

        if (integer > 255)
        {
            cout << "Error: Branch address too far away (2^8=256, from 0-255) Your current address: " << integer << ", at line: " << this->lexer->lineno() << endl;
            throw std::exception();
        }

        integer--;
        address = to_string(integer);

        return int2bin(address);
    }

    string encodeAddress(int address)
    {
        int integer = address;

        if (integer > 255)
        {
            cout << "Error: Branch address too far away (2^8=256, from 0-255) Your current address: " << integer << ", at line: " << this->lexer->lineno() << endl;
            throw std::exception();
        }

        string addressStr = to_string(integer);

        return int2bin(addressStr);
    }

    string int2bin(string number)
    {
        int integer = stoi(number);

        int addressSize = ADDRESS_SIZE; //Bits

        string value = "00000000";

        for (int i = 0; i < addressSize; i++)
        {
            value[addressSize - i - 1] = (integer >> i) & 1 ? '1' : '0';
        }

        return value;
    }

    string hex2bin(string hexNumber)
    {
        std::transform(hexNumber.begin(), hexNumber.end(), hexNumber.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        int number = stoi(hexNumber.substr(2, hexNumber.length()), 0, 16);

        return int2bin(to_string(number));
    }

    string encodeRegisterAddress(string regName)
    {
        char number = regName[2];

        if (!(number >= 48 && number <= 55))
        {
            cout << "Only registers from $r0 - $r7 are supported. Found: " << regName << " on line: " << this->lexer->lineno() << endl;
            throw std::exception();
        }

        //Worst int2bin

        switch (number)
        {
        case '0':
            return "000";
        case '1':
            return "001";
        case '2':
            return "010";
        case '3':
            return "011";
        case '4':
            return "100";
        case '5':
            return "101";
        case '6':
            return "110";
        case '7':
            return "111";
        }

        throw UnexpectedPointReachedException(__func__, __FILE__, __LINE__);
    }

    string encodeALUOperation(string operation)
    {
        std::transform(operation.begin(), operation.end(), operation.begin(),
                       [](unsigned char c) { return std::tolower(c); });

        if (operation == "add")
        {
            return "000";
        }

        if (operation == "sub")
        {
            return "001";
        }

        if (operation == "mul")
        {
            return "010";
        }

        if (operation == "slt")
        {
            return "011";
        }

        if (operation == "and")
        {
            return "100";
        }

        if (operation == "xor")
        {
            return "101";
        }

        if (operation == "srl")
        {
            return "110";
        }

        if (operation == "sll")
        {
            return "111";
        }

        cout << "Invalid Instruction found" << endl;
        throw UnexpectedPointReachedException(__func__, __FILE__, __LINE__);
    }

    bool isTokenRFormat(Token tk)
    {
        return isTokenOperation(tk);
    }

    bool isTokenBranchFormat(Token tk)
    {
        return tk == Token::Instruction_Beq || tk == Token::Instruction_Jmp;
    }

    bool isTokenIFormat(Token tk)
    {
        return tk == Instruction_Li;
    }

    bool isTokenOperation(Token tk)
    {
        switch (tk)
        {
        case Token::Instruction_Add:
        case Token::Instruction_Sub:
        case Token::Instruction_Mul:
        case Token::Instruction_Slt:
        case Token::Instruction_And:
        case Token::Instruction_Xor:
        case Token::Instruction_Srl:
        case Token::Instruction_Sll:
            return true;
        }

        return false;
    }

    bool isTokenConstant(Token tk)
    {
        switch (tk)
        {
        case Token::Int_Literal:
        case Token::Hex_Literal:
        case Token::Bin_Literal:
            return true;
        }

        return false;
    }

private:
    std::istream &in;
    yyFlexLexer *lexer;
    Token lookAhead;
};

#endif