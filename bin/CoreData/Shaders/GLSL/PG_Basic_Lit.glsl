#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "Lighting.glsl"

#if defined(DIFFMAP) || defined(ALPHAMAP)
    varying vec2 vTexCoord;
#endif
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif

varying vec3 vWorldPos;
varying vec3 vAttribNormal;
varying vec3 vWorldNormal;
varying vec3 vViewDir;

uniform vec4 cDiffColor = vec4(.5, .7, .6, 1);
uniform vec4 cSpecColor = vec4(1, .5, .5, 16); // alpha holds specularPower

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    
    #ifdef DIFFMAP
        vTexCoord = iTexCoord;
    #endif
    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif

    vWorldPos = worldPos;
    vAttribNormal = iNormal;
    vWorldNormal = GetWorldNormal(modelMatrix);
    vViewDir = normalize(cCameraPos - worldPos);
}

void PS()
{
    vec4 diffColor = cDiffColor;

    #ifdef VERTEXCOLOR
        diffColor *= vColor;
    #endif

    // LIGHT
    vec3 lightDir;
    float diff = GetDiffuse(vWorldNormal, vWorldPos.xyz, lightDir);
    vec3 diffLight = diff * cLightColor.rgb;

    // SPECULAR
    float spec = GetSpecular(vWorldNormal, vViewDir, cLightDirPS, cSpecColor.a);
    vec3 specColor = spec * cSpecColor.rgb * cLightColor.a;
        
    #if (!defined(DIFFMAP)) && (!defined(ALPHAMAP))
        gl_FragColor = diffColor;
    #endif
    #ifdef DIFFMAP
        vec4 diffInput = texture2D(sDiffMap, vTexCoord);
        #ifdef ALPHAMASK
            if (diffInput.a < 0.5)
                discard;
        #endif
            
        gl_FragColor = vec4( diffLight * (diffColor.rgb + specColor), 1);
        
    #endif
    #ifdef ALPHAMAP
        #ifdef GL3
            float alphaInput = texture2D(sDiffMap, vTexCoord).r;
        #else
            float alphaInput = texture2D(sDiffMap, vTexCoord).a;
        #endif
        gl_FragColor = vec4(diffColor.rgb, diffColor.a * alphaInput);
    #endif
}
