#ifndef SPL_SYMBOL_H
#define SPL_SYMBOL_H

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

class Symbol {
public:
  enum class SymbolType { Variable };

  Symbol(const std::string &identifier, SymbolType type);

  const std::string &name() const;
  SymbolType type() const;

  bool operator==(const Symbol &other) const;

private:
  SymbolType m_Type;
  std::string m_Ident;
};

class SymbolTable {
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
