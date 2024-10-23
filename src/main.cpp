#include <fstream>
#include <iostream>
#include <lexer.h>
#include <parser.h>
#include <typechecker.h>
#include <sstream>

int main(int argc, const char **argv)
{
  if (argc < 2)
  {
    std::cerr << "Usage: splc [file|-]" << std::endl
              << " `-` - Read input from stdin" << std::endl;
    return -1;
  }

  auto input = argv[1];
  std::string filename = "";
  std::string source;
  if (std::string(input) == "-")
  {
    std::stringstream stream;

    std::string line;
    while (std::getline(std::cin, line))
    {
      stream << line << std::endl;
    }
    source = stream.str();
  }
  else
  {
    filename = input;
    std::ifstream fs(input);
    if (!fs.is_open())
    {
      std::cerr << "Failed to open file!" << std::endl;
      return -1;
    }

    std::stringstream stream;
    std::string line;
    while (std::getline(fs, line))
    {
      stream << line << std::endl;
    }
    source = stream.str();
  }

  // lexical analysis
  auto *lexer = new Lexer(source);
  auto stream = lexer->lex_all();

  std::ofstream file("tokens.xml");
  if (!file.is_open())
  {
    return -1;
  }

  file << stream.to_xml() << std::endl;
  file.close();

  // syntax analysis
  auto *parser = new Parser(stream);
  SyntaxTreeNode *syntaxTreeRoot = parser->parse();

  // type checking
  auto *typeChecker = new TypeChecker(syntaxTreeRoot);
  typeChecker->setFilename(filename);
  typeChecker->check();

  delete typeChecker;
  delete parser;
  delete lexer;

  return 0;
}
