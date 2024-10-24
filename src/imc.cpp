#include "parser.h"
#include <imc.h>

IMCStatement IMCStatement::create_assignment(const std::string &place,
                                             const IMCStatement &rhs) {
  IMCStatement result;
  result.m_LHS = place;
  result.m_RHS = std::make_unique<IMCStatement>(rhs);
  result.m_Type = IMCStatement::StatementType::Assignment;

  return result;
}

IMCStatement::IMCStatement(const IMCStatement &other)
    : m_Type(other.m_Type), m_Text(other.m_Text), m_Number(other.m_Number),
      m_LHS(other.m_LHS) {
  if (other.m_RHS.get()) {
    this->m_RHS = std::make_unique<IMCStatement>(*other.m_RHS);
  }
}

IMCStatement &IMCStatement::operator=(const IMCStatement &other) {
  this->m_Type = other.m_Type;
  this->m_Text = other.m_Text;
  this->m_Number = other.m_Number;
  this->m_LHS = other.m_LHS;

  if (other.m_RHS.get()) {
    this->m_RHS = std::make_unique<IMCStatement>(*other.m_RHS);
  }

  return *this;
}

IMCStatement::IMCStatement() {}

IMCStatement::StatementType IMCStatement::get_type() const {
  return this->m_Type;
}

const std::string &IMCStatement::get_text() const {
  return this->m_Text.value();
}

int64_t IMCStatement::get_number() const { return this->m_Number.value(); }
const std::string &IMCStatement::get_lhs_id() const {
  return this->m_LHS.value();
}
const IMCStatement &IMCStatement::get_rhs() const {
  if (!this->m_RHS.get()) {
    throw std::runtime_error("Attempted to get RHS on statement without RHS");
  }

  return *this->m_RHS;
}

IMCStatement IMCStatement::create_value(int64_t number) {
  IMCStatement stat{};
  stat.m_Type = IMCStatement::StatementType::NumberValue;
  stat.m_Number = number;
  return stat;
}

IMCStatement IMCStatement::create_value(const std::string &text) {
  IMCStatement stat{};
  stat.m_Type = IMCStatement::StatementType::TextValue;
  stat.m_Text = text;
  return stat;
}

bool IMCStatement::operator==(const IMCStatement &other) const {
  bool base_eq = this->m_Type == other.m_Type && this->m_Text == other.m_Text &&
                 this->m_LHS == other.m_LHS && this->m_Number == other.m_Number;

  bool rhs = this->m_RHS.get();
  bool other_rhs = other.m_RHS.get();

  if (rhs == other_rhs) {
    if (rhs) {
      return base_eq && *other.m_RHS == *this->m_RHS;
    }
  } else {
    return false;
  }

  return base_eq;
}

bool IMCStatement::operator!=(const IMCStatement &other) const {
  return !this->operator==(other);
}

IMCGenerator::IMCGenerator(SyntaxTreeNode *root)
    : m_SyntaxTree(root), m_Functions(SymbolTable::empty()),
      m_Variables(SymbolTable::empty()) {}

std::vector<IMCStatement>
IMCGenerator::translate_expression(SyntaxTreeNode *, const std::string &) {
  // TODO: Expression translation
  return {};
}

IMCVariable::VariableType IMCVariable::get_type() const { return this->m_Type; }
const char *IMCVariable::get_text() const {
  if (this->m_Type != IMCVariable::VariableType::Text) {
    throw std::runtime_error("Bad IMCVariable access (Tried to access Text)");
  }
  return this->m_Data.m_Text;
}
int64_t IMCVariable::get_number() const {
  if (this->m_Type != IMCVariable::VariableType::Number) {
    throw std::runtime_error("Bad IMCVariable access");
  }
  return this->m_Data.m_Number;
}
