vec3 toGreyscale(vec3 color)
{
    return vec3(0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b);
}