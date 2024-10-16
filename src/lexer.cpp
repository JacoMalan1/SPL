#include <lexer.h>
#include <optional>
#include <regex>
#include <vector>
#include <iostream>

Lexer::Lexer(const std::string &input)
{
  // input preprocessing to help lexer with tokenisation

  std::regex punctuation_regex("([;,.()=+\\-*/<>])"); // match all punctuation marks

  // add spaces around all punctuation marks
  std::string pre_processed_input = std::regex_replace(input, punctuation_regex, " $1 ");

  std::regex space_regex("\\s+"); // match any sequence of whitespace characters (spaces, tabs, newlines)

  // replace all sequences of whitespace characters with a single space
  std::string cleaned_source = std::regex_replace(pre_processed_input, space_regex, " ");

  this->m_Source = cleaned_source;
}

std::optional<Token> Lexer::next_token()
{
  if (this->m_Source.empty())
  {
    return {};
  }

  auto next_delim = this->m_Source.find(" ");

  if (next_delim == 0)
  {
    this->m_Source.erase(0, 1);
    next_delim = this->m_Source.find(" ");
  }

  auto next_brace = this->m_Source.find("(");
  std::string token_str;

  if (this->m_Source[0] == '(')
  {
    this->m_Source.erase(0, 1);
    return Token::punct('(');
  }
  else if (next_delim <= next_brace)
  {
    token_str = this->m_Source.substr(0, next_delim);
    if (next_delim != std::string::npos)
    {
      this->m_Source = this->m_Source.substr(next_delim + 1);
    }
    else
    {
      this->m_Source = std::string();
    }
  }
  else
  {
    token_str = this->m_Source.substr(0, next_brace);
    this->m_Source = this->m_Source.substr(next_brace);
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
  else if (token_str == "<input")
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
  else if (token_str == "=")
  {
    return Token::punct('=');
  }
  else if (token_str == ";")
  {
    return Token::punct(';');
  }
  else if (token_str == ",")
  {
    return Token::punct(',');
  }
  else if (token_str == ")")
  {
    return Token::punct(')');
  }
  else if (token_str == "{")
  {
    return Token::punct('{');
  }
  else if (token_str == "}")
  {
    return Token::punct('}');
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

TokenStream::TokenStream(const std::vector<Token> &tokens)
{
  for (auto it = tokens.begin(); it != tokens.end(); it++)
  {
    this->m_Tokens.push_back(*it);
  }
}

// auto TokenStream::begin() const { return this->m_Tokens.begin(); }
// auto TokenStream::end() const { return this->m_Tokens.end(); }

std::vector<Token> TokenStream::getTokens()
{
  return this->m_Tokens;
}

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

std::optional<Token> TokenStream::next()
{
  if (this->m_Tokens.size() == 0)
  {
    return {};
  }

  Token ret = this->m_Tokens[this->m_Tokens.size() - 1];
  this->m_Tokens.pop_back();
  return ret;
}
