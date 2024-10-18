#include <cstring>
#include <gtest/gtest.h>
#include <random>
#include <sstream>
#include <symbol.h>

class SymbolTableFixture : public testing::Test {
protected:
  SymbolTableFixture() : m_Table(SymbolTable::empty()) {}

  SymbolTable &table() { return *this->m_Table; }
  const SymbolTable &table() const { return *this->m_Table; }

  Symbol bind_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    const char *letters = "abcdefghijklmnopqrstuvwxyz";

    std::uniform_int_distribution<> letter_dist(0, std::strlen(letters) - 1);
    std::uniform_int_distribution<> length_dist(1, 64);

    uint32_t len = length_dist(gen);
    std::stringstream stream;
    for (uint32_t i = 0; i < len; i++) {
      stream << letters[letter_dist(gen)];
    }

    Symbol new_symbol = Symbol(stream.str(), Symbol::SymbolType::Variable);
    this->table().bind(new_symbol);
    return new_symbol;
  }

private:
  std::shared_ptr<SymbolTable> m_Table;
};

TEST_F(SymbolTableFixture, EmptyTest) {
  ASSERT_FALSE(table().lookup("x").has_value());
}

TEST_F(SymbolTableFixture, SingleSymbol) {
  table().bind(Symbol("x", Symbol::SymbolType::Variable));
  Symbol s = bind_random();
  std::optional<Symbol> x = table().lookup(s.name());

  ASSERT_TRUE(x.has_value());
  ASSERT_FALSE(table().lookup("y").has_value());
  ASSERT_EQ(x.value(), s);
}

TEST_F(SymbolTableFixture, SingleEnter) {
  Symbol s = bind_random();
  table().enter();

  ASSERT_FALSE(table().lookup(s.name()).has_value());
}

TEST_F(SymbolTableFixture, EnterExit) {
  Symbol s = bind_random();
  table().enter();

  ASSERT_FALSE(table().lookup(s.name()).has_value());
  table().exit();
  ASSERT_TRUE(table().lookup(s.name()).has_value());
}
