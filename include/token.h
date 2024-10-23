#ifndef SPL_TOKEN_H
#define SPL_TOKEN_H

#include <ostream>
#include <string>

struct TokenException : public std::exception {
private:
  std::string message;

public:
  explicit TokenException(const std::string &msg);
  const char *what() const noexcept override;
};

enum TokenType {
  Variable,
  FunctionName,
  StringLiteral,
  NumLiteral,
  Keyword,
  Punctuation
};
enum Keyword {
  Main,
  Num,
  Text,
  Begin,
  End,
  Skip,
  Halt,
  Print,
  Input,
  If,
  Then,
  Else,
  Not,
  Sqrt,
  Or,
  And,
  Eq,
  Grt,
  Add,
  Sub,
  Mul,
  Div,
  Void,
  Return
};

std::string keyword_to_string(enum Keyword keyword);

class Token {
private:
  TokenType m_Type;
  std::string m_Identifier;
  std::string m_StringLiteral;
  std::string m_NumLiteral;
  enum Keyword m_Keyword;
  std::string m_Punct;

public:
  TokenType type() const;
  std::string to_string() const;
  std::string to_xml() const;

  const std::string get_str_data() const;

  static Token identifier(const std::string &ident);
  static Token function_name(const std::string &ident);
  static Token string_lit(const std::string &literal);
  static Token num_lit(const std::string &literal);
  static Token keyword(enum Keyword keyword);
  static Token punct(char punct);
};

std::ostream &operator<<(std::ostream &stream, const Token &token);

#endif
