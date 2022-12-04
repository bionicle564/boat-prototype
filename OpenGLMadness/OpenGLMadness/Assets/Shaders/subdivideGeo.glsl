#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices=256) out; 

const int sub_divisions = 2;


in VS_OUT {
    mat4 MVP;
} gs_in[]; 

void main() {
  vec4 v0 = gl_in[0].gl_Position;
  vec4 v1 = gl_in[1].gl_Position;
  vec4 v2 = gl_in[2].gl_Position;
  float dx = abs(v0.x-v2.x)/sub_divisions;
  float dz = abs(v0.y-v1.y)/sub_divisions;
  float x=v0.x;
  float z=v0.y;
  for(int j=0;j<sub_divisions*sub_divisions;j++) {
    gl_Position =  gs_in[0].MVP * vec4(x,z,0,1);
    EmitVertex();
    gl_Position =  gs_in[0].MVP * vec4(x,z+dz,0,1);
    EmitVertex();
    gl_Position =  gs_in[0].MVP * vec4(x+dx,z,0,1);
    EmitVertex();
    gl_Position =  gs_in[0].MVP * vec4(x+dx,z+dz,0,1);
    EmitVertex();
    EndPrimitive();
    x+=dx;
    if((j+1) %sub_divisions == 0) {
      x=v0.x;
     z+=dz;
    }
  }
}