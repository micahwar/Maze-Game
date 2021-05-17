#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

layout (location = 0) out vec4 v_Color;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out float v_TexIndex;

uniform mat4 u_MVP; // model view projection matrix

void main()
{
    gl_Position = u_MVP * vec4(a_Position, 1.0f);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}

#shader fragment
#version 450 core

out vec4 color;

layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in float v_TexIndex;
layout (binding = 0) uniform sampler2D u_Textures[32];
void main()
{
    vec4 texColor = texture(u_Textures[0], v_TexCoord) * v_Color;
    color = texColor;
}