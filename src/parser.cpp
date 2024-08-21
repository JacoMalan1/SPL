#include <parser.h>

Parser::Parser(TokenStream tokens) : m_Tokens(tokens) {}

Parser::Parser(const Parser &other) : m_Tokens(other.m_Tokens) {}

Parser &Parser::operator=(const Parser &other) {
  this->m_Tokens = other.m_Tokens;
  return *this;
}
