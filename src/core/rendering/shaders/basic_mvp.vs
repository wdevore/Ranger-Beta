#version 400 core
layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 color;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
//    ourColor = vec3(1.0f, 0.5f, 0.2f);
}
