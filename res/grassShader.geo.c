 
#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 7) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

//TODO take a rotation

//TODO output normals

void grass_blade(vec4 position){
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.05, 0.0, 0.0, 0.0); // 1  
    EmitVertex();   
    gl_Position = position + vec4( 0.05, 0.0, 0.0, 0.0); // 2
    EmitVertex();
    gl_Position = position + vec4(0.05,  0.1, 0.0, 0.0); // 3
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.2, 0.0, 0.0); // 4
    EmitVertex();
    gl_Position = position + vec4( 0.05,  0.3, 0.0, 0.0); // 5
    EmitVertex();
    gl_Position = position + vec4( 0.1,  0.2, 0.0, 0.0); // 6
    EmitVertex();
    gl_Position = position + vec4( 0.15,  0.4, 0.0, 0.0); // 7
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {    
    grass_blade(gl_in[0].gl_Position);
}


