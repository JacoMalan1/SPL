#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <lexer.h>
#include <unordered_map>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <iostream>
#include <atomic>

struct ParserException : public std::exception
{
private:
  std::string msg;

public:
  explicit ParserException(const std::string &msg);
  const char *what() const noexcept override;
};

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
  std::string tokenValue;
  std::vector<SyntaxTreeNode *> children;

  SyntaxTreeNode(const std::string &sym, const std::string &val) : id(syntaxTreeNodeCounter++), symbol(sym), tokenValue(val) {}
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
    std::cout << this->getSymbolOrValue() << std::endl;
    for (const auto &child : this->children)
    {
      child->printTree(depth + 1);
    }
  }

  std::vector<SyntaxTreeNode *> getChildren() const
  {
    return this->children;
  }

  std::string getSymbol() const
  {
    return this->symbol;
  }

  std::string getActualValue() const
  {
    return this->tokenValue;
  }

  std::string getSymbolOrValue() const
  {
    if (this->symbol == "varname" || this->symbol == "numliteral" || this->symbol == "textliteral" || this->symbol == "fname")
    {
      return this->tokenValue;
    }
    else
    {
      return this->symbol;
    }
  }
};

class Parser
{
private:
  bool delayReduce = false;
  std::vector<Token> m_Tokens;
  std::map<std::string, std::map<std::string, std::string>> parseTable;
  std::stack<StackItem> stateStack;
  std::stack<SyntaxTreeNode *> syntaxTreeStack;
  std::vector<std::pair<std::string, std::vector<std::string>>> grammarRules;

  void loadParseTable(const std::string &filename);
  void loadGrammarRules(const std::string &filename);
  std::string getAction(int state, const std::string &token);
  void shift(int state, std::string currentTokenSymbol, std::string currentTokenValue);
  void reduce(std::pair<std::string, std::vector<std::string>> rule);
  void printStateStack(std::string action);

public:
  Parser(TokenStream tokens);
  Parser(const Parser &other);
  Parser &operator=(const Parser &other);
  SyntaxTreeNode *parse();
};

#endif // SPL_PARSER_H
