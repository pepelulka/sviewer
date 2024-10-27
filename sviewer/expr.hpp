#pragma once

#include "exprtk.hpp"

#include <unordered_map>

namespace sviewer
{

// Class for float expression    
class Expression {
private:
    // String expression has been parsed from
    std::string _stringBase;
    // _vars contains variables
    std::vector<float> _varsArray;
    std::unordered_map<std::string, uint> _varsEntries; // Stores index of var in _varsArray array 
    // Symbol table contains references to variables
    exprtk::symbol_table<float> _table;
    exprtk::expression<float> _expr;
    exprtk::parser<float> _parser;

public:

    Expression() = delete;
    Expression(const std::string& base, const std::vector<std::string>& vars);

    void SetVar(const std::string& name, float value);

    float Value();
};

} // namespace sviewer


