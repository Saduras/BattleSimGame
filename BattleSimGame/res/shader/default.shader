#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_PV;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_PV * u_Model * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}