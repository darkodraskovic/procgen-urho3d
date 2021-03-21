#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"

varying vec2 vTexCoord;

varying vec3 vNormal;
varying vec3 vReflectionVec;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    
    vTexCoord = iTexCoord;
    
    // vNormal = iNormal;
    vNormal = GetWorldNormal(modelMatrix);
    vReflectionVec = worldPos - cCameraPos;
}

void PS()
{
    vec4 diffColor = cMatDiffColor;

    vec4 diffInput = texture2D(sDiffMap, vTexCoord);

    vec3 normColor = abs(vNormal);

    vec3 envColor = cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(vReflectionVec, vNormal)).rgb;
    vec3 color = mix(envColor, normColor, .5);
            
    gl_FragColor = vec4(color, 1);
}
