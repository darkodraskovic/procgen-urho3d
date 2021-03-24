#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"

varying vec4 vTexCoord;
varying vec4 vTangent;

varying vec3 vNormal;
varying vec3 vReflectionVec;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vNormal = GetWorldNormal(modelMatrix);
    
#ifdef NORMALMAP
    vec4 tangent = GetWorldTangent(modelMatrix);
    vec3 bitangent = cross(tangent.xyz, vNormal) * tangent.w;
    vTexCoord = vec4(GetTexCoord(iTexCoord), bitangent.xy);
    vTangent = vec4(tangent.xyz, bitangent.z);
#else
    vTexCoord = GetTexCoord(iTexCoord);
#endif
    
    vReflectionVec = worldPos - cCameraPos;
}

void PS()
{
    vec4 color = cMatDiffColor;

    vec4 diffInput = texture2D(sDiffMap, vTexCoord.xy);

    color *= diffInput;

#ifdef NORMALMAP
    mat3 tbn = mat3(vTangent.xyz, vec3(vTexCoord.zw, vTangent.w), vNormal);
    vec3 normal = normalize(tbn * DecodeNormal(texture2D(sNormalMap, vTexCoord.xy)));
#else
    vec3 normal = normalize(vNormal);
#endif

#ifdef ENVCUBEMAP
    vec3 envInput = cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(vReflectionVec, normal)).rgb;
    color.xyz += envInput;
#endif
            
    gl_FragColor = color;
}
