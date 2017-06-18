#include "Camera.hpp"



Camera::Camera()
{
	mPosition = glm::vec3(0.0f, 0.0f, 2.0f);

	mTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	mDirection = glm::normalize(mPosition - mTarget);
	mFront = glm::vec3(0.0f, 0.0f, -1.0f);
	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::normalize(glm::cross(mUp, mDirection));

	m_front2 = glm::vec3(0.0f, 0.0f, -1.0f);
	m_up2 = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right2 = glm::normalize(glm::cross(mUp, mDirection));

	mSpeed = 50.0f;
	m_acceleration = 800.0f;
	m_gravity = 100.0f;
	m_maxSpeed = 50.0f;

	mYaw = -90.0f;
	mPitch = 0.0f;

	rotate(0.0f, 0.0f);
}


Camera::~Camera()
{
}

void Camera::update(float delta, World& world, bool takeInput) {

	glm::vec2 v(m_velocity.x, m_velocity.z);
	if (glm::length(v) != 0) {
		float deceleration = 600 * delta;
		m_velocity -= deceleration * glm::normalize(glm::vec3(v.x, 0.0f, v.y));
		//std::cout << deceleration;
	}
	//else if (glm::length(glm::vec3(m_velocity.x, 0.0f, m_velocity.z)) > m_maxSpeed) {
	if (glm::length(glm::vec3(m_velocity.x, 0.0, m_velocity.z)) > m_maxSpeed) {
		glm::vec2 v2(m_velocity.x, m_velocity.z);
		v2 = glm::normalize(v2);
		m_velocity = glm::vec3(v2.x * m_maxSpeed, m_velocity.y, v2.y * m_maxSpeed);// m_maxSpeed * glm::normalize(m_velocity);
		//std::cout << mPosition.x << " " << mPosition.y << " " << mPosition.z << "\n";
	}

	if (mPosition.y - 4.4f > world.heightAt(mPosition.x, mPosition.z)) {
		m_velocity.y -= m_gravity * delta;
	}

	mPosition += m_velocity * delta;

	if (mPosition.y - 4.4f < world.heightAt(mPosition.x, mPosition.z)) {
		mPosition.y = world.heightAt(mPosition.x, mPosition.z) + 4.4f;
		m_velocity.y = 0.0f;
	}
	

	//std::cout << mPosition.x << " " << mPosition.y << " " << mPosition.z << "\n";
	
}

void Camera::forward(float delta) {
	mPosition += mSpeed * mFront * delta;
}

void Camera::backward(float delta) {
	mPosition -= mSpeed * mFront * delta;
}

void Camera::left(float delta) {
	mPosition -= mSpeed * delta * glm::normalize(glm::cross(mFront, mUp));
}

void Camera::right(float delta) {
	mPosition += mSpeed * delta * glm::normalize(glm::cross(mFront, mUp));
}


void Camera::forwardAccelerate(float delta) {
	m_velocity += m_acceleration * m_front2 * delta;
}

void Camera::backwardAccelerate(float delta) {
	m_velocity -= m_acceleration * m_front2 * delta;
}

void Camera::leftAccelerate(float delta) {
	m_velocity -= m_acceleration * delta * glm::normalize(glm::cross(m_front2, m_up2));
}

void Camera::rightAccelerate(float delta) {
	m_velocity += m_acceleration * delta * glm::normalize(glm::cross(m_front2, m_up2));
}

void Camera::rotate(float x, float y) {

	mYaw += x;
	mPitch += y;

	// Clamp camera pitch
	if (mPitch > 89.0f)
		mPitch = 89.0f;
	if (mPitch < -89.0f)
		mPitch = -89.0f;


	glm::vec3 front;
	front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(front);

	mUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mRight = glm::normalize(glm::cross(mUp, mDirection));



	glm::vec3 front2;
	front2.x = cos(glm::radians(mYaw));
	front2.z = sin(glm::radians(mYaw));
	m_front2 = glm::normalize(front2);

	m_up2 = glm::vec3(0.0f, 1.0f, 0.0f);
	m_right2 = glm::normalize(glm::cross(m_up2, mDirection));

}

glm::vec3 Camera::getPosition() {
	return mPosition;

}

glm::vec3 Camera::getFront() {
	return mFront;

}

glm::vec3 Camera::getUp() {
	return mUp;

}

glm::vec3 Camera::getRight() {
	return mRight;

}
