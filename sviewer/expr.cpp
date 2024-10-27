#include "expr.hpp"

#include <iostream>

namespace sviewer
{

Expression::Expression(const std::string& base,
                       const std::vector<std::string>& vars) : _stringBase(base) {
    _varsArray.resize(vars.size(), 0);

    for (int currentVarInd = 0; currentVarInd < vars.size(); currentVarInd++) {
        _varsEntries[ vars[currentVarInd] ] = currentVarInd;
        _table.add_variable(vars[currentVarInd], _varsArray[currentVarInd] );
    }

    _expr.register_symbol_table(_table);
    _parser.compile(base, _expr);
}

void Expression::SetVar(const std::string& name, float value) {
    _varsArray[ _varsEntries[name] ] = value;
}

float Expression::Value() {
    return _expr.value();
}

} // namespace sviewer
