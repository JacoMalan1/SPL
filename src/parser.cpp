#include "parser.h"
#include <fstream>
#include <sstream>
#include <algorithm>

SyntaxError::SyntaxError(const std::string &msg, std::string filename, const int &line)
{
  if (filename.empty())
  {
    this->msg = "\033[31mSyntax Error\033[0m: " + msg;
  }
  else
  {
    this->msg = filename + ":" + std::to_string(line) + ": \033[31mSyntax Error\033[0m: " + msg;
  }
}

SyntaxError::SyntaxError(const std::string &msg) : msg("\033[31mType Error\033[0m: " + msg) {}

const char *SyntaxError::what() const noexcept { return this->msg.c_str(); }

std::atomic<int> SyntaxTreeNode::syntaxTreeNodeCounter{0};

Parser::Parser(TokenStream tokens) : m_Tokens(tokens.getTokens())
{
  ParserFileHandler parserFileHandler;
  loadParseTable(parserFileHandler);
  loadGrammarRules(parserFileHandler);
}

Parser::Parser(const Parser &other) : m_Tokens(other.m_Tokens), parseTable(other.parseTable), stateStack(other.stateStack) {}

Parser &Parser::operator=(const Parser &other)
{
  if (this != &other)
  {
    this->m_Tokens = other.m_Tokens;
    this->parseTable = other.parseTable;
    this->stateStack = other.stateStack;
  }
  return *this;
}

void Parser::loadParseTable(ParserFileHandler fileHandler)
{
  this->parseTable = fileHandler.getParseTable();
}

void Parser::loadGrammarRules(ParserFileHandler fileHandler)
{
  this->grammarRules = fileHandler.getGrammarRules();
}

std::string Parser::getAction(int state, const std::string &token)
{
  std::string action = this->parseTable[std::to_string(state)][token];
  return action;
}

void Parser::shift(int state, std::string currentTokenSymbol, std::string currentTokenValue, int line)
{
  this->stateStack.push({state, currentTokenSymbol});
  this->syntaxTreeStack.push(new SyntaxTreeNode(currentTokenSymbol, currentTokenValue, line));
}

void Parser::reduce(std::pair<std::string, std::vector<std::string>> rule, int line)
{
  int productionLength = rule.second.size();

  // create a new node for the LHS of the production
  SyntaxTreeNode *lhsNode = new SyntaxTreeNode(rule.first, line);

  // if (delayReduce && rule.second.size() > 0 && rule.second[0] == "COMMAND")
  // {
  //   delayReduce = false;
  //   productionLength = 1;
  // }
  // else if (rule.second.size() > 0 && ((rule.second[0] == "ASSIGN") || (rule.second[0] == "CALL") || (rule.second[0] == "BRANCH")))
  // {
  //   delayReduce = true;
  // }

  for (int i = 0; i < productionLength; ++i)
  {
    this->stateStack.pop();
    auto rhsNode = syntaxTreeStack.top(); // get the node for the RHS of the production
    this->syntaxTreeStack.pop();
    lhsNode->addChild(rhsNode); // add the RHS node as a child of the LHS node
  }

  this->syntaxTreeStack.push(lhsNode); // push the LHS node onto the stack

  int currentState = this->stateStack.top().state;
  std::string nonTerminal = rule.first;

  // get the GOTO action after reduction
  std::string gotoAction = this->getAction(currentState, nonTerminal);
  if (gotoAction.empty())
  {
    return;
  }

  int gotoState = std::stoi(gotoAction);
  this->stateStack.push({gotoState, nonTerminal});
}

SyntaxTreeNode *Parser::parse()
{
  this->stateStack.push({0, ""});

  try
  {
    this->m_Tokens.push_back(Token::string_lit("$")); // add end of input token

    while (!m_Tokens.empty())
    {
      int currentState = this->stateStack.top().state;
      TokenType currentTokenType = this->m_Tokens.front().type();
      std::string currentTokenSymbol = "";
      std::string currentTokenValue = "";
      if (currentTokenType == TokenType::Variable)
      {
        currentTokenSymbol = "varname";
        currentTokenValue = this->m_Tokens.front().get_str_data();
      }
      else if (currentTokenType == TokenType::NumLiteral)
      {
        currentTokenSymbol = "numliteral";
        currentTokenValue = this->m_Tokens.front().get_str_data();
      }
      else if (currentTokenType == TokenType::StringLiteral && this->m_Tokens.front().get_str_data() != "$")
      {
        currentTokenSymbol = "textliteral";
        currentTokenValue = this->m_Tokens.front().get_str_data();
      }
      else if (currentTokenType == TokenType::FunctionName)
      {
        currentTokenSymbol = "fname";
        currentTokenValue = this->m_Tokens.front().get_str_data();
      }
      else
      {
        currentTokenSymbol = this->m_Tokens.front().get_str_data();
      }

      std::string action = this->getAction(currentState, currentTokenSymbol);

      // print state stack
      // this->printStateStack(action);

      if (action[0] == 's')
      {
        int nextState = std::stoi(action.substr(1));
        shift(nextState, currentTokenSymbol, currentTokenValue, this->m_Tokens.front().get_line_number());

        m_Tokens.erase(m_Tokens.begin());
      }
      else if (action[0] == 'r')
      {
        int ruleNum = std::stoi(action.substr(1));
        auto rule = grammarRules[ruleNum];

        reduce(rule, this->m_Tokens.front().get_line_number());
      }
      else if (action == "acc")
      {
        std::cout << "\nInput successfully parsed, syntax tree is shown below\n"
                  << std::endl;
        auto syntaxTreeRoot = this->syntaxTreeStack.top();
        syntaxTreeRoot->printTree(); // print the final syntax tree
        return syntaxTreeRoot;
      }
      else
      {
        throw SyntaxError("Unexpected token symbol " + currentTokenSymbol + " in state " + std::to_string(currentState) + " Action: " + action);
      }
    }
  }
  catch (const std::exception *e)
  {
    std::cerr << "\n"
              << e->what() << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << "\n"
              << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown exception caught during parsing" << std::endl;
  }

  return nullptr;
}

void Parser::printStateStack(std::string action = "")
{
  auto tempStack = this->stateStack;
  std::vector<std::string> stackVector;
  while (!tempStack.empty())
  {
    std::stringstream ss;
    ss << "(" << tempStack.top().state << ", " << tempStack.top().symbol << ") ";
    stackVector.push_back(ss.str());
    tempStack.pop();
  }

  // reverse the vector
  std::reverse(stackVector.begin(), stackVector.end());

  // store vector in string
  std::string stackString = "";
  for (auto &s : stackVector)
  {
    stackString += s;
  }

  if (action != "")
  {
    std::cout << "Action: " << action << " Stack: " << stackString << std::endl;
  }
  else
  {
    std::cout << "Stack: " << stackString << std::endl;
  }
}