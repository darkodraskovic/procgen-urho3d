float rectFill(vec2 fragPos, vec2 size, vec2 anchor = vec2(0)) {
    fragPos = step(-anchor*size, fragPos)
        - step(size -anchor*size, fragPos); // reflect around origin
    return fragPos.x * fragPos.y;
}

// float rectFillSmooth(vec2 fragPos, vec2 size, vec2 anchor = vec2(0), vec2 antiAlias = vec2(0)) {
//     vec2 as = anchor*size;
//     fragPos = smoothstep(-as, -as + antiAlias, fragPos)
//         * smoothstep(size-as, size-as + antiAlias, -fragPos);
//     return fragPos.x * fragPos.y;
// }

float rectStroke(vec2 fragPos, vec2 size, float lineWidth, vec2 anchor = vec2(0)) {
    vec2 innerSize = size - lineWidth * 2;
    // fragPos - lineWidth # inverse transform
    // anchor * (size / innerSize) # inner is smaller than outer, so it's anchor has to be bigger
    return rectFill(fragPos, size, anchor)
        - rectFill(fragPos - lineWidth, innerSize, anchor * (size / innerSize));
}

float circleFill(vec2 fragPos, float radius, vec2 anchor = vec2(0)) {
    return 1.0 - step(radius, length(fragPos - radius * anchor));
}

float circleStroke(vec2 fragPos, float radius, float lineWidth, vec2 anchor = vec2(0)) {
    float innerRadius = radius - lineWidth;
    // anchor * (radius / innerRadius) # inner is smaller than outer, so it's anchor has to be bigger
    return circleFill(fragPos, radius, anchor)
        - circleFill(fragPos, innerRadius, anchor * (radius / innerRadius));
}
