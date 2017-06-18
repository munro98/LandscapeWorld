#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in vec4 Position[1];
flat in int isVisible[1];
in vec3 Normal[1];

const vec4 normalDefault = vec4(0.0,0.0,-1.0,0.0);//going to have to pass this in if rotatioon per instance is implemented

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

out vec3 normal;
out vec3 viewDirection;


void grass_blade(vec4 position){
    vec4 position2 = vec4(position.x,position.y,position.z, 1.0);//take the actual position, without the rotation angle in the 4th parameter

    float cosineAngle = cos(position.w);
    float sineAngle = sin(position.w); 
    mat4 rotationMatrix = mat4(cosineAngle, 0.0, sineAngle, 0.0,
    0.0, 1.0, 0.0, 0.0, 
    -sineAngle, 0.0,0.0,0.0,
    0.0,0.0,0.0,1.0);
    
    vec4 rotatedNormal = rotationMatrix*normalDefault;
    normal = normalize(vec3(rotatedNormal.x,rotatedNormal.y,rotatedNormal.z)); 
    normal = Normal[0];
    
    viewDirection = normalize(camPos - vec3(position.x,position.y,position.z));  

    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4(-0.05, 0.0, 0.0, 0.0)); // 1  
    EmitVertex();   
    gl_Position = projection*view*model*(position2 + rotationMatrix*vec4( 0.00, 0.0, 0.0, 0.0)); // 2
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
}

mat4 rotationMatrix(vec3 axis, float angle){
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main() {   
    if(isVisible[0] == 1){
      grass_blade(Position[0]);
    }
}


