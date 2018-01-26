#version 330 core

uniform sampler2D waterHeightMap;

uniform float waterHeightMapDistance_W;
uniform float waterHeightMapDistance_H;
uniform float sin45;
uniform float attenuation;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec2 currentVelocityHight = texture(waterHeightMap, TexCoord.st).rg;

	float force = 0.0f;

	// Calculate the force from each neighbouring square
	vec2 nextNeighbour = TexCoord.st - vec2(waterHeightMapDistance_W, waterHeightMapDistance_H);
	force += sin45 * (texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r);

	nextNeighbour.x += waterHeightMapDistance_W;
	force += texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r;
	
	nextNeighbour.x += waterHeightMapDistance_W;
	force += sin45 * (texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r);
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r;
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += sin45 * (texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r);
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r;
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += sin45 * (texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r);
	
	nextNeighbour.y -= waterHeightMapDistance_H;
	force += texture(waterHeightMap, nextNeighbour).r - currentVelocityHight.r;

	// divide it by the number of neighbouring squares
	force /= 8.0;

	// add force
	currentVelocityHight.g += force;
	
	// add height
	currentVelocityHight.r += currentVelocityHight.g;
	
	// reduce force to atone for attenuation
	currentVelocityHight.r *= attenuation;

	vFragColor = vec4(currentVelocityHight, 0.0, 0.0);
}