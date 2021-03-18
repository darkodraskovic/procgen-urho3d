#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Constants.glsl"

#include "Transform2D.glsl"
#include "Shapes2D.glsl"
#include "Utilities2D.glsl"

varying vec2 vScreenPos;

// tic-tac-toe field: 0 = O, 1 = x
int ttt[] = int[](0, 1, 1, 1, 0, 0, 1, 0, 1);

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

void PS()
{
    vec2 st = vec2(vScreenPos.x, 1 - vScreenPos.y);

    float sx = 3; float sy = 3;
    
    int x = int(st.x * sx);
    int y = int(st.y * sy);
    int shape = ttt[y * 3 + x];
    
    st = tile2D(st, vec2(sx,sy));
    
    vec3 tnl = translate2D(.5, .5) * vec3(st, 1);
    
    float col;
    if (shape == 1) {
        tnl = rotate2D(M_PI/4 * cElapsedTimePS) * tnl;
        col = rectFill(tnl.xy, vec2(.4,.05), vec2(.5));
        tnl = rotate2D(-M_PI/2 * cElapsedTimePS) * tnl;
        col += rectFill(tnl.xy, vec2(.4,.05), vec2(.5));
    } else {
        col = circleStroke(tnl.xy, .2, .05);
    }
    
    gl_FragColor = vec4(st, col, 1.0);
}
