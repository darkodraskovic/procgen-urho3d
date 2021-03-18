#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

#if defined(DIFFMAP) || defined(ALPHAMAP)
    varying vec2 vTexCoord;
#endif
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif

// PG
varying vec3 vNormal;
varying vec4 vScreenPos;
varying vec3 vVertexLight;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);

    #ifdef DIFFMAP
        vTexCoord = GetTexCoord(iTexCoord);
    #endif
    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif

    // PG
    vNormal = GetWorldNormal(modelMatrix);
    vScreenPos = GetScreenPos(gl_Position);

    vVertexLight = GetAmbient(GetZonePos(worldPos));
    #ifdef NUMVERTEXLIGHTS
        for (int i = 0; i < NUMVERTEXLIGHTS; ++i)
            vVertexLight += GetVertexLight(i, worldPos, vNormal) * cVertexLights[i * 3].rgb;
    #endif
}

void PS()
{
    vec4 diffColor = cMatDiffColor;
    diffColor *= vec4(diffColor.xyz * vVertexLight, 1);
    
    #ifdef VERTEXCOLOR
        diffColor *= vColor;
    #endif

    #if (!defined(DIFFMAP)) && (!defined(ALPHAMAP))
        gl_FragColor = diffColor;
    #endif
    #ifdef DIFFMAP
        vec4 diffInput = texture2D(sDiffMap, vTexCoord);
        #ifdef ALPHAMASK
            if (diffInput.a < 0.5)
                discard;
        #endif
        gl_FragColor = diffColor * diffInput;
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
