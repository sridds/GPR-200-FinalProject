#include "transform.h"

Transform::Transform() {
	position = glm::vec3(1);
	rotation = glm::vec3(1);
	scale = glm::vec3(1);
	rotationAngle = 0;
}

glm::mat4 Transform::getModelMatrix() {
	//Create identity Matrix
	glm::mat4 model = glm::mat4(1.0f);

	//Translation model
	model = glm::translate(model, position);

	//Rotation model
	if (rotation != glm::vec3(0)) {
		model = glm::rotate(model, glm::radians(rotationAngle), rotation);
	}

	//Scale model
	model = glm::scale(model, scale);

	return model;
}