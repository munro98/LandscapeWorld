#pragma once
#include <functional>
#include <cmath>

class TerrainPosition
{
public:
	TerrainPosition(int x, int z);
	~TerrainPosition();
	bool operator==(const TerrainPosition & other) const;
	bool operator!=(const TerrainPosition & other) const;

	static int hash(int key);
	static int hash(int x, int y);

	int x, z;
	
};

namespace std
{
	template<>
	struct hash<TerrainPosition>
	{
		size_t operator() (TerrainPosition const& v) const
		{
			return TerrainPosition::hash(v.x, v.z);
		}

	};
}
