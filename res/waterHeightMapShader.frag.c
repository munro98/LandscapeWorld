#version 330 core

uniform sampler2D WaterHeightMap;

uniform float waterHeightMapDistance_W;
uniform float waterHeightMapDistance_H;
uniform float sin45;
uniform float attenuation;

in vec2 TexCoord;

out vec4 vFragColor;

void main()
{
	vec2 currentVelocityHight = texture(WaterHeightMap, TexCoord.st).rg;

	float force = 0.0;

	// Calculate the force from each neighbouring square
	vec2 nextNeighbour = TexCoord.st - vec2(waterHeightMapDistance_W, waterHeightMapDistance_H);
	force += sin45 * (texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y);
	
	nextNeighbour.x += waterHeightMapDistance_W;
	force += texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y;
	
	nextNeighbour.x += waterHeightMapDistance_W;
	force += sin45 * (texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y);
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y;
	
	nextNeighbour.y += waterHeightMapDistance_H;
	force += sin45 * (texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y);
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y;
	
	nextNeighbour.x -= waterHeightMapDistance_W;
	force += sin45 * (texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y);
	
	nextNeighbour.y -= waterHeightMapDistance_H;
	force += texture(WaterHeightMap, nextNeighbour).y - currentVelocityHight.y;

	// divide it by the number of neighbouring squares
	force /= 8.0;

	// add force
	currentVelocityHight.x += force;
	
	// add height
	currentVelocityHight.y += currentVelocityHight.x;
	
	// reduce force to attone for attenuation
	currentVelocityHight.y *= attenuation ;

	vFragColor = vec4(currentVelocityHight, 0.0, 0.0);
}