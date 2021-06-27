vec2 tile2D(vec2 fragPos, vec2 repeat) {
    // Scale up the coordinate sys by repeat and wrap arround 1.0 with fract; preserve the
    // floating part of the coordinate in order to draw things inside each cell
    return fract(fragPos * repeat);
}

vec2 bricks(vec2 fragPos, vec2 zoom, vec2 offset, vec2 every) {
    fragPos *= zoom;
    // brick offset
    fragPos.x += ( 1 - step(1.0, mod(fragPos.y, every.x)) ) * offset.x;
    fragPos.y += ( 1 - step(1.0, mod(fragPos.x, every.y)) ) * offset.y;
    return fract(fragPos);
}

float random(vec2 fragPos) {
    return fract(sin(dot(fragPos.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float randomInt(vec2 fragPos, float upper) {
    // floor() isolates a common int for a region of pixels
    // use common int to obtain a random value for that area
    return floor(random(floor(fragPos * upper)) * upper);
}

// Subdivides the space into four cells and assigns an angle of rotation to each one
vec2 rotateTilePattern(vec2 st) {
    st *= 2.;
    
    // idx val
    // 2 | 3
    //-------
    // 0 | 1
    float idx = 0.;
    idx += step(1., st.x);
    idx += step(1., st.y) * 2.;

    st = fract(st);

    // reflect vertically and/or horizontally; eq. to +/-(PI/2) or PI rotation
    if (idx == 1.0) {
        st = vec2(st.x, 1. - st.y);
    } else if  (idx == 2.0) {
        st = vec2(1. - st.x, st.y);
    } else if (idx == 3.0) {
        st = 1. - st;
    }

    return st;
}

// Subdivides the space into four cells and assigns an angle of rotation to each one
vec2 truchetPattern(vec2 st, vec2 cell) {
    float idx = floor(random(cell) * 4);
    
    st = fract(st);
    if (idx == 1.0) {
        st = vec2(st.x, 1. - st.y);
    } else if  (idx == 2.0) {
        st = vec2(1. - st.x, st.y);
    } else if (idx == 3.0) {
        st = 1. - st;
    }

    return st;
}

