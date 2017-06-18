#include "MousePicker.hpp"



MousePicker::MousePicker(World &world, Terrain &t) : m_world(world), m_terrain(t), m_currentWaterTexturePoint(glm::vec2(0.0f, 0.0f))
{
	
}


MousePicker::~MousePicker()
{
}


void MousePicker::update(int width, int height, int mouseX, int mouseY, glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix, glm::vec3 cameraPosition) {
	m_currentRay = calculateMouseRay(width, height, mouseX, mouseY, projectionMatrix, viewMatrix);//
	m_currentTerrainPoint = binarySearch(0, 0, RAY_RANGE, m_currentRay, cameraPosition);
	m_currentWaterPoint = binarySearchWater(0, 0, RAY_RANGE, m_currentRay, cameraPosition);

	// substract the scale factor from the position to get the starting position
	m_currentWaterTexturePoint.x = m_currentWaterPoint.x - (m_waterPosition.x - m_waterScaleFactor);
	m_currentWaterTexturePoint.y = m_currentWaterPoint.z - (m_waterPosition.z - m_waterScaleFactor);

	m_currentWaterTexturePoint.x /= 4.0f;
	m_currentWaterTexturePoint.y /= 4.0f;
}

glm::vec3 MousePicker::calculateMouseRay(int width, int height, int mouseX, int mouseY, glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix) {
	glm::vec2 mousePositionNormalized = getNormalisedDeviceCoordinates(width, height, mouseX, mouseY);////
	glm::vec4 mouseClipSpace = glm::vec4(mousePositionNormalized.x, mousePositionNormalized.y, -1.0f, 1.0f);
	glm::vec4 mouseEyeSpace = toEyeCoords(mouseClipSpace, projectionMatrix);
	//std::cout << mouseEyeSpace.x << " " << mouseEyeSpace.y << "\n";
	glm::vec3 worldRay = toWorldCoords(mouseEyeSpace, viewMatrix);
	return worldRay;
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords, glm::mat4 & viewMatrix) {
	glm::mat4 invertedView = glm::inverse(viewMatrix);
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay = glm::normalize(mouseRay);
	return mouseRay;
}

glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords, glm::mat4 & projectionMatrix) {
	glm::mat4 invertedProjection = glm::inverse(projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec2 MousePicker::getNormalisedDeviceCoordinates(int width, int height, float mouseX, float mouseY) {
	float x = (2.0f * (float)mouseX) / (float)width - 1.0f;
	float y = (2.0f * (float)mouseY) / (float)height - 1.0f;
	//std::cout << x << " " << y << "\n";
	return glm::vec2(x, -y);
}

glm::vec3 MousePicker::getPointOnRay(glm::vec3 ray, float distance, glm::vec3 cameraPosition) {
	glm::vec3 scaledRay = glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	//return start + scaledRay;
	return cameraPosition + scaledRay;
}

glm::vec3 MousePicker::stepSearch(glm::vec3 ray) {
	return m_currentRay * 2.0f;
}

glm::vec3 MousePicker::binarySearch(int count, float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition) {
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		glm::vec3 endPoint = getPointOnRay(ray, half, cameraPosition);
		return endPoint;
	}
	if (intersectionInRange(start, half, ray, cameraPosition)) {
		return binarySearch(count + 1, start, half, ray, cameraPosition);
	} else {
		return binarySearch(count + 1, half, finish, ray, cameraPosition);
	}
}

bool MousePicker::intersectionInRange(float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition) {
	glm::vec3 startPoint = getPointOnRay(ray, start, cameraPosition);
	glm::vec3 endPoint = getPointOnRay(ray, finish, cameraPosition);
	if (!isUnderGround(startPoint) && isUnderGround(endPoint)) {
		return true;
	} else {
		return false;
	}
}

bool MousePicker::isUnderGround(glm::vec3 testPoint) {
	float height = m_world.heightAt(testPoint.x, testPoint.z);
	
	if (testPoint.y < height) {
		return true;
	} else {
		return false;
	}
}


//Water stuff
//////////////////////////////////////////////////////////////////////////

glm::vec3 MousePicker::binarySearchWater(int count, float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition) {
	float half = start + ((finish - start) / 2.0f);
	if (count >= RECURSION_COUNT) {
		glm::vec3 endPoint = getPointOnRay(ray, half, cameraPosition);
		return endPoint;

	}
	if (intersectionInRangeWater(start, half, ray, cameraPosition)) {
		return binarySearchWater(count + 1, start, half, ray, cameraPosition);
	}
	else {
		return binarySearchWater(count + 1, half, finish, ray, cameraPosition);
	}
}

bool MousePicker::intersectionInRangeWater(float start, float finish, glm::vec3 ray, glm::vec3 cameraPosition) {
	glm::vec3 startPoint = getPointOnRay(ray, start, cameraPosition);
	glm::vec3 endPoint = getPointOnRay(ray, finish, cameraPosition);
	if (!isUnderWater(startPoint) && isUnderWater(endPoint)) {
		return true;
	}
	else {
		return false;
	}
}

bool MousePicker::isUnderWater(glm::vec3 testPoint) {
	// Plus 1 because it will be translated by one
	if (testPoint.y < m_waterPosition.y + 1.5) {
		return true;
	}
	else {
		return false;
	}
}

void MousePicker::SetWaterPosition(glm::vec3 position)
{
	m_waterPosition = position;
}

glm::vec3 MousePicker::getCurrentTerrainPoint() {
	return m_currentTerrainPoint;
}

glm::vec3 MousePicker::getCurrentWaterPoint()
{
	return m_currentWaterPoint;
}

glm::vec3 MousePicker::getCurrentRay() {
	return m_currentRay;
}