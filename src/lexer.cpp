#include <lexer.h>
#include <regex>

Lexer::Lexer(const std::string &input)
{
  this->m_Source = input;
  std::size_t loc;
  while ((loc = this->m_Source.find("\n")) != std::string::npos)
  {
    this->m_Source.replace(loc, 1, " ");
  }
}

std::optional<Token> Lexer::next_token()
{
  if (this->m_Source.empty())
  {
    return {};
  }

  auto next_delim = this->m_Source.find(" ");
  std::string token_str = this->m_Source.substr(0, next_delim);

  if (next_delim != std::string::npos)
  {
    this->m_Source = this->m_Source.substr(next_delim + 1);
  }
  else
  {
    this->m_Source = std::string();
  }

  if (token_str == "main")
  {
    return Token::keyword(Keyword::Main);
  }
  else if (token_str == "num")
  {
    return Token::keyword(Keyword::Num);
  }
  else if (token_str == "text")
  {
    return Token::keyword(Keyword::Text);
  }
  else if (token_str == "begin")
  {
    return Token::keyword(Keyword::Begin);
  }
  else if (token_str == "end")
  {
    return Token::keyword(Keyword::End);
  }
  else if (token_str == "skip")
  {
    return Token::keyword(Keyword::Skip);
  }
  else if (token_str == "halt")
  {
    return Token::keyword(Keyword::Halt);
  }
  else if (token_str == "print")
  {
    return Token::keyword(Keyword::Print);
  }
  else if (token_str == "input")
  {
    return Token::keyword(Keyword::Input);
  }
  else if (token_str == "if")
  {
    return Token::keyword(Keyword::If);
  }
  else if (token_str == "then")
  {
    return Token::keyword(Keyword::Then);
  }
  else if (token_str == "else")
  {
    return Token::keyword(Keyword::Else);
  }
  else if (token_str == "not")
  {
    return Token::keyword(Keyword::Not);
  }
  else if (token_str == "sqrt")
  {
    return Token::keyword(Keyword::Sqrt);
  }
  else if (token_str == "or")
  {
    return Token::keyword(Keyword::Or);
  }
  else if (token_str == "and")
  {
    return Token::keyword(Keyword::And);
  }
  else if (token_str == "eq")
  {
    return Token::keyword(Keyword::Eq);
  }
  else if (token_str == "grt")
  {
    return Token::keyword(Keyword::Grt);
  }
  else if (token_str == "add")
  {
    return Token::keyword(Keyword::Add);
  }
  else if (token_str == "sub")
  {
    return Token::keyword(Keyword::Sub);
  }
  else if (token_str == "mul")
  {
    return Token::keyword(Keyword::Mul);
  }
  else if (token_str == "div")
  {
    return Token::keyword(Keyword::Div);
  }
  else if (token_str == "void")
  {
    return Token::keyword(Keyword::Void);
  }

  std::regex var_pattern("V_[a-z]([a-z]|[0-9])*");
  if (std::regex_match(token_str, var_pattern))
  {
    return Token::identifier(token_str.substr(2));
  }

  std::regex func_name_pattern("F_[a-z]([a-z]|[0-9])*");
  if (std::regex_match(token_str, func_name_pattern))
  {
    return Token::function_name(token_str.substr(2));
  }

  std::regex string_lit_pattern("\"[A-Z][a-z]{0,7}\"");
  if (std::regex_match(token_str, string_lit_pattern))
  {
    token_str.erase(0, 1);
    token_str.erase(token_str.length() - 1, 1);
    return Token::string_lit(token_str);
  }

  std::regex num_pattern("(0|-?[1-9]*[0-9](\\.[0-9]*[1-9])?)");

  if (std::regex_match(token_str, num_pattern))
  {
    return Token::num_lit(token_str);
  }

  throw LexerException("Invalid Token: \"" + token_str + "\"");
}

LexerException::LexerException(const std::string &msg) : msg(msg) {}

const char *LexerException::what() const noexcept { return this->msg.c_str(); }

TokenStream Lexer::lex_all()
{
  std::vector<Token> tokens;
  std::optional<Token> t;

  while ((t = this->next_token()).has_value())
  {
    tokens.push_back(t.value());
  }

  return TokenStream(tokens);
}

TokenStream::TokenStream(const std::vector<Token> &tokens) : m_Tokens(tokens) {}

auto TokenStream::begin() const { return this->m_Tokens.begin(); }
auto TokenStream::end() const { return this->m_Tokens.end(); }

std::string TokenStream::to_xml() const
{
  std::stringstream stream;
  stream << "<TOKENSTREAM>" << std::endl;

  for (const auto &t : this->m_Tokens)
  {
    auto xml = t.to_xml();
    std::size_t loc;
    while ((loc = xml.find("\n")) != std::string::npos)
    {
      stream << "  " << xml.substr(0, loc) << std::endl;
      xml.erase(0, loc + 1);
    }
    stream << "  " << xml << std::endl;
  }

  stream << "</TOKENSTREAM>";

  return stream.str();
}
