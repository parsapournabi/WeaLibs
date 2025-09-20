#version 460 core

in vec4 vColor;
flat in int vShape; // Shape: 0 == Square, 1 == Circle, 2 == Texture

out vec4 fragColor;

uniform sampler2D u_texture;

void main(void) {
    switch (vShape)
    {
    case 1:
        // Converting from square to circle
        vec2 coord = gl_PointCoord * 2.0 - 1.0;
        if (dot(coord, coord) > 1.0)
            discard;
        break;
    case 2:
        vec2 uv = gl_PointCoord;
        vec4 texColor = texture(u_texture, uv);
        if (texColor.a < 0.1)
            discard;
        fragColor = texColor;
        return;
    }

    fragColor = vColor;
}

