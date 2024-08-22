#ifndef SPL_PARSER_H
#define SPL_PARSER_H

#include <lexer.h>

class Parser
{
private:
  TokenStream m_Tokens;

public:
  explicit Parser(TokenStream tokens);

  Parser(const Parser &other);
  Parser &operator=(const Parser &other);
};

#endif
