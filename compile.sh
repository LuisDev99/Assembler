flex lexer/lexer.l 
g++ -g -std=c++17 parser/parser.h parser/parser.cpp *.h *.cpp 
rm *.gch
rm parser/parser.h.gch