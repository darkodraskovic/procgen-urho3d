// rectFill tests relative to origin (=0) and expects inversely transformed normalized screen coordinates
// origin is the lower left corner of the rect
// NB: test interval (range) is unaffected by frag pos
// test interval is offseted -(size * anchor) relative to origin (=0) => rect is offseted to the left
float rectFill(vec2 fragPos, vec2 size, vec2 anchor) {
    fragPos = step(-(size * anchor), fragPos) - step(size * (1 - anchor), fragPos);
    return fragPos.x * fragPos.y;
}

float rectFillSmooth(vec2 fragPos, vec2 size, vec2 anchor, vec2 antiAlias) {
    vec2 as = anchor*size;
    fragPos = smoothstep(-as, -as+antiAlias, fragPos)
        - smoothstep(size-as - antiAlias, size-as, fragPos);
    return fragPos.x * fragPos.y;
}

float rectStroke(vec2 fragPos, vec2 size, vec2 anchor, float lineWidth) {
    // since anchor is vec2(0), we translated test interval by (size*anchor - lineWidth)
    // therefore, we have to translate frag pos also
    // (size - lineWidth * 2) is inner rect size
    return rectFill(fragPos, size, anchor)
        - rectFill(fragPos + (size*anchor - lineWidth), size - lineWidth*2, vec2(0));
}

float rectStrokeSmooth(vec2 fragPos, vec2 size, vec2 anchor, float lineWidth, vec2 antiAlias) {
    return rectFillSmooth(fragPos, size, anchor, antiAlias)
        - rectFillSmooth(fragPos + (size*anchor - lineWidth), size - lineWidth*2, vec2(0), antiAlias);
}

// circleFill tests relative to origin (=0) and expects inversely transformed normalized screen coordinates
// frag pos is offseted +(radius*anchor) relative to origin (=0) => circle is offseted to the left
// origin is the center of the circle
float circleFill(vec2 fragPos, float radius, vec2 anchor) {
    return 1.0 - step(radius, length(fragPos + radius*anchor));
}

float circleFillSmooth(vec2 fragPos, float radius, vec2 anchor, float antiAlias) {
    return 1.0 - smoothstep(radius - antiAlias, radius, length(fragPos + radius*anchor));
}

float circleStroke(vec2 fragPos, float radius, vec2 anchor, float lineWidth) {
    // since anchor is vec2(0), we translated test interval by +(radius*anchor)
    // therefore, we have to translate frag position also by +(radius*anchor)
    // (radius - lineWidth) is inner circle radius
    return circleFill(fragPos, radius, anchor)
        - circleFill(fragPos + radius*anchor, radius - lineWidth, vec2(0));
}

float circleStrokeSmooth(vec2 fragPos, float radius, vec2 anchor, float lineWidth, float antiAlias) {
    return circleFillSmooth(fragPos, radius, anchor, antiAlias)
        - circleFillSmooth(fragPos + radius*anchor, radius - lineWidth, vec2(0), antiAlias);
}
