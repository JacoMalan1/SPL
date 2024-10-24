#include <stdexcept>
#include <symbol.h>

Symbol::Symbol(const std::string &identifier, std::string type)
    : m_Ident(identifier), m_Type(type), returnType("void") {}

std::string Symbol::type() const { return this->m_Type; }
const std::string &Symbol::name() const { return this->m_Ident; }

bool Symbol::operator==(const Symbol &other) const
{
  return this->m_Type == other.m_Type && this->m_Ident == other.m_Ident;
}

std::vector<std::string> Symbol::getParamTypes() const
{
  return this->paramTypes;
}

void Symbol::setParamTypes(std::vector<std::string> paramTypes)
{
  this->paramTypes = paramTypes;
}

SymbolTable::SymbolTable() : m_Parent(nullptr) {}

SymbolTable::SymbolTable(const SymbolTable &other)
{
  this->m_Symbols = other.m_Symbols;
  if (other.m_Parent.get())
  {
    this->m_Parent = std::make_shared<SymbolTable>(*other.m_Parent);
  }
}

SymbolTable &SymbolTable::operator=(const SymbolTable &other)
{
  this->m_Symbols = other.m_Symbols;
  if (other.m_Parent.get())
  {
    this->m_Parent = std::make_shared<SymbolTable>(*other.m_Parent);
  }

  return *this;
}

std::shared_ptr<SymbolTable> SymbolTable::empty()
{
  return std::make_shared<SymbolTable>(SymbolTable());
}

void SymbolTable::bind(const Symbol &information)
{
  this->m_Symbols.emplace(std::make_pair(information.name(), information));
}

std::optional<Symbol> SymbolTable::lookup(const std::string &identifier) const
{
  try
  {
    return this->m_Symbols.at(identifier);
  }
  catch (const std::out_of_range &e)
  {
    return {};
  }
}

void SymbolTable::enter()
{
  this->m_Parent = std::make_shared<SymbolTable>(*this);
  // this->m_Symbols = {};
}

void SymbolTable::exit()
{
  if (!this->m_Parent.get())
  {
    throw std::runtime_error(
        "Attempted to exit a symbol table with no surrounding scope.");
  }

  this->m_Symbols = this->m_Parent->m_Symbols;
  this->m_Parent = this->m_Parent->m_Parent;
}
