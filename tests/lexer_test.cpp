#include <gtest/gtest.h>
#include <lexer.h>

TEST(LexerTest, EmptyTest) {
  auto *lexer = new Lexer("");
  auto token = lexer->next_token();
  EXPECT_FALSE(token.has_value());
  delete lexer;
}

TEST(LexerTest, FunctionName) {
  auto *lexer = new Lexer("F_abc");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::FunctionName);
  EXPECT_EQ(token.value().get_str_data(), "F_abc");
  delete lexer;
}

TEST(LexerTest, VariableDecl) {
  auto *lexer = new Lexer("V_abc");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Variable);
  EXPECT_EQ(token.value().get_str_data(), "V_abc");
  delete lexer;
}

TEST(LexerTest, StringLiteral) {
  auto *lexer = new Lexer("\"Aabc\"");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::StringLiteral);
  EXPECT_EQ(token.value().get_str_data(), "Aabc");
  delete lexer;
}

TEST(LexerTest, ZeroNumLit) {
  auto *lexer = new Lexer("0");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::NumLiteral);
  EXPECT_EQ(token.value().get_str_data(), "0");
  delete lexer;
}

TEST(LexerTest, FractionNumLit) {
  auto *lexer = new Lexer("0.01");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::NumLiteral);
  EXPECT_EQ(token.value().get_str_data(), "0.01");
  delete lexer;

  lexer = new Lexer("-0.01");
  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::NumLiteral);
  EXPECT_EQ(token.value().get_str_data(), "-0.01");
  delete lexer;

  lexer = new Lexer("-0.00");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("0.00");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("0.0");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("0.10");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;
}

TEST(LexerTest, RealNumLit) {
  auto *lexer = new Lexer("1.01");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::NumLiteral);
  EXPECT_EQ(token.value().get_str_data(), "1.01");
  delete lexer;

  lexer = new Lexer("-1.01");
  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::NumLiteral);
  EXPECT_EQ(token.value().get_str_data(), "-1.01");
  delete lexer;

  lexer = new Lexer("-1.00");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("1.00");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("1.0");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;

  lexer = new Lexer("1.30");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;
}

TEST(LexerTest, InvalidToken) {
  auto *lexer = new Lexer("invalid");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;
}

TEST(LexerTest, Punctuation) {
  auto *lexer = new Lexer("F_abc( V_a , V_b )");

  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::FunctionName);
  EXPECT_EQ(token.value().get_str_data(), "F_abc");

  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Punctuation);
  EXPECT_EQ(token.value().get_str_data(), "(");

  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Variable);
  EXPECT_EQ(token.value().get_str_data(), "V_a");

  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Punctuation);
  EXPECT_EQ(token.value().get_str_data(), ",");

  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Variable);
  EXPECT_EQ(token.value().get_str_data(), "V_b");

  token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Punctuation);
  EXPECT_EQ(token.value().get_str_data(), ")");

  delete lexer;
}
