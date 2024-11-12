#include "player.h"

const Transform& Player::getTransform()
{
	return m_transform;
}

void Player::move(glm::vec3 amount)
{
	m_targetPosition = m_transform.position + amount;
	elapsed = 0.0f;
}

float elapsed = 0.0f;

void Player::update(float deltaTime) 
{
	glm::vec3 start = m_transform.position;

	if (elapsed < m_duration) {
		elapsed += deltaTime;
		float t = elapsed / m_duration;

		// ease out quart function https://easings.net/#easeOutQuart
		transform.position = glm::mix(start, m_targetPosition, 1 - pow(1 - t, 4));

		return;
	}
	else {
		transform.position = m_targetPosition;
	}
}