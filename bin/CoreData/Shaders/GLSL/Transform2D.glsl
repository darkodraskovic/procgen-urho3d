// These are inverse transform matrices. Each matrix is right multiplied by a fragment position.
// The fragment is transformed to the hypothetical non transformed coordinate system.  The
// transformed fragment can be tested against various conditions which assume non transformed
// fragment position, such as shape functions (see Shapes2D.glsl) and similar.

// Since these are inverse transform matrices, the correct order of transformation is:
// translatation, rotation, scale. E.g. scale2D(2, 2) * rotate2D(-.5) * translate2D(.4, .2) *
// vec3(x, y, 1);

mat3 translate2D(float x, float y) {
    return mat3(1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                -x, -y, 1.0);
}

mat3 rotate2D(float angle) {
    return mat3(cos(angle), -sin(angle), 0.0,
                sin(angle), cos(angle), 0.0,
                0.0, 0.0, 1.0);
}

mat3 scale2D(float x, float y){
    return mat3(1.0/x, 0.0, 0.0,
                0.0, 1.0/y, 0.0,
                0.0, 0.0, 1.0);
}

mat2 mat2Rotate2D(float angle) {
    return mat2(cos(angle), -sin(angle),
                sin(angle), cos(angle));
}

mat2 mat2Scale2(float x, float y){
    return mat2(1.0/x, 0.0,
                0.0, 1.0/y);
}
