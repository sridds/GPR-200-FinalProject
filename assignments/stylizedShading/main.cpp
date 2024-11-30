#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ew/transform.h>
#include <ew/procGen.h>
#include <ew/shader.h>
#include <ew/texture.h>

#include <glm/gtc/matrix_transform.hpp>

// Include IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

#pragma region Maze Settings
const int MAZE_SIZE = 19;
const float WALL_SIZE = 1.0f;

float tempMaze[MAZE_SIZE][MAZE_SIZE] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1},
	{1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// calculating center of maze for camera start pos
float mazeCenterX = (MAZE_SIZE / 2.0f) * WALL_SIZE * 2.0f;
float mazeCenterZ = -(MAZE_SIZE / 2.0f) * WALL_SIZE * 2.0f;
#pragma endregion

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//CAMERA
glm::vec3 cameraPos = glm::vec3(mazeCenterX, 1.0f, mazeCenterZ + WALL_SIZE);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f, pitch = 0;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 60.0f;
float lightHeightOffset = 3.0f;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float orthoHeight = 10.0f;

Transform lightTransform;
Transform planeTransform;
Transform sphereTransform;
Transform mazeTransform;

#pragma region Lighting Settings
glm::vec3 lightColor = glm::vec3(1);
struct Material {
	float ambientK = 0.1f;
	float diffuseK = 0.5f;
	float specularK = 0.5f;
	float shininess = 64.0f;
	bool blinnPhong = true;
} material;

bool wireFrame = false;
bool pointRender = false;

//Toon shading Settings
bool isToonShading = true;
int toonShadingLevels = 4;

bool isRimLighting = false;
float rimLightFalloff = 8.0;
float rimLightIntensity = 0.3;

//Fog Settings
float fogStart = 5.0;
float fogEnd = 100.0;
float fogExponential = 2.0;
glm::vec3 fogColor = glm::vec3(1);
#pragma endregion

int main() {
	#pragma region Window Creation
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
	#pragma endregion

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//AFTER gladLoadGL
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Initialization goes here!
	ew::MeshData cubeMeshData;
	ew::MeshData planeMeshData;
	ew::MeshData sphereMeshData;
	ew::createCube(1.0f, &cubeMeshData);
	ew::createPlaneXY(10.0f, 10.0f, 5, &planeMeshData);
	ew::createSphere(2.0f, 64, &sphereMeshData);
	ew::Mesh cubeMesh = ew::Mesh(cubeMeshData);
	ew::Mesh planeMesh = ew::Mesh(planeMeshData);
	ew::Mesh sphereMesh = ew::Mesh(sphereMeshData);

	ew::Shader litShader = ew::Shader("assets/lit.vert", "assets/lit.frag");
	ew::Shader unlitShader = ew::Shader("assets/unlit.vert", "assets/unlit.frag");

	unsigned int brickTex = ew::loadTexture("assets/brick.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	unsigned int wallTex = ew::loadTexture("assets/undergroundBlock.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPointSize(4.0);

	//Set Plane transform
	planeTransform.rotation = glm::vec3(1.0f, 0.0f, 0.0f);
	planeTransform.rotationAngle = -90.0f;
	planeTransform.position = glm::vec3(0.0f);
	planeTransform.scale = glm::vec3(3.0f, 3.0f, 1.0f);


	glBindTexture(GL_TEXTURE_2D, wallTex);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		processInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set light to follow player
		lightTransform.position.x = cameraPos.x;
		lightTransform.position.y = cameraPos.y + lightHeightOffset;
		lightTransform.position.z = cameraPos.z;

		// Light rotation
		lightTransform.rotation = glm::vec3(1.0, 0.0, 1.0);
		lightTransform.rotationAngle = glfwGetTime() * 20.0f;

		ew::DrawMode drawMode = pointRender ? ew::DrawMode::POINTS : ew::DrawMode::TRIANGLES;

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projMatrix = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

		#pragma region Shader Programs
		//Draw light source as cube
		unlitShader.use();
		unlitShader.setMat4("_ViewProjection", projMatrix * view);
		unlitShader.setMat4("_Model", lightTransform.getModelMatrix());
		unlitShader.setVec3("_Color", lightColor);
		cubeMesh.draw(drawMode);

		litShader.use();
		litShader.setVec3("_ViewPos", cameraPos);
		litShader.setMat4("_ViewProjection", projMatrix * view);
		litShader.setVec3("_LightColor", lightColor);
		litShader.setVec3("_LightPos", lightTransform.position);
		litShader.setFloat("_Material.ambientK", material.ambientK);
		litShader.setFloat("_Material.diffuseK", material.diffuseK);
		litShader.setFloat("_Material.specularK", material.specularK);
		litShader.setFloat("_Material.shininess", material.shininess);
		litShader.setInt("_Material.blinnPhong", material.blinnPhong);

		//Set Toon settings
		litShader.setInt("_ToonShadingEnabled", isToonShading);
		litShader.setInt("_ToonLevels", toonShadingLevels);
		litShader.setInt("_RimLightingEnabled", isRimLighting);
		litShader.setFloat("_RimLightFalloff", rimLightFalloff);
		litShader.setFloat("_RimLightIntensity", rimLightIntensity);

		//Fog settings
		litShader.setFloat("_FogStart", fogStart);
		litShader.setFloat("_FogEnd", fogEnd);
		litShader.setFloat("_FogExponential", fogExponential);
		litShader.setVec3("_FogColor", fogColor);

		//Draw plane
		litShader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw(drawMode);

		// draw sphere
		sphereTransform.position = glm::vec3(5.0f, 3.0f, 1.0f);
		sphereTransform.scale = glm::vec3(1.0f);
		litShader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw(drawMode);
		#pragma endregion


		#pragma region Draw Maze
		// reading through tempMaze matrix
		for (int row = 0; row < MAZE_SIZE; row++)
		{
			int col = 0;
			while (col < MAZE_SIZE)
			{
				// if it is an empty space in the maze, we dont draw a square
				if (tempMaze[row][col] == 0)
				{
					col++;
					continue;
				}

				int wallCount = 1;
				// tracking wall count and ensuring we dont somehow exceed the size of the maze
				while (col + wallCount < MAZE_SIZE && tempMaze[row][col + wallCount] == 1)
				{
					wallCount++;
				}

				// adjust scaling and positioning
				float centerX = (col + wallCount / 2.0f) * WALL_SIZE * 2.0f;
				float centerY = -row * WALL_SIZE * 2.0f;
				float centerZ = 1.0f;

				// create model matrix
				glm::mat4 model = glm::mat4(1.0f);
				// rotating around x axis to lay maze flat
				model = glm::translate(model, glm::vec3(centerX, 1.0f, centerY));
				model = glm::scale(model, glm::vec3(wallCount, 1.0f, 1.0f) * WALL_SIZE * 2.0f);

				litShader.setMat4("_Model", model);

				// using winebrenner mesh draws
				cubeMesh.draw(drawMode);

				col += wallCount;
			}
		}
		#pragma endregion

		#pragma region ImGui
		//Start drawing ImGUI
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Create a window called Settings.
		ImGui::Begin("Settings");

		if (ImGui::CollapsingHeader("Light Settings")) {
			ImGui::DragFloat3("Light Position", &lightTransform.position.x, 0.1f);
			ImGui::ColorEdit3("Light Color", &lightColor.r);
			ImGui::SliderFloat("Ambient K", &material.ambientK, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse K", &material.diffuseK, 0.0f, 1.0f);
			ImGui::SliderFloat("Specular K", &material.specularK, 0.0f, 1.0f);
			ImGui::SliderFloat("Shininess", &material.shininess, 2.0f, 1024);
		}

		if (ImGui::CollapsingHeader("Drawing Settings")) {
			if (ImGui::Checkbox("Wireframe", &wireFrame)) {
				glPolygonMode(GL_FRONT_AND_BACK, wireFrame ? GL_LINE : GL_FILL);
			}
			ImGui::Checkbox("Draw as Points", &pointRender);
		}

		if (ImGui::CollapsingHeader("Toon Shading Settings")) {
			ImGui::Checkbox("Enable Toon Shading", &isToonShading);
			ImGui::SliderInt("Toon Levels", &toonShadingLevels, 2.0f, 64.0f);
			ImGui::Checkbox("Enable Rim Lighting", &isRimLighting);
			ImGui::SliderFloat("Rim Lighting Falloff", &rimLightFalloff, 0.0f, 64.0f);
			ImGui::SliderFloat("Rim Light Intensity", &rimLightIntensity, 0.0f, 5.0f);
		}

		if (ImGui::CollapsingHeader("Fog Settings")) {
			ImGui::InputFloat("Fog start", &fogStart);
			ImGui::InputFloat("Fog end", &fogEnd);
			ImGui::SliderFloat("Fog density", &fogExponential, 0.0f, 10.0f);
			ImGui::ColorEdit3("Fog Color", &fogColor.r);
		}
		ImGui::End();

		//Actually render IMGUI elements using OpenGL
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		#pragma endregion

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	printf("Shutting down...");

	return 0;
}

#pragma region Input Functions
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
	
	float cameraSpeed = 5.0f * deltaTime;
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
#pragma endregion