#version 430

in vec4 colour;
in vec2 fUV;

in vec3 fNormal;

out vec4 FragColor;


uniform float ourColour;

uniform sampler2D texture_00;
uniform sampler2D texture_01;


void main()
{
	FragColor = vec4(fNormal, 1);
};