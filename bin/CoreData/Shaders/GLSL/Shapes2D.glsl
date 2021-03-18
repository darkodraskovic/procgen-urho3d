float rectFill(vec2 fragPos, vec2 size, vec2 anchor = vec2(0)) {
    fragPos = step(-anchor*size, fragPos)
        * step(-anchor*size, -fragPos); // reflect around origin
    return fragPos.x * fragPos.y;
}

float rectFillSmooth(vec2 fragPos, vec2 size, vec2 anchor = vec2(0), vec2 antiAlias = vec2(0)) {
    vec2 as = anchor*size;
    fragPos = smoothstep(-as, -as + antiAlias, fragPos)
        * smoothstep(-as, -as + antiAlias, -fragPos);
    return fragPos.x * fragPos.y;
}

float rectStroke(vec2 fragPos, vec2 size, float lineWidth, vec2 anchor = vec2(0)) {
    return rectFill(fragPos, size, anchor)
        - rectFill(fragPos, size - lineWidth, anchor);
}

float circleFill(vec2 fragPos, float radius, vec2 anchor = vec2(0)) {
    return 1.0 - step(radius, length(fragPos - radius * anchor));
}

float circleStroke(vec2 fragPos, float radius, float lineWidth, vec2 anchor = vec2(0)) {
    return circleFill(fragPos, radius, anchor)
        - circleFill(fragPos, radius - lineWidth, anchor);
}
