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
  delete lexer;
}

TEST(LexerTest, VariableDecl) {
  auto *lexer = new Lexer("V_abc");
  auto token = lexer->next_token();
  EXPECT_TRUE(token.has_value());
  EXPECT_EQ(token.value().type(), TokenType::Variable);
  delete lexer;
}

TEST(LexerTest, InvalidToken) {
  auto *lexer = new Lexer("invalid");
  EXPECT_THROW(lexer->next_token(), LexerException);
  delete lexer;
}
