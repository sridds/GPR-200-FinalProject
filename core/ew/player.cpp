// Authored by Seth Riddensdale

#include "player.h"
#include <iostream>

float elapsed = 0.0f;
float rotationElapsed = 0.0f;

const Transform& Player::getTransform()
{
	return m_transform;
}

bool Player::isMoving() {
	return m_isMoving;
}
bool Player::isTurning() {
	return m_isTurning;
}

int Player::getDirection() {
	return m_direction;
}

void Player::moveForward()
{
	if (m_isMoving || m_isTurning) return;

	m_targetPosition = m_transform.position + (getFrontDir() * 2.0f);
	m_startPosition = m_transform.position;
	elapsed = 0.0f;

	cellPos = getProjectedForwardCell();

	m_isMoving = true;
}

void Player::moveBackward()
{
	if (m_isMoving || m_isTurning) return;

	m_targetPosition = m_transform.position + (-getFrontDir() * 2.0f);
	m_startPosition = m_transform.position;
	elapsed = 0.0f;

	cellPos = getProjectedBackwardCell();

	m_isMoving = true;
}

glm::vec2 Player::getProjectedForwardCell() {
	glm::vec2 temp = cellPos;

	if (m_direction == 0) {
		temp -= glm::vec2(1.0f, 0.0f);
	}
	else if (m_direction == 1) {
		temp += glm::vec2(0.0f, 1.0f);
	}
	else if (m_direction == 2) {
		temp -= glm::vec2(-1.0f, 0.0f);
	}
	else if (m_direction == 3) {
		temp += glm::vec2(0.0f, -1.0f);
	}

	return temp;
}

glm::vec2 Player::getProjectedBackwardCell() {
	glm::vec2 temp = cellPos;

	if (m_direction == 0) {
		temp += glm::vec2(1.0f, 0.0f);
	}
	else if (m_direction == 1) {
		temp -= glm::vec2(0.0f, 1.0f);
	}
	else if (m_direction == 2) {
		temp += glm::vec2(-1.0f, 0.0f);
	}
	else if (m_direction == 3) {
		temp -= glm::vec2(0.0f, -1.0f);
	}

	return temp;
}

void Player::turnLeft()
{
	if (m_isMoving || m_isTurning) return;

	m_direction--;
	m_direction %= 4;

	if (m_direction < 0) m_direction = 3;

	m_startYaw = m_curYaw;
	m_targetYaw = m_curYaw - 90.0f;
	rotationElapsed = 0.0f;

	m_isTurning = true;
}

void Player::turnRight()
{
	if (m_isMoving || m_isTurning) return;

	m_direction++;
	m_direction %= 4;

	if (m_direction < 0) m_direction = 3;

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

void Player::setPositionImmediate(glm::vec3 startPosition)
{
	m_transform.position = startPosition;
	m_startPosition = startPosition;
	m_targetPosition = startPosition;
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

float Player::getYaw() {
	return m_curYaw;
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

Player::Player(float duration, glm::vec3 startPosition)
{
	m_transform.position = startPosition;
	m_startPosition = startPosition;
	m_targetPosition = startPosition;

	//m_transform.rotation = glm::vec3(0, 0, 0);
	float forward = 180.0f + 90.0f;

	m_curYaw = forward;
	m_startYaw = forward;
	m_targetYaw = forward;
	m_transform.rotationAngle = forward;
	m_duration = duration;
}

glm::vec3 Player::getFrontDir()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(m_transform.rotationAngle)) * cos(glm::radians(0.0f));
	direction.y = sin(glm::radians(0.0f));
	direction.z = sin(glm::radians(m_transform.rotationAngle)) * cos(glm::radians(0.0f));
	m_front = glm::normalize(direction);

	return m_front;
}
