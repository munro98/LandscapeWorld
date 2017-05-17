#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void forward(float delta);
	void backward(float delta);
	void left(float delta);
	void right(float delta);
	void rotate(float pitch, float yaw);

	glm::vec3 getPosition();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getRight();


private:

	float mSpeed;
	float mPitch, mYaw;

	glm::vec3 mTarget;
	glm::vec3 mPosition;

	glm::vec3 mDirection;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
};

