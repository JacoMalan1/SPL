#ifndef SPL_LEXER_H
#define SPL_LEXER_H

#include <exception>
#include <optional>
#include <string>
#include <token.h>
#include <vector>

struct LexerException : public std::exception {
private:
  std::string msg;

public:
  explicit LexerException(const std::string &msg);
  const char *what() const noexcept override;
};

class TokenStream {
private:
  std::vector<Token> m_Tokens;

public:
  explicit TokenStream(const std::vector<Token> &tokens);
  std::vector<Token> getTokens();
  auto begin() const;
  auto end() const;
  std::optional<Token> next();
  std::string to_xml() const;
};

class Lexer {
private:
  std::string m_Source;
  std::vector<std::string> unprocessed_input;

public:
  Lexer(const std::string &input);
  std::optional<Token> next_token();
  TokenStream lex_all();
  int getTokenLineNumber(std::string tokenValue);
};

#endif
