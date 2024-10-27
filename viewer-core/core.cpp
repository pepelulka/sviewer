#include "core.hpp"

namespace sviewer {

ShaderViewerWindow::ShaderViewerWindow(size_t h, size_t w) :
                                                    height(h),
                                                    width(w),
                                                    window(sf::VideoMode(w, h), "Shader Viewer") { }

void ShaderViewerWindow::RecalculateTimeFunctions(float time) {
    // float functions
    for (const auto& p : attachedTimeFunctions) {
        auto& name = p.first;
        auto& func = p.second.first;
        void* data = p.second.second;

        SetUniform(name, func(time, data));
    }

    // vec2 functions
    for (const auto& p : attachedTimeFunctionsVec2) {
        auto& name = p.first;
        auto& func = std::get<0>(p.second);
        void* data1 = std::get<1>(p.second);
        void* data2 = std::get<2>(p.second);

        auto result = func(time, data1, data2);
        SetUniform(name, result);
    }
}

void ShaderViewerWindow::Start() {
    const float widthf = static_cast<float>(width), heightf = static_cast<float>(height);
    const float ratio = widthf / heightf;
    sf::Vector2f centerPos = sf::Vector2f(0, 0);

    // Setting up default uniforms
    float deltaX = 2.f;
    shader.setUniform("resolution", sf::Vector2f(widthf, heightf));
    shader.setUniform("centerPos", centerPos);
    shader.setUniform("deltaX", deltaX);

    // Create texture to display shader
    sf::Texture tex;
    tex.create(width, height);
    sf::Sprite spr(tex);

    // Handle mouse movement
    bool mousePressed = false;
    sf::Vector2i mousePosWhenPressed;
    sf::Vector2f lastCenterPos;

    // Setting up clock
    sf::Clock clk;
    clk.restart();

    while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);)
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // Events for changing position and zoom
            if (event.type == sf::Event::MouseButtonPressed) {
                mousePressed = true;
                mousePosWhenPressed = sf::Mouse::getPosition(window);
                lastCenterPos = centerPos;
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                mousePressed = false;
                centerPos = lastCenterPos;
            }
            if (event.type == sf::Event::MouseMoved && mousePressed) {
                sf::Vector2i moved = sf::Vector2i(event.mouseMove.x, event.mouseMove.y) - mousePosWhenPressed;
                sf::Vector2f offset = sf::Vector2f(moved.x / widthf, moved.y / heightf);
                offset.x = -offset.x;
                lastCenterPos = centerPos + sf::Vector2f(offset.x * deltaX, offset.y * deltaX / ratio);
                shader.setUniform("centerPos", lastCenterPos);
            }
            if (event.type == sf::Event::MouseWheelMoved && (!mousePressed)) {
                auto delta = event.mouseWheel.delta;
                deltaX *= std::pow(1.2, -delta);
                shader.setUniform("deltaX", deltaX);
            }
        }

        float time = clk.getElapsedTime().asSeconds();
        shader.setUniform("time", time);
        
        // Recalculating attached time functions:
        RecalculateTimeFunctions(time);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        mousePos.y = height - mousePos.y;
        sf::Vector2f mousePosf = sf::Vector2f(mousePos.x, mousePos.y);
        shader.setUniform("mouse", mousePosf);

        window.clear();
        window.draw(spr, &shader);
        window.display();
    }
}

bool ShaderViewerWindow::SetShader(const std::string& shaderSource) {
    return shader.loadFromMemory(shaderSource, sf::Shader::Fragment);
}

bool ShaderViewerWindow::SetShaderFromFile(const std::string& filename) {
    return shader.loadFromFile(filename, sf::Shader::Fragment);
}

} // namespace sviewer
