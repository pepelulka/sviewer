// Julia set:
//
// f(z) = z^2 + c, where c is uniform

// Custom:
uniform vec2 c;

// Common:
uniform vec2 resolution;
uniform float time;

// precision highp float;

// Config:
// CenterPos (cp), deltaX (dx)
//
// +----------------------------+
// |                            |
// |<--------->(cp)             |
// |     (dx)                   |
// +----------------------------+
// centerPos & deltaX in world coords
//
uniform vec2 centerPos;
uniform float deltaX;

const float THRESHOLD = 1000.0; // Threshold for values z_k
const int MAX_ITER = 1000; // Maximum count of iterations

// We have world coords and screen coords

vec2 screenToWorld(vec2 uv) {
    float dx = deltaX;
    // Transforms:
    // dx = dx / time;
    //
    uv = uv - vec2(0.5, 0.5);
    float ratio = resolution.x / resolution.y;
    float dy = dx / ratio; 
    return centerPos + uv * vec2(dx, dy);
}

// Complex:

#define Complex vec2

Complex complex_Mult(Complex a, Complex b) {
    return Complex(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

Complex complex_Pow2(Complex a) {
    return complex_Mult(a, a);
}

// Main function for computing indicator:
// Indicator is a float between (0, 1) to render color. 1 - belong to set, otherwise - don't belong
float computeIndicator() {
    Complex z = screenToWorld(gl_FragCoord.xy / resolution);
    for (int i = 0; i < MAX_ITER; i++) {
        z = complex_Pow2(z) + c;
        if (length(z) >= THRESHOLD) {
            return pow(float(i) / float(MAX_ITER), 0.5);
        }
    }
    return 1.0;
}

// Indicator to actual color
vec3 indicatorToColor(float ind) {
    return vec3(ind);
}

void main() {
    float indicator = computeIndicator();
    gl_FragColor = vec4(indicatorToColor(indicator), 1.0);
}