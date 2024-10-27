#include "opts.hpp"

#include <cstring>
#include <sstream>
#include <format>

namespace sviewer {

using ExpressionPtr = AppConfig::ExpressionPtr;

ParsingException::ParsingException(const std::string& msg) : _msg(msg) { }

std::string ParsingException::What() const {
    return _msg;
}


// Parses "12312" "342"
static unsigned int parseUint(const std::string_view& view) {
    unsigned int result;
    try {
        result = std::stoul(std::string(view));
    } catch (...) {
        throw ParsingException("Can't parse " + std::string(view) + " as uint");
    }
    return result;
}

// Parses "0.231" ".34242" "233"
static float parseFloat(const std::string_view& view) {
    float result;
    try {
        result = std::stof(std::string(view));
    } catch (...) {
        throw ParsingException("Can't parse " + std::string(view) + " as float");
    }
    return result;
}

// Parses "<float>,<float>", e.g. "453,12" "0.23,0"
static sf::Vector2f parseVec2(const std::string_view& view) {
    Splitter splitter(view, ',');
    std::string_view temp;

    std::string_view x, y;

    auto throwErr = []() {
        throw ParsingException("Can't parse float vec2. Use following syntax: <float1>,<float2>.\n    e.g \"--const-uniform vec2:myVec:3.12,4\"");
    };

    if (!splitter.Next(temp)) {
        throwErr();
    }
    x = temp;
    if (!splitter.Next(temp)) {
        throwErr();
    }
    y = temp;
    return sf::Vector2f(parseFloat(x), parseFloat(y));
}

// Parses "t + 2" "0.35t + 12" "t^0.12"
static ExpressionPtr parseExpr(const std::string_view& view) {
    try {
        return std::make_unique<Expression>(std::string(view), std::vector<std::string>({"t"}));
    } catch (...) {
        throw ParsingException("Can't parse expression " + std::string(view));
    }
}

// Parses "<expr>,<expr>" e.g. "2t,3"
static std::pair<ExpressionPtr, ExpressionPtr> parseExprVec2(const std::string_view& view) {
    auto throwParseError = []() {
        throw ParsingException("Can't parse vec2 expression. Usage: --expr-uniform vec2:<name>=<expr>,<expr>");
    };
    
    Splitter splitter(view, ',');
    std::string_view first, second;
    if (!splitter.Next(first)) {
        throwParseError();
    }
    if (!splitter.Next(second)) {
        throwParseError();
    }
    try {
        return std::make_pair(parseExpr(first), parseExpr(second)); 
    } catch (...) {
        throwParseError();
    }
}

// Parses "<type>:<name>=<value>" e.g. "float:e1=2t+5" "vec2:e2=t,2t" 
static void parseExprUniform(const std::string_view& view, AppConfig& out) {
    auto throwParseError = []() {
        throw ParsingException("Can't parse expression uniform. Usage: --expr-uniform <type>:<name>=<expr>");
    };
    auto throwUndefinedTypeError = [](const std::string& type) {
        throw ParsingException("Unsupported type " + type);
    };
    
    Splitter splitter(view, ':');
    std::string_view type, other;
    if (!splitter.Next(type)) {
        throwParseError();
    }
    if (!splitter.Next(other)) {
        throwParseError();
    }

    Splitter splitterEq(other, '=');
    std::string_view name, expr;
    if (!splitterEq.Next(name)) {
        throwParseError();
    }
    if (!splitterEq.Next(expr)) {
        throwParseError();
    }

    if (type == "float") {
        out.floatExprs[std::string(name)] = parseExpr(expr);
    } else if (type == "vec2") {
        out.vec2Exprs[std::string(name)] = std::move(parseExprVec2(expr));
    } else {
        throwUndefinedTypeError(std::string(type));
    }
}

// Parses "<type>:<name>=<value>" e.g. "float:c1=0.25" "vec2:c2=0.1,0.2" "uint:c3=12" 
static void parseConstUniform(const std::string_view& view, AppConfig& out) {
    // view = "<type>:<name>=<value>"
    Splitter splitterColon(view, ':');
    
    std::string_view type, name, value;

    auto throwErr = []() {
        throw ParsingException("Can't parse const uniform arg! Required format: <type>:<name>=<value>");
    };

    // Splitting line
    std::string_view temp;
    if(!splitterColon.Next(temp)) {
        throwErr();
    }
    type = temp;
    if (!splitterColon.Next(temp)) {
        throwErr();
    }
    std::string_view nameAndValue = temp;
    Splitter splitterEqual(nameAndValue, '=');

    if (!splitterEqual.Next(temp)) {
        throwErr();
    }
    name = temp;
    if (!splitterEqual.Next(temp)) {
        throwErr();
    }
    value = temp;
    // Error if something else is left
    if (splitterEqual.Next(temp) || splitterColon.Next(temp)) {
        throwErr();
    }

    // Checking type
    if (type == "float") {
        out.floatConstArgs[std::string(name)] = parseFloat(value);
    }
    else if (type == "uint") {
        out.uintConstArgs[std::string(name)] = parseUint(value);
    }
    else if (type == "vec2") {
        out.vec2ConstArgs[std::string(name)] = parseVec2(value);
    }
    else {
        throw ParsingException("Type " + std::string(type) + " is not supported.\nList of supported types: " + CONST_UNIFORM_SUPPORTED_TYPES_STRING);
    }
}

AppConfig parseArgs(int argc, char ** argv) {
    AppConfig cfg;
    std::vector<std::string> positional;
    for (int i = 1; i < argc; ++i) {
        if        (std::strcmp(argv[i], "--const-uniform") == 0 ||
                   std::strcmp(argv[i], "-c") == 0) {

            if (i + 1 == argc) {
                throw ParsingException(std::string(argv[i]) + " opt requires argument!");
            }
            parseConstUniform(argv[i + 1], cfg);
            ++i;

        } else if (std::strcmp(argv[i], "--width") == 0 ||
                   std::strcmp(argv[i], "-ww") == 0) {

            // Check if there'a argument next to current
            if (i + 1 == argc) {
                throw ParsingException(std::string(argv[i]) + " opt requires argument!");
            }
            cfg.windowWidth = parseUint(argv[i + 1]);
            ++i;

        } else if (std::strcmp(argv[i], "--height") == 0 ||
                   std::strcmp(argv[i], "-wh") == 0) {

            // Check if there'a argument next to current
            if (i + 1 == argc) {
                throw ParsingException(std::string(argv[i]) + " opt requires argument!");
            }
            cfg.windowHeight = parseUint(argv[i + 1]);
            ++i;

        } else if (std::strcmp(argv[i], "--expr-uniform") == 0 ||
                   std::strcmp(argv[i], "-e") == 0) {

            // Check if there'a argument next to current
            if (i + 1 == argc) {
                throw ParsingException(std::string(argv[i]) + " opt requires argument!");
            }
            parseExprUniform(argv[i + 1], cfg);
            ++i;

        } else if (argv[i][0] == '-') { // Unknown opt
            throw ParsingException("Don't know opt " + std::string(argv[i]));
        }else {                        // Positional arg
            positional.push_back(std::string(argv[i]));
        }
    }
    if (positional.size() < 1) {
        throw ParsingException("You must specify shader filename!");
    }

    // Handle positional argument
    cfg.shaderName = positional[0];

    return cfg;
}

} // namespace sviewer
