// #version 120

// void main()
// {
	// gl_TexCoord[0] = gl_Vertex;
	// gl_Position = gl_Vertex * 2.0 - 1.0;
// }
#version 330

// attrib: vertex pos + height
// x, y - position from -1 to 1
// z    - water pressure that is applied to water's height map
//        that way you can draw on the water
layout(location = 0) in vec3 vVertex; 

// attrib: standard texture coords
layout(location = 1) in vec2 vTexCoord0;

// output: tex coord
out vec2 vVaryingTexCoord0;    

void main() 
{
	vVaryingTexCoord0 = vTexCoord0;    
	// vec4 tmp = vVertex * 2 - 1;
	
	// gl_Position = vec4(tmp.x, tmp.y, 0.0, 1.0);         
	gl_Position = vec4(vVertex.x, vVertex.y, 0.0, 1.0);         
}