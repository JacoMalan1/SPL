#include <sstream>
#include <token.h>

Token Token::identifier(const std::string &ident) {
  Token res;
  res.m_Type = TokenType::Variable;
  res.m_Identifier = ident;

  return res;
}

Token Token::function_name(const std::string &ident) {
  Token res;
  res.m_Type = TokenType::FunctionName;
  res.m_Identifier = ident;

  return res;
}

Token Token::string_lit(const std::string &literal) {
  Token res;
  res.m_Type = TokenType::StringLiteral;
  res.m_StringLiteral = literal;

  return res;
}

Token Token::num_lit(const std::string &literal) {
  Token res;
  res.m_Type = TokenType::NumLiteral;
  res.m_NumLiteral = literal;

  return res;
}

Token Token::keyword(enum Keyword keyword) {
  Token res;
  res.m_Type = TokenType::Keyword;
  res.m_Keyword = keyword;

  return res;
}

TokenType Token::type() const { return this->m_Type; }

std::string Token::to_string() const {
  std::stringstream stream;

  switch (this->m_Type) {
  case TokenType::Variable:
    stream << "Var(" << this->m_Identifier << ")";
    break;
  case TokenType::StringLiteral:
    stream << "String(" << this->m_StringLiteral << ")";
    break;
  case TokenType::NumLiteral:
    stream << "Num(" << this->m_NumLiteral << ")";
    break;
  case TokenType::FunctionName:
    stream << "Func(" << this->m_Identifier << ")";
    break;
  case TokenType::Keyword:
    stream << this->m_Keyword;
    break;
  }
  return stream.str();
}

std::ostream &operator<<(std::ostream &stream, const Token &token) {
  stream << token.to_string();
  return stream;
}

std::string Token::to_xml() const {
  static std::size_t id = 0;
  std::string token_class;
  std::string word;

  switch (this->m_Type) {
  case TokenType::FunctionName:
    token_class = "F";
    word = this->m_Identifier;
    break;
  case TokenType::NumLiteral:
    token_class = "N";
    word = this->m_NumLiteral;
    break;
  case TokenType::StringLiteral:
    token_class = "T";
    word = this->m_StringLiteral;
    break;
  case TokenType::Keyword:
    token_class = "reserved_keyword";
    word = keyword_to_string(this->m_Keyword);
    break;
  case TokenType::Variable:
    token_class = "V";
    word = this->m_Identifier;
    break;
  }

  std::stringstream stream;
  stream << "<TOK>" << std::endl;
  stream << "  <ID>" << id << "</ID>" << std::endl;
  stream << "  <CLASS>" << token_class << "</CLASS>" << std::endl;
  stream << "  <WORD>" << word << "</WORD>" << std::endl;
  stream << "</TOK>";

  id++;

  return stream.str();
}

std::string keyword_to_string(enum Keyword keyword) {
  switch (keyword) {
  case Keyword::Main:
    return "main";
    break;
  case Keyword::Num:
    return "num";
    break;
  case Keyword::Text:
    return "text";
    break;
  case Keyword::Begin:
    return "begin";
    break;
  case Keyword::End:
    return "end";
    break;
  case Keyword::Skip:
    return "skip";
    break;
  case Keyword::Halt:
    return "halt";
    break;
  case Keyword::Print:
    return "print";
    break;
  case Keyword::Input:
    return "input";
    break;
  case Keyword::If:
    return "if";
    break;
  case Keyword::Then:
    return "then";
    break;
  case Keyword::Else:
    return "else";
    break;
  case Keyword::Not:
    return "not";
    break;
  case Keyword::Sqrt:
    return "sqrt";
    break;
  case Keyword::Or:
    return "or";
    break;
  case Keyword::And:
    return "and";
    break;
  case Keyword::Eq:
    return "eq";
    break;
  case Keyword::Grt:
    return "grt";
    break;
  case Keyword::Add:
    return "add";
    break;
  case Keyword::Sub:
    return "sub";
    break;
  case Keyword::Mul:
    return "mul";
    break;
  case Keyword::Div:
    return "div";
    break;
  case Keyword::Void:
    return "void";
    break;
  }

  return std::string();
}