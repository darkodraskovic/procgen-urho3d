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
    vec3 pos = vec3(vScreenPos.x, 1 - vScreenPos.y, 1);

    vec3 pos0 = translate2D(.2, .2) * pos;
    pos0 = rotate2D(-cElapsedTimePS) * pos0;
    
    vec3 pos1 = translate2D(.4, .2) * pos;
    pos1 = rotate2D(-cElapsedTimePS) * pos1;
    
    vec3 pos2 = translate2D(.6, .2) * pos;
    pos2 = rotate2D(-cElapsedTimePS) * pos2;
    float scl = abs(sin(cElapsedTimePS * .5));
    pos2 = scale2D(scl + .5, scl + .5) * pos2;
    
    vec3 pos3 = translate2D(.8, .2) * pos;
    pos3 = rotate2D(-cElapsedTimePS) * pos3;

    vec3 pos4 = translate2D(.2, .6) * pos;
    pos4 = rotate2D(-cElapsedTimePS) * pos4;
    
    vec3 pos5 = translate2D(.4, .8) * pos;
    
    vec3 pos6 = translate2D(.6, .6) * pos;
    pos6 = rotate2D(-cElapsedTimePS) * pos6;
    
    vec3 pos7 = translate2D(.8, .8) * pos;
    pos7 = rotate2D(cElapsedTimePS) * pos7;
    
    float intensity0 = 0;
    float intensity1 = 0;
    
    intensity0 += rectFill(pos0.xy, vec2(.1, .15), vec2(.5));
    intensity1 += rectStroke(pos1.xy, vec2(.1, .15), vec2(.3), .02);
    intensity0 += rectFillSmooth(pos2.xy, vec2(.15, .15), vec2(.3), vec2(.03));
    intensity1 += rectStrokeSmooth(pos3.xy, vec2(.15, .15), vec2(.6), .03, vec2(.03));

    intensity1 += circleFill(pos4.xy, .1, vec2(.3));
    intensity0 += circleFillSmooth(pos5.xy, .1, vec2(.5), .02);
    intensity1 += circleStroke(pos6.xy, .2, vec2(.5), .03);
    intensity0 += circleStrokeSmooth(pos7.xy, .2, vec2(.2), .08, .02);
    
    vec4 diffColor = vec4(intensity0, intensity1, (intensity0 + intensity1) * abs(sin(cElapsedTimePS)), 1);
    
    gl_FragColor = diffColor;
}
