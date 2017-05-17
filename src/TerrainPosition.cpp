#include "TerrainPosition.hpp"

TerrainPosition::TerrainPosition(int x, int z): x(x), z(z)
{
}

TerrainPosition::~TerrainPosition()
{
}

bool TerrainPosition::operator== (const TerrainPosition& other) const
{
	return (x == other.x) && (z == other.z);
}

bool TerrainPosition::operator!= (const TerrainPosition& other) const
{
	return !((x == other.x) && (z == other.z));
}

int TerrainPosition::hash(int key)
{
	key = ~key + (key << 15);
	key = key ^ (key >> 12);
	key = key + (key << 2);
	key = key ^ (key >> 4);
	key = key * 2048;
	key = key ^ (key >> 16);
	return key;
}

int TerrainPosition::hash(int x, int y)
{
	x = hash(x);
	y = hash(y);
	return hash(x ^ (y << 2));
}
