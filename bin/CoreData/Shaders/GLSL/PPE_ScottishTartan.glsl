#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

#include "Shapes2D.glsl"
#include "Transform2D.glsl"
#include "Utilities2D.glsl"

#define NUM_LAYERS 8

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
    
    float lineW = .2;
    vec2 layers[NUM_LAYERS];

    for (int i = 0; i < NUM_LAYERS; i++) {
        float numTiles = pow(2, i+1);
        layers[i] = tile2D(st, vec2(numTiles, numTiles));
    }

    float colors[3] = float[](0,0,0);
    for (int i = 0; i < NUM_LAYERS; i++) {
        vec3 pos = vec3(layers[i], 1);
        pos = translate2D(-lineW/2, -lineW/2) * pos;
        colors[i%3] += rectFill(pos.xy, vec2(1 + lineW/2,lineW), vec2(0));
        colors[i%3] += rectFill(pos.xy, vec2(lineW,1 + lineW/2), vec2(0));
    }

    colors[0] += rectStroke(st.xy, vec2(1,1), vec2(0), lineW/4);
    
    gl_FragColor = vec4(colors[0], colors[1], colors[2], 1);
}
