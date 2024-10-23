#ifndef TYPECHECKER_H
#define TYPECHECKER_H

#include "parser.h"
#include <unordered_map>
#include <string>
#include "symbol.h"

struct TypeCheckerException : public std::exception
{
private:
  std::string msg;

public:
  explicit TypeCheckerException(const std::string &msg);
  const char *what() const noexcept override;
};

class TypeChecker
{
public:
  TypeChecker(SyntaxTreeNode *root);
  void check();

private:
  SyntaxTreeNode *root;
  std::shared_ptr<SymbolTable> symbolTable;

  void checkProgram(SyntaxTreeNode *node);
  void checkGlobVars(SyntaxTreeNode *node);
  void checkAlgo(SyntaxTreeNode *node);
  void checkInstruc(SyntaxTreeNode *node);
  void checkCommand(SyntaxTreeNode *node);
  std::string checkAtomic(SyntaxTreeNode *node);
  void checkAssign(SyntaxTreeNode *node);
  std::optional<std::string> checkCall(SyntaxTreeNode *node);
  void checkFunctionArguments(const Symbol &functionSymbol, std::vector<std::string> argTypes);
  void checkBranch(SyntaxTreeNode *node);
  std::string checkTerm(SyntaxTreeNode *node);
  std::string checkOp(SyntaxTreeNode *node);
  std::string checkCond(SyntaxTreeNode *node);
  std::string checkSimple(SyntaxTreeNode *node);
  std::string checkComposit(SyntaxTreeNode *node);
  std::string checkUnop(SyntaxTreeNode *node, SyntaxTreeNode *argNode);
  std::string checkBinop(SyntaxTreeNode *node, SyntaxTreeNode *leftArgNode, SyntaxTreeNode *rightArgNode);
  std::string checkArg(SyntaxTreeNode *node);
  void checkFunctions(SyntaxTreeNode *node);
  void checkDecl(SyntaxTreeNode *node);
  void checkHeader(SyntaxTreeNode *node);
  void checkBody(SyntaxTreeNode *node);
  void checkLocVars(SyntaxTreeNode *node);
};

#endif // TYPECHECKER_H