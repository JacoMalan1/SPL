#ifndef SPL_SYMBOL_H
#define SPL_SYMBOL_H

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Symbol
{
public:
  Symbol(const std::string &identifier, std::string type);

  const std::string &name() const;
  std::string type() const;

  bool operator==(const Symbol &other) const;

  std::vector<std::string> getParamTypes() const;
  std::string getReturnType() const;

  void setParamTypes(std::vector<std::string> paramTypes);
  void setReturnType(std::string returnType);

private:
  std::string m_Ident;
  std::string m_Type;
  std::vector<std::string> paramTypes; // function symbol parameter types
  std::string returnType;              // function symbol return type
};

class SymbolTable
{
public:
  SymbolTable(const SymbolTable &other);
  SymbolTable &operator=(const SymbolTable &other);

  static std::shared_ptr<SymbolTable> empty();

  void bind(const Symbol &information);
  std::optional<Symbol> lookup(const std::string &identifier) const;

  void enter();
  void exit();

private:
  SymbolTable();

  std::shared_ptr<SymbolTable> m_Parent;
  std::unordered_map<std::string, Symbol> m_Symbols;
};

#endif
