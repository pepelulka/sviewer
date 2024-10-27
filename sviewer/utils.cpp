#include "utils.hpp"

namespace sviewer {

Splitter::Splitter(const std::string_view& base, char delimiter) {
    _delim = delimiter;
    _base = base;
    ptr = -1;
}

bool Splitter::Next(std::string_view& result) {
    if (ptr + 1 >= _base.size()) {
        return false;
    }
    ptr++;
    int startPtr = ptr;
    while (ptr < _base.size() && _base[ptr] != _delim) {
        ptr++;
    }

    result = std::string_view(_base.data() + startPtr, ptr - startPtr);
    return true;
}

}
