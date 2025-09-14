#version 460 core

struct SeriesProps {
    int startIndex, endIndex;
    int markerShape; // Shape: 0 == Square, 1 == Circle, 2 == Texture
    int markerSize;
    int colorType; // ColorType: 0 == MixBasePoint, 1 == OnlyBase, 2 == Only Point
    int type; // Type: 0 == Points, 1 == Line, 2 == Area
    vec4 color;
};

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(std430, binding = 2) restrict readonly buffer SeriesPropsBuffer {
    SeriesProps seriesProps[];
};

out vec4 vColor;
flat out int vShape;

// Constants
const int MAX_SELECTED_RANGES = 32;
const vec4 SELECT_COLOR = vec4(1.0, 1.0, 0.0, 1.0);
const vec4 OUT_OF_RANGE = vec4(-200.0, -200.0, -200.0, -200.0);
const float OFFSET_SELECT = 0.12;

uniform mat4 mvp;
uniform vec4 u_selectedRanges[MAX_SELECTED_RANGES]; // x == top, y == left, z == bottom, w == right

void main(void) {
    vShape = 0;
    // Check which type
    for (int i = 0; i < seriesProps.length(); ++i) {
        if (gl_VertexID >= seriesProps[i].startIndex && gl_VertexID < seriesProps[i].endIndex) {
            if (seriesProps[i].type == 0) {
                gl_PointSize = seriesProps[i].markerSize;
                vShape = seriesProps[i].markerShape;

            }

            switch (seriesProps[i].colorType)
            {
            case 0: vColor = mix(seriesProps[i].color, color, vec4(0.5)); break;
            case 1: vColor = seriesProps[i].color; break;
            case 2: vColor = color; break;
            }
            break;
        }
    }
    // Check if its valid
    if (vColor == vec4(0)) {
        gl_Position = mvp * OUT_OF_RANGE;
        //        vShape = 1;
        return;
    }

    gl_Position = mvp * vec4(position.xy, 0.0, 1.0);
    for (int i = 0; i < MAX_SELECTED_RANGES; ++i) {
        if (position.y <= u_selectedRanges[i].x + OFFSET_SELECT &&
                position.x >= u_selectedRanges[i].y - OFFSET_SELECT &&
                position.y >= u_selectedRanges[i].z - OFFSET_SELECT &&
                position.x <= u_selectedRanges[i].w + OFFSET_SELECT) {
            vColor = SELECT_COLOR;
            return;
        }
    }
}
