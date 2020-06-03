# flex lexer/lexer.l #IF ON WINDOWS, USE WIN_FLEX
# g++ -g -std=c++17 parser/parser.h parser/parser.cpp *.h *.cpp 
# rm *.gch
# rm parser/parser.h.gch

echo "Checking if platform is Linux, if on Linux, use flex, if not, use win_flex, to generate the lexer"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  echo "Platform is Linux"
  flex lexer/lexer.l 
else
  echo "Platform is Windows"
  win_flex lexer/lexer.l 
fi

g++ -g -std=c++17 parser/parser.h parser/parser.cpp *.h *.cpp -o l_assembler
rm *.gch
rm parser/parser.h.gch