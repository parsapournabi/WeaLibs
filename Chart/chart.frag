#version 460 core

in vec4 vColor;
flat in int vShape; // 0 == Square, 1 == Circle
out vec4 fragColor;

void main(void)
{
    //    vec2 pos = mod(gl_FragCoord.xy, vec2(50.0)) - vec2(25.0);
    //    float dist_squared = dot(pos, pos);
    //    fragColor = (dist_squared < 400.0)
    //            ? vec4(.90, .90, .90, 1.0)
    //            : vec4(.20, .20, .40, 1.0);

    //    float distSq = dot(gl_PointCoord.xy - vec2(0.5), gl_PointCoord - vec2(0.5));
    //    if (distSq > 0.25)
    //        discard;

    if (vShape == 1) {
        // Converting point from square to circle
        vec2 coord = gl_PointCoord * 2.0 - 1.0;
        if (dot(coord, coord) > 1.0)
            discard;
    }

    fragColor = vColor;
}
