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
