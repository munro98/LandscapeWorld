#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in vec4 Position[1];
flat in int isVisible[1];

const vec4 normal = vec4(0.0,0.0,-1.0,0.0);//going to have to pass this in once instance rotation is implemented

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

out vec3 normal_out;


void grass_blade(vec4 position){
    vec4 position2 = vec4(position.x,position.y,position.z, 1.0);//take the actual position, without the rotation angle in the 4th parameter

    float cosineAngle = cos(position.w);
    float sineAngle = sin(position.w); 
    mat4 rotationMatrix = mat4(cosineAngle, 0.0, sineAngle, 0.0,
    0.0, 1.0, 0.0, 0.0, 
    -sineAngle, 0.0,0.0,0.0,
    0.0,0.0,0.0,1.0);
    

    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4(-0.05, 0.0, 0.0, 0.0)); // 1  
    EmitVertex();   
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.05, 0.0, 0.0, 0.0)); // 2
    EmitVertex();
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4(0.05,  0.2, 0.0, 0.0)); // 3
    EmitVertex();
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.0,  0.4, 0.0, 0.0)); // 4
    EmitVertex();
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.05,  0.6, 0.0, 0.0)); // 5
    EmitVertex();
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.1,  0.4, 0.0, 0.0)); // 6
    EmitVertex();
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.15,  0.8, 0.0, 0.0)); // 7
    EmitVertex();
    EndPrimitive();
    vec4 rotatedNormal = rotationMatrix*normal;
    normal_out = vec3(rotatedNormal.x,rotatedNormal.y,rotatedNormal.z); 
}

void main() {   
    if(isVisible[0] == 1){
    grass_blade(Position[0]);
    }
}


