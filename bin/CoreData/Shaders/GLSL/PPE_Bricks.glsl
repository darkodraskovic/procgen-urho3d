#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "Constants.glsl"

#include "Transform2D.glsl"
#include "Shapes2D.glsl"
#include "Utilities2D.glsl"

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
    vec3 pos = vec3(vScreenPos.x, 1 - vScreenPos.y, 1);

    float numX = 20;
    float everyX = 3;
    float numY = 20;
    float everyY = 2;

    float time = cElapsedTimePS * 2;
    
    float offsetX = time;
    float offsetY = time;
    if (mod(floor(numY * pos.y), 2.0) == 0) {
        offsetX = -offsetX;
    } 
    if (mod(floor(numX * pos.x), 2.0) == 0) {
        offsetY = -offsetY;
    } 
    
    // float offset = .5;
    vec2 offset = vec2(offsetX,0);
    if (mod(floor(time / 2), 2.0) == 0) offset = vec2(0,offsetY);

    float mortarX = .9;
    float mortarY = .8;
    
    vec2 brickPos = bricks(pos.xy, vec2(numX,numY), offset, vec2(everyX, everyY));

    // vec3 color = vec3(rectFill(brickPos, vec2(mortarX, mortarY), vec2(0)));
    
    vec3 pos1 = translate2D(.5,.5) * vec3(brickPos, 1);
    vec3 color = vec3(1-circleFillSmooth(pos1.xy, .4, vec2(0), .07));
    
    gl_FragColor = vec4(color, 1.0);
}
