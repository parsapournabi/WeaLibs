#version 460 core

/** Definitions Reference to GLEnums.h/GLStyleLine enumeration **/
#define NO_STYLE 0
#define SOLID_LINE 1
#define DASH_LINE 2
#define DOT_LINE 3
#define DASH_DOT_LINE 4
#define DASH_DOT_DOT_LINE 5

// Inputs
in vec2 v_texCoord;

// Outputs
out vec4 fragColor;

// Uniforms
uniform vec4 u_color;
uniform vec4 u_gradient;
uniform bool u_useImage;
uniform bool u_useGradient;
uniform bool u_isVertical;
uniform int u_lineStyle;
uniform sampler2D u_texture;

// Constants
const int LINE_GAP = 4; // Pixel
const int DASH_LEN = 6; // Pixel
const int DOT_LEN = 1; // Pixel

// These methods returns true if the coordinate should discard
bool styleNoStyle(vec2 coord) {
    return false;

}
bool styleSolidLine(vec2 coord) {
    return false;
}

bool styleDashLine(vec2 coord) {
    const float factor = 20.0;
    const float diff = 10.0;
    if (u_isVertical)
        return (mod(coord.y, factor) > diff);
    else
        return (mod(coord.x, factor) > diff);

}
bool styleDotLine(vec2 coord) {
    const float factor = 8.0;
    const float diff = 1.5;
    if (u_isVertical)
        return (mod(coord.y, factor) > diff);
    else
        return (mod(coord.x, factor) > diff);


}
bool styleDashDotLine(vec2 coord) {
    float factor = 30.0;
    float pattern = u_isVertical ? mod(coord.y, factor) : mod(coord.x, factor);
    float diff1 = 12.0;
    float diff2 = 15.0;
    float diff3 = 17.0;
    return !((pattern < diff1) ||
             (pattern > diff2 && pattern < diff3));

}
bool styleDashDotDotLine(vec2 coord) {
    float factor = 40.0;
    float pattern = u_isVertical ? mod(coord.y, factor) : mod(coord.x, factor);
    float diff1 = 12.0;
    float diff2 = 15.0;
    float diff3 = 17.0;
    float diff4 = 20.0;
    float diff5 = 22.0;
    return !((pattern < diff1) ||
             (pattern > diff2 && pattern < diff3) ||
             (pattern > diff4 && pattern < diff5));
}

void main(void)
{
    switch(u_lineStyle) {
    case NO_STYLE:
        if (styleNoStyle(gl_FragCoord.xy)) discard;
        break;
    case SOLID_LINE:
        if (styleSolidLine(gl_FragCoord.xy)) discard;
        break;
    case DASH_LINE:
        if (styleDashLine(gl_FragCoord.xy)) discard;
        break;
    case DOT_LINE:
        if (styleDotLine(gl_FragCoord.xy)) discard;
        break;
    case DASH_DOT_LINE:
        if (styleDashDotLine(gl_FragCoord.xy)) discard;
        break;
    case DASH_DOT_DOT_LINE:
        if (styleDashDotDotLine(gl_FragCoord.xy)) discard;
        break;
    default:
        break;

    };
    if (u_useImage) {
        fragColor = texture(u_texture, gl_FragCoord.xy);
    }
    else if (u_useGradient) {
        fragColor = u_gradient;
    }
    else {
        fragColor = u_color;
    }

}
