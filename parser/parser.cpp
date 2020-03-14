#include "parser.h"

#define END_OF_INSTRUCTIONS "Epsilon"
#define IMMEDIATE_BITS "00000000"
#define GET_NEXT_TOKEN() \
    lookAhead = getNextToken();

vector<Instruction> Parser::Program()
{
    return Instructions();
}

vector<Instruction> Parser::Instructions()
{
    vector<Instruction> instructions;
    Instruction singleInstruction;

    singleInstruction = InstructionC();

    instructions.push_back(singleInstruction);

    singleInstruction = InstructionPrime();

    while (singleInstruction.binaryCode != END_OF_INSTRUCTIONS)
    {
        instructions.push_back(singleInstruction);
        singleInstruction = InstructionPrime();
    }

    return instructions;
}

Instruction Parser::InstructionPrime()
{
    if (!(isTokenRFormat(this->lookAhead) || isTokenBranchFormat(this->lookAhead) || isTokenIFormat(this->lookAhead) || this->lookAhead == Token::Label_Name))
    {
        Instruction epsilonInst = Instruction();
        epsilonInst.binaryCode = END_OF_INSTRUCTIONS;

        return epsilonInst;
    }

    return InstructionC();
}

Instruction Parser::InstructionC()
{
    if (isTokenRFormat(this->lookAhead))
    {
        return R_Format();
    }
    else if (isTokenBranchFormat(this->lookAhead))
    {
        return Branch_Format();
    }
    else if (isTokenIFormat(this->lookAhead))
    {
        return I_Format();
    }
    else
    {
        if (this->lookAhead != Token::Label_Name)
            throw IllegalInstructionException(this->lexer->lineno());

        string labelName = this->lexer->YYText();
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Colon)
        {
            throw UnexpectedTokenException(Token::Colon, this->lookAhead, this->lexer->lineno());
        }
        GET_NEXT_TOKEN();

        Instruction labelInstruction;
        labelInstruction.isLabel = true;
        labelInstruction.labelName = labelName;

        return labelInstruction;
    }
}

Instruction Parser::R_Format()
{
    string operationStr = Operation();

    if (this->lookAhead != Token::Register)
    {
        cout << "Expected a destination Register in " + operationStr + " Instruction. Details: ";
        throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
    }

    string destinationRegister = this->lexer->YYText();

    GET_NEXT_TOKEN();

    if (this->lookAhead != Token::Comma)
    {
        cout << "Missing Comma in " + operationStr + " Instruction. Details: ";
        throw UnexpectedTokenException(Token::Comma, this->lookAhead, this->lexer->lineno());
    }

    GET_NEXT_TOKEN();

    if (this->lookAhead != Token::Register)
    {
        cout << "Expected a first operand Register in " + operationStr + " Instruction. Details: ";
        throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
    }

    string firstOperandRegister = this->lexer->YYText();

    GET_NEXT_TOKEN();

    if (this->lookAhead != Token::Comma)
    {
        cout << "Missing Comma in " + operationStr + " Instruction. Details: ";
        throw UnexpectedTokenException(Token::Comma, this->lookAhead, this->lexer->lineno());
    }

    GET_NEXT_TOKEN();

    if (this->lookAhead != Token::Register)
    {
        cout << "Expected a second operand Register in " + operationStr + " Instruction. Details: ";
        throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
    }

    string secondOperandRegister = this->lexer->YYText();
    GET_NEXT_TOKEN();

    string encoded_aluopcode = encodeALUOperation(operationStr);
    string encoded_destinationRegister = encodeRegisterAddress(destinationRegister);
    string encoded_firstOperandRegister = encodeRegisterAddress(firstOperandRegister);
    string encoded_secondOperandRegister = encodeRegisterAddress(secondOperandRegister);

    Instruction rFormat;
    rFormat.binaryCode = IMMEDIATE_BITS + encoded_aluopcode + "1" + encoded_firstOperandRegister + encoded_secondOperandRegister + encoded_destinationRegister + "000";

    return rFormat;
}

Instruction Parser::Branch_Format()
{
    if (this->lookAhead == Token::Instruction_Beq)
    {
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Register)
        {
            cout << "Expected a first operand Register in BEQ Instruction. Details: ";
            throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
        }

        string firstOperandRegister = this->lexer->YYText();
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Comma)
        {
            cout << "Missing Comma in BEQ Instruction";
            throw UnexpectedTokenException(Token::Comma, this->lookAhead, this->lexer->lineno());
        }

        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Register)
        {
            cout << "Expected a second operand Register in BEQ Instruction. Details: ";
            throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
        }

        string secondOperandRegister = this->lexer->YYText();
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Comma)
        {
            cout << "Missing Comma in BEQ Instruction";
            throw UnexpectedTokenException(Token::Comma, this->lookAhead, this->lexer->lineno());
        }

        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Label_Name)
        {
            cout << "Expected a valid label address in BEQ instruction. Correct Syntax is: beq $r0, $r0, SomeWhere. Details: ";
            throw UnexpectedTokenException(Token::Label, this->lookAhead, this->lexer->lineno());
        }

        string labelName = this->lexer->YYText();
        GET_NEXT_TOKEN();

        string encoded_firstOperandRegister = encodeRegisterAddress(firstOperandRegister);
        string encoded_secondOperandRegister = encodeRegisterAddress(secondOperandRegister);

        Instruction beqInstruction;

        beqInstruction.needsLabelAddress = true;
        beqInstruction.labelName = labelName;
        beqInstruction.lineNumber = this->lexer->lineno();
        beqInstruction.binaryCode = "22222222" + string("001") + "0" + encoded_firstOperandRegister + encoded_secondOperandRegister + "000" + "1" + "0" + "0";
        return beqInstruction;
    }

    if (this->lookAhead == Token::Instruction_Jmp)
    {
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Label_Name)
        {
            cout << "Expected a valid label address in jump instruction. Correct Syntax is: jmp L0. Details: ";
            throw UnexpectedTokenException(Token::Label, this->lookAhead, this->lexer->lineno());
        }

        string labelName = this->lexer->YYText();

        GET_NEXT_TOKEN();

        Instruction jmpInstruction;

        jmpInstruction.needsLabelAddress = true;
        jmpInstruction.labelName = labelName;
        jmpInstruction.lineNumber = this->lexer->lineno();
        jmpInstruction.binaryCode = "22222222" + string("000") + "0" + "000" + "000" + "000" + "0" + "1" + "0";

        return jmpInstruction;
    }
}

Instruction Parser::I_Format()
{
    if (this->lookAhead == Token::Instruction_Li)
    {
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Register)
        {
            cout << "Expected a destination register in LI Instruction. Details: ";
            throw UnexpectedTokenException(Token::Register, this->lookAhead, this->lexer->lineno());
        }

        string destinationRegister = this->lexer->YYText();
        GET_NEXT_TOKEN();

        if (this->lookAhead != Token::Comma)
        {
            cout << "Missing Comma in BEQ Instruction";
            throw UnexpectedTokenException(Token::Comma, this->lookAhead, this->lexer->lineno());
        }

        GET_NEXT_TOKEN();

        string immediateValue = Constant();
        string encoded_destinationRegister = encodeRegisterAddress(destinationRegister);

        Instruction loadImmediateInstruction = Instruction();
        loadImmediateInstruction.binaryCode = immediateValue + "000" + "1" + "000" + "000" + encoded_destinationRegister + "001";

        return loadImmediateInstruction;
    }

    throw UnexpectedPointReachedException(__func__, __FILE__, __LINE__);
}

string Parser::Operation()
{
    if (this->lookAhead == Token::Instruction_Add)
    {
        GET_NEXT_TOKEN();
        return "add";
    }

    if (this->lookAhead == Token::Instruction_Sub)
    {
        GET_NEXT_TOKEN();
        return "sub";
    }

    if (this->lookAhead == Token::Instruction_Mul)
    {
        GET_NEXT_TOKEN();
        return "mul";
    }

    if (this->lookAhead == Token::Instruction_Slt)
    {
        GET_NEXT_TOKEN();
        return "slt";
    }

    if (this->lookAhead == Token::Instruction_And)
    {
        GET_NEXT_TOKEN();
        return "and";
    }

    if (this->lookAhead == Token::Instruction_Xor)
    {
        GET_NEXT_TOKEN();
        return "xor";
    }

    if (this->lookAhead == Token::Instruction_Srl)
    {
        GET_NEXT_TOKEN();
        return "srl";
    }

    if (this->lookAhead == Token::Instruction_Sll)
    {
        GET_NEXT_TOKEN();
        return "sll";
    }

    throw UnexpectedPointReachedException(__func__, __FILE__, __LINE__);
}

string Parser::Constant()
{
    if (this->lookAhead == Token::Int_Literal)
    {
        string binaryInt = int2bin(this->lexer->YYText());
        GET_NEXT_TOKEN();

        return binaryInt;
    }

    if (this->lookAhead == Token::Hex_Literal)
    {
        string binaryHex = hex2bin(this->lexer->YYText());
        GET_NEXT_TOKEN();

        return binaryHex;
    }

    if (this->lookAhead == Token::Bin_Literal)
    {
        string binary = this->lexer->YYText();
        GET_NEXT_TOKEN();

        return binary.substr(2, binary.length());
    }

    if (this->lookAhead == Token::Char_Literal)
    {
        string charLiteral = this->lexer->YYText();
        GET_NEXT_TOKEN();

        char character = charLiteral[1];
        string binaryChar = int2bin(to_string((int)character));

        return binaryChar;
    }

    cout << "Expected a valid constant value on line: " << this->lexer->lineno() << ". Valid Constants are: hex: 0x00, binary: 0x0000000. Write better code";
    throw std::exception();
}