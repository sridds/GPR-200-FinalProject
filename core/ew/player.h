#include "transform.h"

class Player {
private:
	Transform m_transform;
	float m_duration; // the amount of time it takes to lerp to a position
	float m_stepSize; // the size of each step

	// positions for lerping
	glm::vec3 m_startPosition;
	glm::vec3 m_targetPosition; // the target position to lerp to

	// rotations for lerping
	float m_startYaw;
	float m_targetYaw;
	float m_curYaw;

	glm::vec3 m_front;

	// flags
	bool m_isMoving;
	bool m_isTurning;

	// helper functions
	void updatePos(float deltaTime);
	void updateRot(float deltaTime);

public:
	// returns the transform of the player
	const Transform& getTransform();
	glm::vec3 getFrontDir();

	void move(glm::vec3 amount); // called whenever we need to update the target position
	void turnLeft();
	void turnRight();
	void update(float deltaTime); // should be called in a loop

	Player(float duration, float stepSize);
};