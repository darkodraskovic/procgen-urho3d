#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "Constants.glsl"
#include "Utilities2D.glsl"
#include "Transform2D.glsl"
#include "Shapes2D.glsl"

varying vec2 vScreenPos;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

void PS()
{
    vec2 st = vec2(vScreenPos.x, 1 - vScreenPos.y);
    vec3 color;
    
    float zoom = 16.;
    st *= zoom;

    // PATTERN
    // st = rotateTilePattern(fract(st));
    st = truchetPattern(fract(st), floor(st));

    // TILES & TRANSFORMS
    // st = tile2D(st, vec2(2.));
    
    // st -= .5;
    // st = mat2Rotate2D(-cElapsedTimePS) * st;
    // st += .5;

    // ================================
    // SHAPES
    
    // TRIANGLES
    // color = vec3(step(st.x, st.y));

    // RECT
    st -= .5;
    st = mat2Rotate2D(M_PI * .25) * st;
    color = vec3(rectStroke(st, vec2(1.2, .2), vec2(.5), .05));

    // CIRCLE
    // color = vec3(circleStroke(st, .6, vec2(.0), .2));
    // st -= 1.0;
    // color += vec3(circleStroke(st, .6, vec2(.0), .2));
    
    gl_FragColor = vec4(color, 1.0);
}
