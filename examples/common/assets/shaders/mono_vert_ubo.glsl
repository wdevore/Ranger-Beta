#version 450 core

// This input attribute handles the stream of vertices sent to this shader
// The client defines the structure layout using vertex-attribute-pointer
layout (location = 0) in vec3 aPos;

// Common uniforms between shaders. UBO is named "Matrices
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

// The transforms applied to vertices, for example, translate and or scale.
uniform mat4 model;

void main()
{
    // note: that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}