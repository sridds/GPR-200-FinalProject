#include "transform.h"

class Player {
private:
	Transform m_transform;
	float m_duration; // the amount of time it takes to lerp to a position

	// positions for lerping
	glm::vec3 m_startPosition;
	glm::vec3 m_targetPosition; // the target position to lerp to

	// rotations for lerping
	float m_startYaw;
	float m_targetYaw;
	float m_curYaw;

	int m_direction;

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
	glm::vec2 cellPos;

	void moveForward(); // called whenever we need to update the target position
	void moveBackward();

	void turnLeft();
	void turnRight();
	void update(float deltaTime); // should be called in a loop
	glm::vec2 getProjectedForwardCell();
	glm::vec2 getProjectedBackwardCell();

	bool isMoving();
	bool isTurning();
	int getDirection();
	float getYaw();

	Player(float duration, glm::vec3 startPosition);
};