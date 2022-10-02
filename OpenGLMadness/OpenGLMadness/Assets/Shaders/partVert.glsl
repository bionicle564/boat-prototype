#version 430

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

out vec4 colour;
out vec2 fUV;

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;

uniform bool bBillboard;
uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform vec3 centerPos;

layout(std430) buffer OffsetsBlock
{
	vec4 offsets[];
};


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
		vPos_world = offsets[gl_InstanceID].xyz + cameraRight * vPos.x
					+ cameraUp * vPos.y;
		
	}
	else
	{
		vPos_world = vPos + offsets[gl_InstanceID].xyz;
	}

    //gl_Position = projection * view * model * vec4(vPos, 1.0f);
    gl_Position = matrices.proj * matrices.viw * model * vec4(vPos_world, 1.0f);
	//gl_Position.xy = vPos.xy;
	colour = offsets[gl_InstanceID];
    fUV = vUV;
};