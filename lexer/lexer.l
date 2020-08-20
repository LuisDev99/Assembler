%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tokens.h"

%}

%option outfile="Scanner.cpp"
%option c++
%option case-insensitive
%option noyywrap
%option yylineno


COMMENT "//"[^\n]*
WHITESPACE [\t\r\f" "]

LABEL_NAME [a-zA-Z0-9_]+

REGISTER ($r[0-9])
INT_LITERAL {DIGIT}+
HEX_LITERAL (0[xX][0-9a-fA-F][0-9a-fA-F])
BIN_LITERAL (0[bB][0-1]+)
CHAR_LITERAL ('([a-zA-Z]|[ -@])')

LABEL ([lL][0-9]+)

LETTER [a-zA-Z]
DIGIT [0-9]


%%

"add"         { return Instruction_Add;        }
"sub"         { return Instruction_Sub;        }
"mul"         { return Instruction_Mul;        }
"slt"         { return Instruction_Slt;        }
"and"         { return Instruction_And;        }
"xor"         { return Instruction_Xor;        }
"srl"         { return Instruction_Srl;        }
"sll"         { return Instruction_Sll;        }
"li"          { return Instruction_Li;         }
"beq"         { return Instruction_Beq;        }
"jmp"         { return Instruction_Jmp;        }
","           { return Comma;                  }
":"           { return Colon;                  }

{REGISTER}       { return Register;           }

{INT_LITERAL}    { return Int_Literal;          }
{HEX_LITERAL}    { return Hex_Literal;          }
{BIN_LITERAL}    { return Bin_Literal;          }
{CHAR_LITERAL}   { return Char_Literal;         }

{LABEL_NAME}     { return Label_Name;           }

{WHITESPACE}    {}
{COMMENT}       {}

.|\n   {  }

%%
