#ifndef SPL_IMC_H
#define SPL_IMC_H

#include <parser.h>
#include <symbol.h>

class IMCStatement {
public:
  enum class StatementType { Assignment, NumberValue, TextValue };

  IMCStatement(const IMCStatement &);
  IMCStatement &operator=(const IMCStatement &);
  bool operator==(const IMCStatement &) const;
  bool operator!=(const IMCStatement &) const;

  static IMCStatement create_assignment(const std::string &place,
                                        const IMCStatement &rhs);
  static IMCStatement create_value(int64_t number);
  static IMCStatement create_value(const std::string &text);

  StatementType get_type() const;
  const std::string &get_text() const;
  int64_t get_number() const;
  const std::string &get_lhs_id() const;
  const IMCStatement &get_rhs() const;

private:
  IMCStatement();

  StatementType m_Type;
  std::optional<std::string> m_Text;

  std::optional<int64_t> m_Number;
  std::optional<std::string> m_LHS;
  std::unique_ptr<IMCStatement> m_RHS;
};

class IMCGenerator {
public:
  IMCGenerator(SyntaxTreeNode *root);
  ~IMCGenerator();

private:
  std::vector<IMCStatement> translate_expression(SyntaxTreeNode *expr,
                                                 const std::string &place);

  SyntaxTreeNode *m_SyntaxTree;
  std::shared_ptr<SymbolTable> m_Functions;
  std::shared_ptr<SymbolTable> m_Variables;
};

class IMCVariable {
public:
  enum class VariableType { Number, Text };

  VariableType get_type() const;
  const char *get_text() const;
  int64_t get_number() const;

  static IMCVariable make_variable(const std::string &id, int64_t data) {
    IMCVariable result{};
    result.m_ID = id;
    result.m_Type = IMCVariable::VariableType::Number;
    result.m_Data.m_Number = data;

    return result;
  }

  static IMCVariable make_variable(const std::string &id, const char *data) {
    IMCVariable result{};
    result.m_ID = id;
    result.m_Type = IMCVariable::VariableType::Text;
    result.m_Data.m_Text = data;

    return result;
  }

private:
  IMCVariable() = default;

  std::string m_ID;
  VariableType m_Type;

  union {
    const char *m_Text;
    int64_t m_Number;
  } m_Data;
};

#endif
