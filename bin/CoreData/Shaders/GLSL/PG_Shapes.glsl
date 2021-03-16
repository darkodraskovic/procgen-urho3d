#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "Transform2D.glsl"
#include "Shapes2D.glsl"

#if defined(DIFFMAP) || defined(ALPHAMAP)
    varying vec2 vTexCoord;
#endif
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif

// PP
varying vec4 vWorldPos;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);

    // PP
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));
    
    #ifdef DIFFMAP
        vTexCoord = iTexCoord;
    #endif
    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif
}

void PS()
{
    // vec4 diffColor = cMatDiffColor;

    // PP
    vec3 pos = vec3(vWorldPos.xy + .5, 1);
    
    // vec4 diffColor = vec4(pos.xy, 0, 1);

    pos = rotate2D(-cElapsedTimePS) * translate2D(.5, .5) * pos;
    float rect = rectFill(pos.xy, vec2(.3, .3), vec2(.5));
    vec4 diffColor = vec4(rect, rect, 0, 1);

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
