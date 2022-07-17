#shader vertex
#version 330 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec3 a_Color;

out vec4 v_Color;

uniform mat4 u_PV;

void main()
{
	gl_Position = u_PV * vec4(a_Position, 1.0, 1.0);
	v_Color = vec4(a_Color, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_Color;

void main()
{
	color = v_Color;
}