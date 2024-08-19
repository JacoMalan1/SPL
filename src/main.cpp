#include <fstream>
#include <iostream>
#include <lexer.h>

int main() {
  std::string input("F_abcdef 32 V_abc main");
  auto *lexer = new Lexer(input);

  auto stream = lexer->lex_all();
  std::cout << stream.to_xml() << std::endl;

  std::ofstream file("tokens.xml");
  if (!file.is_open()) {
    return -1;
  }

  file << stream.to_xml() << std::endl;
  file.close();

  delete lexer;
  return 0;
}
