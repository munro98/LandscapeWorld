#version 330 core 

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;

out vec4 outPosition;
flat out int objectVisible;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPos;

void main() {
	outPosition = position+offset;//get the world position of the point
	objectVisible = 1;

	//TODO check whether this instance is within the height parameters where grass should be
	// y value of offset should be > -60 and <=10, otherwise set objectVisible to 0

	//TODO check whether the object is within some cutoff distance of the camera
	//subtract outPosition from camPos to get the vector and check its magnitude, if the agnitude is greater than some value then set objectVisible to 0

	//TODO check whether this instance falls within the frustum
	//create a bounding box for the grass blade and check some point of it falls within the frustum, if not set ObjectVisible to 0

	vec4 MVPPosition = projection*view*model*vec4(translatedPosition, 1.0);//needed for the frustum culling

} 
