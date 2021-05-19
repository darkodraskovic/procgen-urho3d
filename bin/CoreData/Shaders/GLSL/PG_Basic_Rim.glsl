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

uniform vec4 cRimColor = vec4(0, .5, .5, 0);
uniform float cRimPower = 3;

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);

    vWorldPos = worldPos;
    vAttribNormal = iNormal;
    vWorldNormal = GetWorldNormal(modelMatrix);
    vViewDir = normalize(cCameraPos - worldPos);

    #ifdef DIFFMAP
        vTexCoord = iTexCoord;
    #endif
    #ifdef VERTEXCOLOR
        vColor = iColor;
    #endif
}

void PS()
{
    vec4 diffColor = cMatDiffColor;

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
        // gl_FragColor = diffColor * diffInput;
            
            // gl_FragColor = vec4(vAttribNormal,1);
            // gl_FragColor = vec4(vWorldNormal,1);

            // gl_FragColor = vec4(dot(vWorldNormal, vViewDir), 1, 1, 1);
            // gl_FragColor = vec4(1, dot(vWorldNormal, vViewDir), 1, 1);
            // gl_FragColor = vec4(1, 1, dot(vWorldNormal, vViewDir), 1);

            float rim = 1 - clamp(dot(vViewDir,vWorldNormal), 0, 1);
            // vec3 rimComponent = cRimColor.rgb * pow(rim, cRimPower);
            // vec3 rimComponent = cRimColor.rgb * (rim > .8 ? rim : 0);
            // vec3 rimComponent = cRimColor.rgb * (rim > .8 ? 1 : 0);
            // vec3 rimComponent = rim > .8 ? vec3(1,0,0) : vec3(0);
            vec3 rimComponent = rim > .8 ? vec3(1,0,0) : rim > .7 ? vec3(0,1,0) : rim > .6 ? vec3(0,0,1) : vec3(0);
            // gl_FragColor = vec4(rimComponent,1);

            vec3 lightDir;
            float diff = GetDiffuse(vWorldNormal, vWorldPos.xyz, lightDir);
            vec3 diffComponent = vec3(diff);
            // gl_FragColor = vec4(diffComponent, 1);

            // vec3 posComponent = vWorldPos.y > .25 ? vec3(1,0,0) : vec3(0,1,0);
            vec3 posComponent = fract(vWorldPos.y * 10) > .3 ? vec3(.2,0,.3) : vec3(0,.3,0);
            // gl_FragColor = vec4(posComponent, 1);
            
            // gl_FragColor = vec4(diffComponent + rimComponent + posComponent,1);
            gl_FragColor = vec4(rimComponent + posComponent,1);
            
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
