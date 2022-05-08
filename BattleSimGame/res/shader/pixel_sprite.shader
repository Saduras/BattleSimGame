// Based on https://colececil.io/blog/2017/scaling-pixel-art-without-destroying-it/
// Choose between nearest neighbor and bilinear filter based on u_TexelPerPixel

#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_PV;
uniform mat4 u_Model;
uniform vec2 u_TextureSize;

void main()
{
	gl_Position = u_PV * u_Model * vec4(a_Position, 1.0);
	v_TexCoord = a_TexCoord * u_TextureSize;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;
uniform vec2 u_TextureSize;
uniform float u_TexelPerPixel;

void main()
{
	vec2 locationWithinTexel = fract(v_TexCoord);
	vec2 interpolationAmount = clamp(locationWithinTexel / u_TexelPerPixel, 0, 0.5)
		+ clamp((locationWithinTexel + 1) / u_TexelPerPixel + 0.5, 0, 0.5);
	vec2 finalTextureCoords = (floor(v_TexCoord) + interpolationAmount) / u_TextureSize;

	vec4 texColor = texture(u_Texture, finalTextureCoords);
	color = texColor * u_Color;
};