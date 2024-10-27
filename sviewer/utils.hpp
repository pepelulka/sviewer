#pragma once

#include <string>
#include <unordered_set>
#include <stdexcept>
#include <string_view>

namespace sviewer {

// Util class for string splitting
class Splitter {
private:
    char _delim;
    std::string_view _base;
    int ptr = -1;

public:
    Splitter(const std::string_view& base, char delimiter);

    bool Next(std::string_view& result);
};

}
