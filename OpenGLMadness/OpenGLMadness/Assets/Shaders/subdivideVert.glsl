#version 430

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

out vec4 colour;


out VS_OUT {
    mat4 MVP;
	mat4 model;
	vec2 gUV;
} vs_out;

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

uniform bool bBillboard;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 centerPos;



layout (std140) uniform MatrixBlock
{
	mat4 proj;
	mat4 viw;
} matrices;

void main()
{
	vec3 vPos_world = vPos;
	if(bBillboard)
	{
		vPos_world = centerPos + cameraRight * vPos.x
					+ cameraUp * vPos.y;
		
	}
	vs_out.model = inverse(transpose(model)) ;
	vs_out.MVP = matrices.proj * matrices.viw * model;
   //gl_Position = projection * view * model * vec4(vPos, 1.0f);
   //gl_Position = matrices.proj * matrices.viw * model * vec4(vPos_world, 1.0f);
   gl_Position = vec4(vPos_world, 1.0f);
	
   
   vs_out.gUV = vUV;
};