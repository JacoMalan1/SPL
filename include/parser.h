#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <lexer.h>
#include <map>
#include <stack>
#include <string>
#include <vector>

struct StackItem
{
  int state;
  std::string symbol;
};

class Parser
{
private:
  std::vector<Token> m_Tokens;
  std::map<std::string, std::map<std::string, std::string>> parseTable;
  std::vector<std::pair<std::string, std::vector<std::string>>> grammarRules;
  std::stack<StackItem> stateStack;

  void loadParseTable(const std::string &filename);
  void loadGrammarRules(const std::string &filename);
  std::string getAction(int state, const std::string &token);
  void shift(int state, std::string currentToken);
  void reduce(std::pair<std::string, std::vector<std::string>> rule);

public:
  explicit Parser(TokenStream tokens);

  Parser(const Parser &other);
  Parser &operator=(const Parser &other);

  void parse();
};

#endif // SPL_PARSER_H
