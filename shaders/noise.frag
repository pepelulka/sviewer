#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 resolution;
uniform float time;

uniform vec2 centerPos;
uniform float deltaX;


uniform float x, y;

vec2 random2(vec2 st){
    st = vec2( dot(st,vec2(127.1,311.7)),
              dot(st,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(st)*43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f*f*(3.0-2.0*f);

    return mix( mix( dot( random2(i + vec2(0.0,0.0) ), f - vec2(0.0,0.0) ),
                     dot( random2(i + vec2(1.0,0.0) ), f - vec2(1.0,0.0) ), u.x),
                mix( dot( random2(i + vec2(0.0,1.0) ), f - vec2(0.0,1.0) ),
                     dot( random2(i + vec2(1.0,1.0) ), f - vec2(1.0,1.0) ), u.x), u.y);
}

vec2 get_uv() {
    vec2 uv = gl_FragCoord.xy;
    return deltaX * uv + centerPos * 1000.0;
}

void main() {
    vec2 uv = get_uv();
    vec2 st = uv/resolution.xy;
    st.x *= resolution.x/resolution.y;
    vec3 color = vec3(0.0);

    float t = 1.0;
    // Uncomment to animate
    t = abs(x-sin(time*.1))*y;
    st += noise(st*2.)*t; // Animate the coordinate space
    color = vec3(1.) * smoothstep(.18,.2,noise(st)); // Big black drops
    color += smoothstep(.15,.2,noise(st*10.)); // Black splatter
    color -= smoothstep(.35,.4,noise(st*10.)); // Holes on splatter

    gl_FragColor = vec4(1.-color,1.0);
}
