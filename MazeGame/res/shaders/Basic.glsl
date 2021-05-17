#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP; // model view projection matrix

void main()
{
    gl_Position = u_MVP * position;
}

#shader fragment
#version 330 core

out vec4 color;
uniform vec4 u_Color;

void main()
{
    color = u_Color;
}