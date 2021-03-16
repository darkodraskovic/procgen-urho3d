#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
// #include "Lighting.glsl"

#include "Shapes2D.glsl"
#include "Transform2D.glsl"

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
    // gl_FragColor = vec4(intensity, intensity, intensity, 1.0);
    
    vec3 pos = rotate2D(cElapsedTimePS) * translate2D(.2,.2) * vec3(vScreenPos, 1);
    float rect1 = rectFill(pos.xy, vec2(.2,.2), vec2(.5));
    pos = translate2D(.3,.3) * vec3(vScreenPos, 1);
    float rect2 = rectFill(pos.xy, vec2(.4,.2));
    
    gl_FragColor = vec4(rect1, rect2, 0, 1);
}
