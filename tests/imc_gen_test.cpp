#include "imc.h"
#include <gtest/gtest.h>

TEST(IMCVariable, TestText) {
  IMCVariable var = IMCVariable::make_variable("v1", "Hello World!");

  const char *t = var.get_text();
  IMCVariable::VariableType ty = var.get_type();
  ASSERT_STREQ(t, "Hello World!");
  ASSERT_EQ(ty, IMCVariable::VariableType::Text);
  ASSERT_ANY_THROW([[maybe_unused]] int64_t _ = var.get_number());
}

TEST(IMCVariable, TestNumber) {
  IMCVariable var = IMCVariable::make_variable("v1", 42);

  int64_t n = var.get_number();
  IMCVariable::VariableType ty = var.get_type();
  ASSERT_EQ(n, 42);
  ASSERT_EQ(ty, IMCVariable::VariableType::Number);
  ASSERT_ANY_THROW([[maybe_unused]] const char *_ = var.get_text());
}

TEST(IMCStatement, TestNumber) {
  IMCStatement stat = IMCStatement::create_value(42);
  ASSERT_EQ(stat.get_type(), IMCStatement::StatementType::NumberValue);
  ASSERT_EQ(stat.get_number(), 42);
  ASSERT_ANY_THROW(stat.get_text());
  ASSERT_ANY_THROW(stat.get_lhs_id());
  ASSERT_ANY_THROW(stat.get_rhs());
}

TEST(IMCStatement, TestText) {
  IMCStatement stat = IMCStatement::create_value("Hello World!");
  ASSERT_EQ(stat.get_type(), IMCStatement::StatementType::TextValue);
  ASSERT_EQ(stat.get_text(), "Hello World!");
  ASSERT_ANY_THROW(stat.get_number());
  ASSERT_ANY_THROW(stat.get_lhs_id());
  ASSERT_ANY_THROW(stat.get_rhs());
}

TEST(IMCStatement, TestAssignment) {
  IMCStatement val = IMCStatement::create_value(42);
  IMCStatement stat = IMCStatement::create_assignment("v1", val);
  ASSERT_EQ(stat.get_type(), IMCStatement::StatementType::Assignment);
  ASSERT_EQ(stat.get_lhs_id(), "v1");
  ASSERT_EQ(stat.get_rhs(), val);
  ASSERT_NE(stat, val);
}
