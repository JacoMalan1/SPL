#include "parser_file_handler.h"
#include <sstream>
#include <iostream>

ParserFileHandler::ParserFileHandler()
{
    loadParseTable();
    loadGrammarRules();
}

std::map<std::string, std::map<std::string, std::string>> ParserFileHandler::getParseTable() const
{
    return this->parseTable;
}

std::vector<std::pair<std::string, std::vector<std::string>>> ParserFileHandler::getGrammarRules() const
{
    return this->grammarRules;
}

std::vector<std::string> splitCSVLine(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string token;
    bool inQuotes = false;
    char c;

    while (ss.get(c))
    {
        if (c == '\"')
        {
            inQuotes = !inQuotes; // toggle quoted field
        }
        else if (c == ',' && !inQuotes)
        {
            result.push_back(token); // end of a field, push to result
            token.clear();
        }
        else
        {
            token += c; // add character to current token
        }
    }
    result.push_back(token); // push the last token

    return result;
}

std::vector<std::vector<std::string>> readCSV(const std::string &input)
{
    std::stringstream ss(input);
    std::vector<std::vector<std::string>> data;
    std::string line;

    // read each line of the CSV
    while (std::getline(ss, line))
    {
        // split the line into fields, handling quoted fields correctly
        std::vector<std::string> row = splitCSVLine(line);
        data.push_back(row);
    }

    return data;
}

void ParserFileHandler::loadParseTable()
{
    std::vector<std::vector<std::string>> data = readCSV(PARSE_TABLE_INPUT);
    std::vector<std::string> headers = data[1];

    if (!data.empty())
    {
        for (size_t m = 2; m < data.size(); ++m)
        {
            std::string state = data[m][0];
            for (size_t n = 1; n < headers.size(); ++n)
            {
                std::string token = data[m][n];
                this->parseTable[state][headers[n]] = token;
            }
        }
    }
}

void ParserFileHandler::loadGrammarRules()
{
    std::stringstream ss(GRAMMAR_RULES_INPUT);
    std::string line;

    while (std::getline(ss, line))
    {
        std::istringstream iss(line);
        std::string lhs, arrow, rhs;
        if (!(iss >> lhs >> arrow))
        {
            std::cerr << "Error: Invalid grammar rule format in line: " << line << std::endl;
            continue;
        }

        std::getline(iss, rhs);
        rhs.erase(0, rhs.find_first_not_of(" \t")); // trim leading whitespace

        std::vector<std::string> rhsSymbols;
        std::istringstream rhsStream(rhs);
        std::string symbol;

        // std::cout << lhs << " -> ";
        while (rhsStream >> symbol)
        {
            if (symbol != "''")
            {
                rhsSymbols.push_back(symbol);
            }

            // std::cout << symbol << " ";
        }
        // std::cout << std::endl;

        grammarRules.push_back({lhs, rhsSymbols});
    }
}
