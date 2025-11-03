#version 330 core

out vec4 FragColor;

uniform vec4 fragColor;

void main()
{
    float dist = length(gl_PointCoord - vec2(0.5));
    float alpha = 1.0 - smoothstep(0.4, 0.5, dist);
    FragColor = vec4(fragColor.rgb, fragColor.a * alpha);
}