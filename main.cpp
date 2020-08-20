#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./parser/parser.h"

using namespace std;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        std::cout << "By LuisDev99" << endl;
        std::cout << "Usage: <asm filename> " << std::endl;
        std::cout << "Arithmetic Instructions: add(0), sub(1), mul(2), slt(3), and(4), xor(5), srl(6), sll(7) \n";
        std::cout << "Branching Instructions: jmp, beq \n";
        std::cout << " ------ Syntax example: ------\n";
        std::cout << " Arithmetic: \t add $r0, $r6, $r7 \n";
        std::cout << " Jump Instr: \t jmp AlLab \n";
        std::cout << " Branch if eq: \t beq $r0, $r7, DondeAlla \n";
        return 1;
    }

    std::fstream in(argv[1]);

    if (!in.is_open())
    {
        std::cout << "Could not open input file! " << std::endl;
        return 1;
    }

    Parser parser(in);

    try
    {
        vector<Instruction> instructions = parser.beginParsing();
        parser.dumpBinary(argv[1], instructions);
    }
    catch (UnexpectedPointReachedException e)
    {
        std::cout << e.getErrorMessage() << std::endl;
        return 1;
    }
    catch (IllegalInstructionException e)
    {
        std::cout << e.getErrorMessage() << std::endl;
        return 1;
    }
    catch (UnexpectedTokenException e)
    {
        std::cout << e.getErrorMessage() << std::endl;
        return 1;
    }
    catch (std::exception e)
    {
        std::cout << "Translation canceled" << std::endl;
        return 1;
    }

    //parser.beginParsing();

    return 0;
}