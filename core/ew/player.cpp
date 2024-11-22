#include "player.h"

float elapsed = 0.0f;

const Transform& Player::getTransform()
{
	return m_transform;
}

void Player::move(glm::vec3 amount)
{
	if(m_isMoving) return;

	m_targetPosition = m_transform.position + (amount * m_stepSize);
	m_startPosition = m_transform.position;
	elapsed = 0.0f;

	m_isMoving = true;
}

void Player::update(float deltaTime) 
{
	updatePos(deltaTime);
	updateRot(deltaTime);
}

void Player::updatePos(float deltaTime)
{
	glm::vec3 start = m_startPosition;

	if (elapsed < m_duration) {
		elapsed += deltaTime;
		float t = elapsed / m_duration;

		// ease out quart function https://easings.net/#easeOutQuart
		m_transform.position = glm::mix(start, m_targetPosition, 1 - pow(1 - t, 4));

		return;
	}
	else {
		m_transform.position = m_targetPosition;
		m_isMoving = false;
	}
}

void Player::updateRot(float deltaTime)
{

}

Player::Player(float duration, float stepSize)
{
	m_duration = duration;
	m_stepSize = stepSize;
}