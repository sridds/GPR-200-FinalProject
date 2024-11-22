#include "player.h"

float elapsed = 0.0f;

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

	m_startYaw = m_front.x;
	m_targetYaw = m_curYaw - 90.0f;
	elapsed = 0.0f;

	m_isTurning = true;
}

void Player::turnRight()
{
	if(m_isMoving || m_isTurning) return;

	m_startYaw = m_front.x;
	m_targetYaw = m_curYaw + 90.0f;
	elapsed = 0.0f;

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
	glm::vec3 m_dir;
	m_dir.x = cos(glm::radians(m_curYaw)) * cos(glm::radians(m_curYaw));
	m_front = glm::normalize(m_dir);

	if (elapsed < m_duration) {
		elapsed += deltaTime;
		float t = elapsed / m_duration;

		// ease out quart function https://easings.net/#easeOutQuart
		m_curYaw = glm::mix(m_startYaw, m_targetYaw, 1 - pow(1 - t, 4));

		return;
	}
	else {
		m_curYaw = m_targetYaw;
		m_isTurning = false;
	}
}

Player::Player(float duration, float stepSize)
{
	m_duration = duration;
	m_stepSize = stepSize;
}

glm::vec3 Player::getFrontDir()
{
	return m_front;
}
