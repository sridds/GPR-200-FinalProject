// Authored by Christopher Eichert

#include "external/glad.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
	Transform();
	glm::vec3 position;
	glm::vec3 rotation; //Euler angles
	glm::vec3 scale;
	float rotationAngle;
	glm::mat4 getModelMatrix();
};