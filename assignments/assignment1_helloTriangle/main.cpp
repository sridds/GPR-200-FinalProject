#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include <ew/procGen.h>
#include <ew/shader.h>
#include <ew/texture.h>

#include <glm/gtc/matrix_transform.hpp>

//Include IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
const int NUM_CUBES = 20;

glm::vec3 cubePositions[NUM_CUBES];

//CAMERA
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f, pitch = 0;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 60.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float orthoHeight = 10.0f;

glm::vec3 lightPosition = glm::vec3(0);
glm::vec3 lightColor = glm::vec3(1);
struct Material {
	float ambientK = 0.1f;
	float diffuseK = 0.5f;
	float specularK = 0.5f;
	float shininess = 64.0f;
	bool blinnPhong = true;
} material;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//AFTER gladLoadGL
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();


	
	//glfwSetMouseButtonCallback(window, mouse_button_callback);
	

	//Initialization goes here!
	ew::Mesh cubeMesh = ew::Mesh(ew::createCube(1.0f));
	ew::Shader litShader = ew::Shader("assets/lit.vert", "assets/lit.frag");
	ew::Shader unlitShader = ew::Shader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTex = ew::loadTexture("assets/brick.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glBindTexture(GL_TEXTURE_2D, brickTex);
	float cubeFieldRadius = 5.0f;

	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubePositions[i] = glm::vec3(
			ew::RandomRange(-cubeFieldRadius, cubeFieldRadius), 
			ew::RandomRange(-cubeFieldRadius, cubeFieldRadius), 
			ew::RandomRange(-cubeFieldRadius, cubeFieldRadius));
	}
	
	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);

		//Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float time = (float)glfwGetTime();

		//glPolygonMode(GL_FRONT, GL_LINE);


		litShader.use();

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projMatrix = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

		litShader.setVec3("_ViewPos", cameraPos);
		litShader.setMat4("_ViewProjection", projMatrix * view);
		litShader.setVec3("_LightColor", lightColor);
		litShader.setVec3("_LightPos", lightPosition);
		litShader.setFloat("_Material.ambientK", material.ambientK);
		litShader.setFloat("_Material.diffuseK", material.diffuseK);
		litShader.setFloat("_Material.specularK", material.specularK);
		litShader.setFloat("_Material.shininess", material.shininess);
		litShader.setInt("_Material.blinnPhong", material.blinnPhong);
		//Draw 10 cubes
		for (unsigned int i = 0; i < NUM_CUBES; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::scale(model, glm::vec3(i / 20.0f + 0.1f));
			litShader.setMat4("_Model", model);
			cubeMesh.draw();
		}

		//Draw light source as cube
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.5f));
		unlitShader.use();
		unlitShader.setMat4("_ViewProjection", projMatrix * view);
		unlitShader.setMat4("_Model", model);
		unlitShader.setVec3("_Color", lightColor);
		cubeMesh.draw();

		//Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Create a window called Settings.
		ImGui::Begin("Settings");
		ImGui::DragFloat3("Light Position", &lightPosition.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &material.ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &material.diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &material.specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &material.shininess, 2.0f, 1024);
		ImGui::End();

		//Actually render IMGUI elements using OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void processInput(GLFWwindow* window)
{
	//Only allow camera input
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
		return;
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	
	float cameraSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 2.0f;
	glm::vec3 camRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	glm::vec3 camUp = glm::normalize(glm::cross(camRight, cameraFront));

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= camRight * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += camRight * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPos -= camUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPos += camUp * cameraSpeed;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return;

	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 90.0f)
		fov = 90.0f;
}
