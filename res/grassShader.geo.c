#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in VS_OUT {
    vec3 color;
    //int isVisible;
} gs_in[];

//flat in int isVisible;
const vec3 normal = vec3(0.0,0.0,-1.0);

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

out vec3 normal_out;

//TODO take a rotation

//TODO normals

void grass_blade(vec4 position){
    vec4 position2 = vec4(position.x,position.y,position.z, 1.0);

    gl_Position = projection*view*model*(position2 + vec4(-0.05, 0.0, 0.0, 0.0)); // 1  
    EmitVertex();   
    gl_Position = projection*view*model*(position2 + vec4( 0.05, 0.0, 0.0, 0.0)); // 2
    EmitVertex();
    gl_Position = projection*view*model*(position2 + vec4(0.05,  0.2, 0.0, 0.0)); // 3
    EmitVertex();
    gl_Position = projection*view*model*(position2 + vec4( 0.0,  0.4, 0.0, 0.0)); // 4
    EmitVertex();
    gl_Position = projection*view*model*(position2 + vec4( 0.05,  0.6, 0.0, 0.0)); // 5
    EmitVertex();
    gl_Position = projection*view*model*(position2 + vec4( 0.1,  0.4, 0.0, 0.0)); // 6
    EmitVertex();
    gl_Position = projection*view*model*(position2 + vec4( 0.15,  0.8, 0.0, 0.0)); // 7
    EmitVertex();
    EndPrimitive();
    normal_out = normal; //TODO modify this based on the rotation once it is implemented
}

void main() {    
    //if(isVisible == 1){
      grass_blade(gl_in[0].gl_Position);
    //}
}


