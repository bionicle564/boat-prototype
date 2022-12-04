#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices=256) out; 

uniform int sub_divisions;
uniform float time;

in VS_OUT {
    mat4 MVP;
	vec2 gUV;
} gs_in[]; 



vec4 Waves(vec4 pointIn)
{
	vec4 finalPoint;
	
	int M = 1;
	float deapthSolved = 1; //should use tanh
	float g = 9.81;
	float Kxm = 12;
	float Kzm = 0;
	float Am = 0.05;
	float phasem = 0;
	float Km = sqrt((Kxm * Kxm) + (Kzm * Kzm));
	float Wm2 = g * Km * deapthSolved;
	
	float theatam = (Kxm * pointIn.x) + (Kzm * pointIn.y) - (sqrt(Wm2) * (time / 10)) - phasem;
	
	int N = 2;
	float Kxn = 8;
	float Kzn = 8;
	float An = 0.04;
	float phasen = 0;
	float Kn = sqrt((Kxn * Kxn) + (Kzn * Kzn));
	float Wn2 = g * Kn * deapthSolved;
	
	float theatan = (Kxn * pointIn.x) + (Kzn * pointIn.y) - (sqrt(Wn2) * (time / 20)) - phasen;

	for(int m = 1; m <= M;m++)
	{
		finalPoint.x = pointIn.x - ((Kxm / Km) * (Am / deapthSolved) * sin(theatam)) - ((Kxn / Kn) * (An / deapthSolved) * sin(theatan));
		finalPoint.y = pointIn.y - ((Kzm / Km) * (Am / deapthSolved) * sin(theatam))- ((Kzn / Kn) * (An / deapthSolved) * sin(theatan));
		finalPoint.z = -Am * cos(theatam) - An * cos(theatan);
		
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
  
  
  for(int j=0;j<sub_divisions*sub_divisions;j++)  {
  
	
    //gl_Position =  gs_in[0].MVP * vec4(x,y,0,1);
    gl_Position =  gs_in[0].MVP * Waves(vec4(x,y,0,1));
    EmitVertex();
    //gl_Position =  gs_in[0].MVP * vec4(x,y+dy,0,1);
    gl_Position =  gs_in[0].MVP * Waves(vec4(x,y+dy,0,1));
    EmitVertex();
    //gl_Position =  gs_in[0].MVP * vec4(x+dx,y,0,1);
    gl_Position =  gs_in[0].MVP * Waves(vec4(x+dx,y,0,1));
    EmitVertex();
    //gl_Position =  gs_in[0].MVP * vec4(x+dx,y+dy,0,1);
    gl_Position =  gs_in[0].MVP * Waves(vec4(x+dx,y+dy,0,1));
	EmitVertex();
    EndPrimitive();
    x+=dx;
    if((j+1) %sub_divisions == 0) {
      x=v0.x;
     y+=dy;
    }
  }
}