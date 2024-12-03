#include "player.h"

float elapsed = 0.0f;
float rotationElapsed = 0.0f;

const Transform& Player::getTransform()
{
	return m_transform;
}

void Player::move(glm::vec3 amount)
{
	if(m_isMoving || m_isTurning) return;

	m_targetPosition = m_transform.position + (amount * m_stepSize);
	m_startPosition = m_transform.position;
	elapsed = 0.0f;

	m_isMoving = true;
}

void Player::turnLeft()
{
	if(m_isMoving || m_isTurning) return;

	// cos(yaw) * cos(pitch)
	m_startYaw = m_curYaw;
	m_targetYaw = m_curYaw - 90.0f;
	rotationElapsed = 0.0f;

	m_isTurning = true;
}

void Player::turnRight()
{
	if(m_isMoving || m_isTurning) return;

	m_startYaw = m_curYaw;
	m_targetYaw = m_curYaw + 90.0f;
	rotationElapsed = 0.0f;

	m_isTurning = true;
}

void Player::update(float deltaTime) 
{
	updatePos(deltaTime);
	updateRot(deltaTime);
}

void Player::updatePos(float deltaTime)
{
	if (elapsed < m_duration) {
		elapsed += deltaTime;
		float t = elapsed / m_duration;

		// ease out quart function https://easings.net/#easeOutQuart
		m_transform.position = glm::mix(m_startPosition, m_targetPosition, 1 - pow(1 - t, 4));

		return;
	}
	else {
		m_transform.position = m_targetPosition;
		m_isMoving = false;
	}
}

void Player::updateRot(float deltaTime)
{
	if (rotationElapsed < m_duration) {
		rotationElapsed += deltaTime;
		float t = rotationElapsed / m_duration;

		// ease out quart function https://easings.net/#easeOutQuart
		m_curYaw = glm::mix(m_startYaw, m_targetYaw, 1 - pow(1 - t, 4));
		m_transform.rotationAngle = m_curYaw;

		return;
	}
	else {
		m_curYaw = m_targetYaw;
		m_isTurning = false;
	}
}

Player::Player(float duration, float stepSize)
{
	m_transform.rotation = glm::vec3(0, 1, 0);
	m_transform.rotationAngle = 0.0f;

	m_duration = duration;
	m_stepSize = stepSize;
}

glm::vec3 Player::getFrontDir()
{
	glm::vec3 m_dir;
	m_dir.x = cos(glm::radians(m_curYaw)) * cos(glm::radians(m_curYaw));

	m_front = glm::normalize(m_dir);

	/*
		glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);*/
	return m_front;
}
