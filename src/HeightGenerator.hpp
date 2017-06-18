#pragma once
#define PI 3.1415926535897932384626433832795 
#define AMPLITUDE 64.0f
#include <random>
#include <glm/gtc/type_ptr.hpp>

#include "Terrain.hpp"

class HeightGenerator
{
public:
	HeightGenerator();
	~HeightGenerator();

	static float generateHeight(int x, int z);
	static float generateInterpolatedNoise(float x, float z);
	static float interpolate(float a, float b, float blend);
	static float lerp(float a, float b, float f);
	static float generateNoise(int x, int z);

	static float generateNoise2(int x, int z);

	static int hash(int key);

	static int hash(int x, int y);


	static float random();

	static int m_seed;
	static bool m_interpolateNoise;


};

