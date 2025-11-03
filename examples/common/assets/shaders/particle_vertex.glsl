#version 450 core

layout (location = 0) in vec3 aPos;

// Common uniforms between shaders. UBO is named "Matrices
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform float pointSize;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = pointSize;
}