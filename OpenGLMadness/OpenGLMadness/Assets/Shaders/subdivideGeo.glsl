#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices=146) out; 

uniform int sub_divisions;
uniform float time;

in VS_OUT {
    mat4 MVP;
	mat4 model;
	vec2 gUV;
} gs_in[]; 

out vec3 fNormal;

vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

//
// Description : GLSL 2D simplex noise function
//      Author : Ian McEwan, Ashima Arts
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License :
//  Copyright (C) 2011 Ashima Arts. All rights reserved.
//  Distributed under the MIT License. See LICENSE file.
//  https://github.com/ashima/webgl-noise
//
float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
                        // 1.0 / 41.0

    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
            permute( i.y + vec3(0.0, i1.y, 1.0))
                + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(
                        dot(x0,x0),
                        dot(x1,x1),
                        dot(x2,x2)
                        ), 0.0);

    m = m*m ;
    m = m*m ;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}

vec4 Waves(vec4 pointIn)
{
	vec4 finalPoint;
	
	int M = 1;
	float deapthSolved = 1; //should use tanh
	float g = 1.81;
	float Kxm = 12;
	float Kzm = 0;
	float Am = .04;
	float phasem = snoise(pointIn.xy + time * .5);
	float Km = sqrt((Kxm * Kxm) + (Kzm * Kzm));
	float Wm2 = g * Km * deapthSolved;
	
	float theatam = (Kxm * pointIn.x) + (Kzm * pointIn.y) - (sqrt(Wm2) * (time * .5)) - phasem;
	
	int N = 2;
	float Kxn = 0;
	float Kzn = 0;
	float An = 0.04;
	float phasen = 0;
	float Kn = sqrt((Kxn * Kxn) + (Kzn * Kzn));
	float Wn2 = g * Kn * deapthSolved;
	
	float theatan = (Kxn * pointIn.x) + (Kzn * pointIn.y) - (sqrt(Wn2) * (time * .1)) - phasen;

	for(int m = 1; m <= M;m++)
	{
		finalPoint.x = pointIn.x - ((Kxm / Km) * (Am / deapthSolved) * sin(theatam)) - ((Kxn / Kn) * (An / deapthSolved) * sin(theatan));
		finalPoint.y = pointIn.y - ((Kzm / Km) * (Am / deapthSolved) * sin(theatam))- ((Kzn / Kn) * (An / deapthSolved) * sin(theatan));
		finalPoint.z = Am * cos(theatam) + An * cos(theatan);
		
	}
	
	finalPoint.w = 1;
	return finalPoint;
}


void main() {
  vec4 v0 = gl_in[0].gl_Position;
  vec4 v1 = gl_in[1].gl_Position;
  vec4 v2 = gl_in[2].gl_Position;
  float dx = abs(v0.x-v1.x)/sub_divisions;
  float dy = abs(v0.y-v2.y)/sub_divisions;
  float x=v0.x;
  float y=v0.y;
  fNormal = vec3(0,0,1);
  
  for(int j=0;j<sub_divisions*sub_divisions;j++)  {
  
	
    //gl_Position =  gs_in[0].MVP * vec4(x,y,0,1);
	
	vec4 pos = Waves(vec4(x,y,0,1));
	vec4 pos2 = Waves(vec4(x + dx,y,0,1));
	
	pos2 = pos2 - pos;
	pos2 /= dx;
	
	vec4 pos3 = Waves(vec4(x,y + dy,0,1));
	pos3 = pos3 - pos;
	pos3 /= dy;
	
	fNormal = cross(pos2.xyz, pos3.xyz);
	
	fNormal =  (gs_in[0].model * vec4(fNormal, 1)).xyz;
	
    gl_Position =  gs_in[0].MVP * pos;
    EmitVertex();
	
    //gl_Position =  gs_in[0].MVP * vec4(x,y+dy,0,1);
	pos = Waves(vec4(x,y+dy,0,1));
	pos2 = Waves(vec4(x + dx,y + dy,0,1));
	
	pos2 = pos2 - pos;
	pos2 /= dx;
	
	pos3 = Waves(vec4(x,y + dy + dy,0,1));
	pos3 = pos3 - pos;
	pos3 /= dy;
	
	fNormal = cross(pos2.xyz, pos3.xyz);
	
	fNormal =  (gs_in[0].model * vec4(fNormal, 1)).xyz;
    gl_Position =  gs_in[0].MVP * pos;
    EmitVertex();
	
    //gl_Position =  gs_in[0].MVP * vec4(x+dx,y,0,1);
    pos = Waves(vec4(x+dx,y,0,1));
	pos2 = Waves(vec4(x + dx + dx,y,0,1));
	
	pos2 = pos2 - pos;
	pos2 /= dx;
	
	pos3 = Waves(vec4(x+dx,y + dy,0,1));
	pos3 = pos3 - pos;
	pos3 /= dy;
	
	fNormal = cross(pos2.xyz, pos3.xyz);
	
	fNormal =  (gs_in[0].model * vec4(fNormal, 1)).xyz;
    gl_Position =  gs_in[0].MVP * pos;
    EmitVertex();
	
    //gl_Position =  gs_in[0].MVP * vec4(x+dx,y+dy,0,1);
        pos = Waves(vec4(x+dx,y+dx,0,1));
	pos2 = Waves(vec4(x + dx + dx,y+dy,0,1));
	
	pos2 = pos2 - pos;
	pos2 /= dx;
	
	pos3 = Waves(vec4(x+dx,y + dy + dy,0,1));
	pos3 = pos3 - pos;
	pos3 /= dy;
	
	fNormal = cross(pos2.xyz, pos3.xyz);
	
	fNormal =  (gs_in[0].model * vec4(fNormal, 1)).xyz;
    gl_Position =  gs_in[0].MVP * pos;
	EmitVertex();
    EndPrimitive();
    x+=dx;
    if((j+1) %sub_divisions == 0) {
      x=v0.x;
     y+=dy;
    }
  }
}