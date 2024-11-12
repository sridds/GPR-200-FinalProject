#include "transform.h"

class Player {
private:
	Transform m_transform;
	float m_duration; // the amount of time it takes to lerp to a position
	glm::vec3 m_targetPosition; // the target position to lerp to

public:
	const Transform& getTransform();
	void move(glm::vec3 amount); // called whenever we need to update the target position
	void update(float deltaTime); // should be called in a loop
};