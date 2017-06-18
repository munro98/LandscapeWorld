#version 330 core

uniform sampler2D WaterHeightMap;

uniform float waterHeightMapDistance_W;
uniform float waterHeightMapDistance_H;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec2 currentVelocityHight = texture(WaterHeightMap, TexCoord.st).rg;

	float force = 0.0;

	// Calculate the force from each neighbouring square
	vec2 nextNeighbour = TexCoord.st - vec2(waterHeightMapDistance_W, waterHeightMapDistance_H);
	force += 0.707107 * (texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g);
	
	nextNeighbour.x += waterHeightMapDistance_W;
	force += texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g;
	
	nextNeighbour.x += waterHeightMapDistance_W;
	force += 0.707107 * (texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g);
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g;
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += 0.707107 * (texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g);
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g;
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += 0.707107 * (texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g);
	
	nextNeighbour.y -= waterHeightMapDistance_H;
	force += texture(WaterHeightMap, nextNeighbour).g - currentVelocityHight.g;

	// divide it by the number of neighbouring squares
	force /= 8.0;

	// set new force
	currentVelocityHight.r += force;
	
	// set new height
	currentVelocityHight.g += currentVelocityHight.r;
	
	// gradualy decline height
	currentVelocityHight.g *= 0.99;

	vFragColor = vec4(currentVelocityHight, 0.0, 0.0);
}