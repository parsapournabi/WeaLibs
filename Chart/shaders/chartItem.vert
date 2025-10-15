#version 460 core

layout(location = 0) in vec2 position;

uniform mat4 mvp;
uniform float u_z;

out vec2 v_texCoord;

void main(void)
{
    gl_Position = mvp * vec4(position.xy, 0.0, 1.0);
    gl_Position.z -= u_z;

    v_texCoord = gl_Position.xy;
}
