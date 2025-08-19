#version 460 core
layout (location = 0) in vec2 positions;
layout (location = 1) in vec4 colorRgb;
//layout (location = 2) in float alpha;

out vec4 vColor;
out int vShape; // 0 == Square, 1 == Circle

uniform mat4 mvp;
uniform vec4 u_selectedRanges[32];

void main(void)
{
    vec4 selectColor = vec4(1.0, 1.0, 0.0, 1.0);
    gl_PointSize = 5.0f;
    gl_Position = mvp * vec4(positions.xy, 0.0, 1.0);
    for (int i = 0; i < 64; ++i) {
        if (positions.y <= u_selectedRanges[i].x &&
                positions.x >= u_selectedRanges[i].y &&
                positions.y >= u_selectedRanges[i].z &&
                positions.x <= u_selectedRanges[i].w) {
            vColor = selectColor;
            return;
        }

    }
//    vColor = vec4(colorRgb, alpha);
    vShape = 1;
    vColor = colorRgb;
}
