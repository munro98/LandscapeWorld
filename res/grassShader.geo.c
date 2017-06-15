#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in VS_OUT {
    vec3 color;
} gs_in[];

const vec3 normal = vec3(0.0,0.0,-1.0);

out vec3 fColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

out vec3 normal_out;

//TODO take a rotation

//TODO normals

void grass_blade(vec4 position){

    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = projection*view*model*(position + vec4(-0.05, 0.0, 0.0, 0.0)); // 1  
    EmitVertex();   
    gl_Position = projection*view*model*(position + vec4( 0.05, 0.0, 0.0, 0.0)); // 2
    EmitVertex();
    gl_Position = projection*view*model*(position + vec4(0.05,  0.2, 0.0, 0.0)); // 3
    EmitVertex();
    gl_Position = projection*view*model*(position + vec4( 0.0,  0.4, 0.0, 0.0)); // 4
    EmitVertex();
    gl_Position = projection*view*model*(position + vec4( 0.05,  0.6, 0.0, 0.0)); // 5
    EmitVertex();
    gl_Position = projection*view*model*(position + vec4( 0.1,  0.4, 0.0, 0.0)); // 6
    EmitVertex();
    gl_Position = projection*view*model*(position + vec4( 0.15,  0.8, 0.0, 0.0)); // 7
    fColor = vec3(1.0, 1.0, 1.0);
    normal_out = normal; //TODO modify this based on the rotation once it is implemented
    EmitVertex();
    EndPrimitive();
}

void main() {    
    grass_blade(gl_in[0].gl_Position);
}


