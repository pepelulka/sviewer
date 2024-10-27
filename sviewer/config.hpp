#pragma once

#include "viewer-core/core.hpp"
#include "expr.hpp"

#include <unordered_map>
#include <memory>

namespace sviewer {

struct AppConfig {
    using ExpressionPtr = std::unique_ptr<Expression>;

    std::string shaderName;
    unsigned int windowWidth = 800;
    unsigned int windowHeight = 600;

    // --const-uniform
    std::unordered_map<std::string, float> floatConstArgs;
    std::unordered_map<std::string, unsigned int> uintConstArgs;
    std::unordered_map<std::string, sf::Vector2f> vec2ConstArgs;

    // --expr-uniform
    std::unordered_map<std::string, ExpressionPtr > floatExprs;
    std::unordered_map<std::string, std::pair<ExpressionPtr, ExpressionPtr> > vec2Exprs;
};

} // namespace sviewer
