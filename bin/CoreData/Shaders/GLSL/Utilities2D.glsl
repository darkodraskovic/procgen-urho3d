vec2 tile2D(vec2 st, vec2 repeat) {
    // Scale up the space by repeat and wrap arround 1.0 with fract
    return fract(st * repeat);
}
