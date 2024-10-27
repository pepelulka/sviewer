#include "config.hpp"
#include "utils.hpp"

namespace sviewer {

const std::string CONST_UNIFORM_SUPPORTED_TYPES_STRING =
    "uint, float, vec2";

const std::string CONST_EXPRESSION_SUPPORTED_TYPES_STRING =
    "float, vec2";

// Exception for parsing related errors
class ParsingException {
private:
    std::string _msg;
public: 
    ParsingException(const std::string& msg);

    std::string What() const;
};

AppConfig parseArgs(int argc, char ** argv);

} // namespace sviewer
