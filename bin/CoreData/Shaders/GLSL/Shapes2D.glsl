// rectFill tests relative to origin and expects inversely transformed coordinates
// i.e. test interval range is unaffected by fragPos
float rectFill(vec2 fragPos, vec2 size, vec2 anchor) {
    fragPos = step(size * -anchor, fragPos) - step(size * (1 - anchor), fragPos);
    return fragPos.x * fragPos.y;
}

float rectFillSmooth(vec2 fragPos, vec2 size, vec2 anchor, vec2 antiAlias) {
    vec2 as = anchor*size;
    fragPos = smoothstep(-as, -as+antiAlias, fragPos)
        - smoothstep(size-as - antiAlias, size-as, fragPos);
    return fragPos.x * fragPos.y;
}

float rectStroke(vec2 fragPos, vec2 size, vec2 anchor, float lineWidth) {
    // (size - lineWidth * 2) is inner rect size
    return rectFill(fragPos, size, anchor)
        - rectFill(fragPos + size*anchor - lineWidth, size - lineWidth * 2, vec2(0));
}

float rectStrokeSmooth(vec2 fragPos, vec2 size, vec2 anchor, float lineWidth, vec2 antiAlias) {
    return rectFillSmooth(fragPos, size, anchor, antiAlias)
        - rectFillSmooth(fragPos + size*anchor - lineWidth, size - lineWidth * 2, vec2(0), antiAlias);
}

float circleFill(vec2 fragPos, float radius, vec2 anchor) {
    return 1.0 - step(radius, length(fragPos + radius * anchor));
}

float circleFillSmooth(vec2 fragPos, float radius, vec2 anchor, float antiAlias) {
    return 1.0 - smoothstep(radius - antiAlias, radius, length(fragPos + radius * anchor));
}

float circleStroke(vec2 fragPos, float radius, vec2 anchor, float lineWidth) {
    return circleFill(fragPos, radius, anchor)
        - circleFill(fragPos + radius*anchor, radius - lineWidth, vec2(0));
}

float circleStrokeSmooth(vec2 fragPos, float radius, vec2 anchor, float lineWidth, float antiAlias) {
    return circleFillSmooth(fragPos, radius, anchor, antiAlias)
        - circleFillSmooth(fragPos + radius*anchor, radius - lineWidth, vec2(0), antiAlias);
}
