#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;

uniform mat4 u_PV;

void main()
{
	v_Position = a_Position;
	gl_Position = u_PV * vec4(a_Position, 1.0);;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;

void main()
{
	color = vec4(0.2, 0.3, 0.8, 1.0);
}