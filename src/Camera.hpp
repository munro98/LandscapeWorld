#pragma once

#include "World.hpp"
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	void update(float delta, World & world, bool takeInput);

	void forward(float delta);
	void backward(float delta);
	void left(float delta);
	void right(float delta);
	void forwardAccelerate(float delta);
	void backwardAccelerate(float delta);
	void leftAccelerate(float delta);
	void rightAccelerate(float delta);
	void rotate(float pitch, float yaw);

	glm::vec3 getPosition();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getRight();

	float mPitch, mYaw;
private:

	float mSpeed;
	
	float m_acceleration;
	float m_maxSpeed;
	float m_gravity;

	glm::vec3 mTarget;
	glm::vec3 mPosition;
	glm::vec3 m_velocity;
	

	glm::vec3 mDirection;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;

	glm::vec3 m_front2;
	glm::vec3 m_up2;
	glm::vec3 m_right2;
};

