#include "viewer-core/core.hpp"
#include "config.hpp"
#include "opts.hpp"

using namespace sviewer;

// expressionPtr must point to Expression instance
float TimeFunctionFromExpression(float time, void* expressionPtr) {
    Expression *expr = static_cast<Expression*>(expressionPtr);
    expr->SetVar("t", time);
    return expr->Value();
}

sf::Vector2f TimeFunctionFromExpressionVec2(float time, void* data1, void* data2) {
    using ExpressionPtr = AppConfig::ExpressionPtr;
    
    Expression *expr1 = static_cast<Expression*>(data1);
    Expression *expr2 = static_cast<Expression*>(data2);

    expr1->SetVar("t", time);
    expr2->SetVar("t", time);

    return sf::Vector2f(expr1->Value(), expr2->Value());
}

void appRun(const AppConfig& cfg) {
    ShaderViewerWindow window(cfg.windowHeight, cfg.windowWidth);
    window.SetShaderFromFile(cfg.shaderName);

    // Setting up const uniforms
    for (const auto& p : cfg.floatConstArgs) {
        window.SetUniform<float>(p.first, p.second);
    }
    for (const auto& p : cfg.uintConstArgs) {
        window.SetUniform<int>(p.first, p.second);
    }
    for (const auto& p : cfg.vec2ConstArgs) {
        window.SetUniform<sf::Vector2f>(p.first, p.second);
    }

    for (auto& p : cfg.floatExprs) {
        window.AttachTimeFunction(p.first, TimeFunctionFromExpression, static_cast<void*>(p.second.get()));
    }

    for (auto& p : cfg.vec2Exprs) {
        window.AttachTimeFunctionVec2(p.first, TimeFunctionFromExpressionVec2, p.second.first.get(), p.second.second.get()); // ENDED HERE
    }

    window.Start();
}

int main(int argc, char ** argv) {
    AppConfig cfg;
    try {
        cfg = parseArgs(argc, argv);
    } catch (const ParsingException& e) {
        std::cout << "Error: " << e.What() << std::endl;
        exit(EXIT_FAILURE);
    }

    appRun(cfg);
}