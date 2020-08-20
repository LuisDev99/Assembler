#ifndef TOKENS_____
#define TOKENS_____

#include <unordered_map>

enum Token : unsigned char
{
    Instruction_Add = 1,
    Instruction_Sub,
    Instruction_Mul,
    Instruction_Slt,
    Instruction_And,
    Instruction_Xor,
    Instruction_Srl,
    Instruction_Sll,
    Instruction_Li,
    Instruction_Beq,
    Instruction_Jmp,
    Register,
    Int_Literal,
    Hex_Literal,
    Bin_Literal,
    Char_Literal,
    Label,
    Label_Name,
    Comma,
    Colon,
    ERROR,
};

const std::unordered_map<Token, const char *> TokenToStringMap = {
    {Instruction_Add, "Add"},
    {Instruction_Sub, "Sub"},
    {Instruction_Mul, "Mul"},
    {Instruction_Slt, "Slt"},
    {Instruction_And, "And"},
    {Instruction_Xor, "Xor"},
    {Instruction_Srl, "Srl"},
    {Instruction_Sll, "Sll"},
    {Instruction_Li, "Li"},
    {Instruction_Beq, "Beq"},
    {Instruction_Jmp, "Jmp"},
    {Register, "Register"},
    {Int_Literal, "Int Literal"},
    {Hex_Literal, "Hex_Literal"},
    {Bin_Literal, "Bin_Literal"},
    {Label, "Label"},
    {Label_Name, "Label_Name"},
    {Comma, "Comma"},
    {Colon, "Colon"},

    {ERROR, "PANIC MODE! Lexer"},
};

const std::unordered_map<std::string, std::string> instructionToBinaryStrMapy = {
    {"add", "000"},
    {"sub", "001"},
    {"mul", "010"},
    {"slt", "011"},
    {"and", "100"},
    {"xor", "101"},
    {"srl", "110"},
    {"sll", "111"},
};

static const char *tokenToStringy(Token tk)
{
    auto tuple = TokenToStringMap.find(tk);

    if (tuple == TokenToStringMap.end())
        return "Error, Token either does not exists or its string has not been added in the TokenToString map";

    return tuple->second;
}

#endif //TOKENS