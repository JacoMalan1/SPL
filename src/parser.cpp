#include "parser.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Parser::Parser(TokenStream tokens) : m_Tokens(tokens.getTokens())
{
  loadParseTable("parse_table.csv");
  loadGrammarRules("grammar.txt");
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

std::vector<std::string> splitCSVLine(const std::string &line)
{
  std::vector<std::string> result;
  std::stringstream ss(line);
  std::string token;
  bool inQuotes = false;
  char c;

  while (ss.get(c))
  {
    if (c == '\"')
    {
      inQuotes = !inQuotes; // toggle quoted field
    }
    else if (c == ',' && !inQuotes)
    {
      result.push_back(token); // End of a field, push to result
      token.clear();
    }
    else
    {
      token += c; // add character to current token
    }
  }
  result.push_back(token); // push the last token

  return result;
}

std::vector<std::vector<std::string>> readCSV(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return {};
  }

  std::vector<std::vector<std::string>> data;
  std::string line;

  // read each line of the CSV
  while (std::getline(file, line))
  {
    // split the line into fields, handling quoted fields correctly
    std::vector<std::string> row = splitCSVLine(line);
    data.push_back(row);
  }

  file.close();
  return data;
}

void Parser::loadParseTable(const std::string &filename)
{
  std::vector<std::vector<std::string>> data = readCSV(filename);
  std::vector<std::string> headers = data[1];

  if (!data.empty())
  {
    for (size_t m = 2; m < data.size(); ++m)
    {
      std::string state = data[m][0];
      for (size_t n = 1; n < headers.size(); ++n)
      {
        std::string token = data[m][n];
        this->parseTable[state][headers[n]] = token;
      }
    }
  }
}

void Parser::loadGrammarRules(const std::string &filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file " << filename << std::endl;
    return;
  }

  std::string line;
  while (std::getline(file, line))
  {
    std::istringstream iss(line);
    std::string lhs, arrow, rhs;
    if (!(iss >> lhs >> arrow))
    {
      std::cerr << "Error: Invalid grammar rule format in line: " << line << std::endl;
      continue;
    }

    std::getline(iss, rhs);
    rhs.erase(0, rhs.find_first_not_of(" \t")); // Trim leading whitespace

    std::vector<std::string> rhsSymbols;
    std::istringstream rhsStream(rhs);
    std::string symbol;
    while (rhsStream >> symbol)
    {
      if (symbol != "''")
      {
        rhsSymbols.push_back(symbol);
      }
    }

    grammarRules.push_back({lhs, rhsSymbols});
    for (const auto &s : rhsSymbols)
    {
      std::cout << s << " ";
    }
    std::cout << std::endl;
  }

  file.close();
}

std::string Parser::getAction(int state, const std::string &token)
{
  std::string action = this->parseTable[std::to_string(state)][token];
  return action;
}

void Parser::shift(int state, std::string currentToken)
{
  this->stateStack.push({state, currentToken});
}

void Parser::reduce(std::pair<std::string, std::vector<std::string>> rule)
{
  int productionLength = rule.second.size();

  for (int i = 0; i < productionLength; ++i)
  {
    this->stateStack.pop();
  }

  int currentState = this->stateStack.top().state;
  std::string nonTerminal = rule.first;
  int gotoState = std::stoi(getAction(currentState, nonTerminal));
  this->stateStack.push({gotoState, nonTerminal});
}

void Parser::parse()
{
  this->stateStack.push({0, ""});

  try
  {
    m_Tokens.push_back(Token::string_lit("$")); // add end of input token

    while (!m_Tokens.empty())
    {
      int currentState = stateStack.top().state;
      TokenType currentTokenType = m_Tokens.front().type();
      std::string currentToken = "";
      if (currentTokenType == Variable)
      {
        currentToken = "varname";
      }
      else if (currentTokenType == NumLiteral)
      {
        currentToken = "numliteral";
      }
      else if (currentTokenType == StringLiteral && m_Tokens.front().get_str_data() != "$")
      {
        currentToken = "textliteral";
      }
      else if (currentTokenType == FunctionName)
      {
        currentToken = "fname";
      }
      else
      {
        currentToken = m_Tokens.front().get_str_data();
      }

      std::string action = getAction(currentState, currentToken);

      // print stack
      std::string stackString = getStackString(this->stateStack);
      std::cout << "Action: " << action << " Stack: " << stackString << std::endl;

      if (action[0] == 's')
      {
        int nextState = std::stoi(action.substr(1));
        shift(nextState, currentToken);

        m_Tokens.erase(m_Tokens.begin());
      }
      else if (action[0] == 'r')
      {
        int ruleNum = std::stoi(action.substr(1));
        auto rule = grammarRules[ruleNum];
        reduce(rule);
      }
      else if (action == "acc")
      {
        std::cout << "Input successfully parsed!" << std::endl;
        return;
      }
      else
      {
        std::cerr << "Syntax Error: Unexpected token " << currentToken << " in state " << currentState << " Action: " << action << std::endl;
        return;
      }
    }
  }
  catch (const std::exception *e)
  {
    std::cerr << "Exception caught during parsing: " << e->what() << std::endl;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Exception caught during parsing: " << e.what() << std::endl;
  }
  catch (...)
  {
    std::cerr << "Unknown exception caught during parsing" << std::endl;
  }
}

std::string getStackString(std::stack<StackItem> stack)
{
  auto tempStack = stack;
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
  return stackString;
}