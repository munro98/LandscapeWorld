
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

#include "World.hpp"
#include "Terrain.hpp"
#include "Camera.hpp"

#define RECURSION_COUNT 200.0f
#define RAY_RANGE 100.0f

class MousePicker
{

public:
	MousePicker(World & world, Terrain & t);

	~MousePicker();

	glm::vec2 m_currentWaterTexturePoint;


	void update(int width, int height, int mouseX, int mouseY, glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix, glm::vec3 cameraPosition);

	glm::vec3 calculateMouseRay(int width, int height, int mouseX, int mouseY, glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix);

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords, glm::mat4 & viewMatrix);

	glm::vec4 toEyeCoords(glm::vec4 clipCoords, glm::mat4 & projectionMatrix);

	glm::vec2 getNormalisedDeviceCoordinates(int width, int hieght, float mouseX, float mouseY);

	glm::vec3 getPointOnRay(glm::vec3 ray, float distance, glm::vec3 cameraPosition);


	glm::vec3 stepSearch(glm::vec3 ray);

	glm::vec3 binarySearch(int count, float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition);

	bool intersectionInRange(float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition);

	glm::vec3 getCurrentTerrainPoint();
	glm::vec3 getCurrentWaterPoint();
	glm::vec3 getCurrentRay();

private:
	glm::vec3 m_currentRay;

	
	Terrain & m_terrain;
	World & m_world;
	glm::vec3 m_currentTerrainPoint;

	glm::vec3 m_currentWaterPoint;
	

	bool isUnderGround(glm::vec3 testPoint);

	glm::vec3 binarySearchWater(int count, float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition);

	bool intersectionInRangeWater(float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition);

	bool isUnderWater(glm::vec3 testPoint);

	Terrain getTerrain(float worldX, float worldZ);

};