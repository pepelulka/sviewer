#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <unordered_map>
#include <cmath>
#include <functional>

namespace sviewer {

/*
Class to view fragment shaders


Always gives to a shader following uniforms:

uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

uniform vec2 centerPos;
uniform float deltaX;

*/

struct UniformsSet {

    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, uint> uints;
    std::unordered_map<std::string, sf::Vector2f> vec2s;
    std::unordered_map<std::string, sf::Vector3f> vec3s;

};

class ShaderViewerWindow {
public:
    using CustomTimeFunction = std::function<float(float, void*)>;
    using CustomTimeFunctionVec2 = std::function<sf::Vector2f(float, void*, void*)>;

    ShaderViewerWindow(size_t h, size_t w);

    void Start();

    bool SetShader(const std::string& shaderSource);
    bool SetShaderFromFile(const std::string& filename);

    // Directly setting constant uniforms:
    template <typename T>
    void SetUniform(const std::string name, const T& value) {
        shader.setUniform(name, value);
    }

    void SetUniforms(const UniformsSet& set) {
        std::for_each(set.floats.begin(), 
                      set.floats.end(), 
                      [=](auto p){ SetUniform(p.first, p.second); } );
        
        std::for_each(set.uints.begin(), 
                      set.uints.end(), 
                      [=](auto p){ SetUniform(p.first, (int)p.second); } );

        std::for_each(set.vec2s.begin(), 
                      set.vec2s.end(), 
                      [=](auto p){ SetUniform(p.first, p.second); } );
        
        std::for_each(set.vec3s.begin(), 
                      set.vec3s.end(), 
                      [=](auto p){ SetUniform(p.first, p.second); } );
        
    }

    // Attach function of time
    // Each frame this uniforms will be recalculated using attached function
    void AttachTimeFunction(const std::string& uniformName, CustomTimeFunction func, void *data) {
        attachedTimeFunctions[uniformName] = {func, data};
    }

    void AttachTimeFunctionVec2(const std::string& uniformName, CustomTimeFunctionVec2 func, void *data1, void *data2) {
        attachedTimeFunctionsVec2[uniformName] = {func, data1, data2};
    }

private:
    size_t height, width;
    sf::Shader shader;
    sf::RenderWindow window;

    std::unordered_map<std::string, std::pair<CustomTimeFunction, void*> > attachedTimeFunctions;
    std::unordered_map<std::string, std::tuple<CustomTimeFunctionVec2, void*, void*> > attachedTimeFunctionsVec2;

    void RecalculateTimeFunctions(float time);
};

} // namespace sviewer
