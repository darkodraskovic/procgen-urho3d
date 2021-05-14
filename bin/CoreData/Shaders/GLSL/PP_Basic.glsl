#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

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
    // vec3 rgb = texture2D(sDiffMap, vScreenPos).rgb;
    // float intensity = GetIntensity(rgb);
    
    vec3 pos = vec3(vScreenPos.x, 1 - vScreenPos.y, 1);
    
    pos = rotate2D(-cElapsedTimePS) * translate2D(.3, .3) * pos;
    float rect = rectFill(pos.xy, vec2(.3, .3), vec2(.5));
    vec4 diffColor = vec4(rect * cos(cElapsedTimePS), rect * sin(cElapsedTimePS), 0, 1);

    gl_FragColor = diffColor;
}
