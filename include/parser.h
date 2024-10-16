#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <lexer.h>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include <atomic>

struct StackItem
{
  int state;
  std::string symbol;
};

struct SyntaxTreeNode
{
  static std::atomic<int> syntaxTreeNodeCounter;
  int id;
  std::string symbol;
  std::vector<SyntaxTreeNode *> children;

  SyntaxTreeNode(const std::string &sym) : id(syntaxTreeNodeCounter++), symbol(sym) {}

  void addChild(SyntaxTreeNode *child)
  {
    this->children.insert(this->children.begin(), child);
  }

  void printTree(int depth = 0) const
  {
    for (int i = 0; i < depth; ++i)
    {
      std::cout << "  ";
    }
    // std::cout << symbol << " (ID: " << id << ")" << std::endl;
    std::cout << symbol << std::endl;
    for (const auto &child : this->children)
    {
      child->printTree(depth + 1);
    }
  }
};

class Parser
{
private:
  std::vector<Token> m_Tokens;
  std::map<std::string, std::map<std::string, std::string>> parseTable;
  std::vector<std::pair<std::string, std::vector<std::string>>> grammarRules;
  std::stack<StackItem> stateStack;
  std::stack<SyntaxTreeNode *> syntaxTreeStack;

  void loadParseTable(const std::string &filename);
  void loadGrammarRules(const std::string &filename);
  std::string getAction(int state, const std::string &token);
  void shift(int state, std::string currentToken);
  void reduce(std::pair<std::string, std::vector<std::string>> rule);
  void printStateStack(std::string action);

public:
  explicit Parser(TokenStream tokens);

  Parser(const Parser &other);
  Parser &operator=(const Parser &other);

  SyntaxTreeNode *parse();
};

#endif // SPL_PARSER_H
