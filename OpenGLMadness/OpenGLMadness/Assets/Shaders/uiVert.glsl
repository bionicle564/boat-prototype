#version 430

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

out vec4 colour;
out vec2 fUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float screenWidth;
uniform float screenHeight;

uniform float width;
uniform float height;

uniform vec3 position;

layout (std140) uniform MatrixBlock
{
	mat4 proj;
	mat4 viw;
} matrices;

void main()
{
   //gl_Position = projection * view * model * vec4(vPos, 1.0f);
   float x = ((vPos.x * width + position.x) / screenWidth) * 2;
   x = x - 1;
   
   float y = ((vPos.y * height + position.y) / screenHeight) * 2;
   y = y + 1;
   
   gl_Position = vec4(x,y,0,1);
   //gl_Position = vec4(vPos, 1.0f);
   
   fUV = vUV;
};