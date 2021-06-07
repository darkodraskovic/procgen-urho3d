vec2 tile2D(vec2 fragPos, vec2 repeat) {
    // Scale up the coordinate sys by repeat and wrap arround 1.0 with fract
    return fract(fragPos * repeat);
}

vec2 bricks(vec2 fragPos, vec2 zoom, vec2 offset, vec2 every) {
    fragPos *= zoom;
    // brick offset
    fragPos.x += ( 1 - step(1.0, mod(fragPos.y, every.x)) ) * offset.x;
    fragPos.y += ( 1 - step(1.0, mod(fragPos.x, every.y)) ) * offset.y;
    return fract(fragPos);
}
