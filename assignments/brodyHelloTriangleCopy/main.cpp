#include <cstdlib>
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <ew/procGen.h>
#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/transform.h>

//Include IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int MAZE_SIZE = 19;
const float WALL_SIZE = 1.0f;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,

	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,

	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,

	-0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,
	 0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,
	 0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
};

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

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// calculating center of maze for camera start pos
float mazeCenterX = (MAZE_SIZE / 2.0f) * WALL_SIZE * 2.0f;
float mazeCenterZ = -(MAZE_SIZE / 2.0f) * WALL_SIZE * 2.0f;

// updating camera pos to be at the center of the maze
glm::vec3 cameraPos = glm::vec3(mazeCenterX, 1.0f, mazeCenterZ + WALL_SIZE);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f, pitch = 0;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 60.0f;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

Transform lightTransform;



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

bool isRimLighting = true;
float rimLightFalloff = 8.0;
float rimLightIntensity = 0.3;

//Fog Settings
float fogStart = 5.0;
float fogEnd = 100.0;
float fogExponential = 2.0;
glm::vec3 fogColor = glm::vec3(1);

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze", NULL, NULL);
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
	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	
	ew::Shader shader = ew::Shader("assets/maze.vert", "assets/maze.frag");
	
	// enabling depth testing
	glEnable(GL_DEPTH_TEST);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);

	// create random colors, temporary until we use textures
	float colValues[MAZE_SIZE][MAZE_SIZE];

	for (int j = 0; j < MAZE_SIZE; j++)
	{
		for (int i = 0; i < MAZE_SIZE; i++)
		{
			srand(rand());

			int val = rand() % 100 + 1;
			float awesomeSauce = val / 100.0f;

			colValues[i][j] = awesomeSauce;
		}
	}

	//Create Toon shader program
	ew::Shader toonShader = ew::Shader("assets/ToonShader.vert", "assets/ToonShader.frag");
	ew::Shader unlitShader = ew::Shader("assets/unlit.vert", "assets/unlit.frag");

	//Set default light transform
	lightTransform.position = glm::vec3(0.0f, 1.5f, 0.0f);
	lightTransform.rotation = glm::vec3(1);
	lightTransform.scale = glm::vec3(1);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		processInput(window);

		// Clear framebuffer with depth
		glClearColor(0.3f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float time = (float)glfwGetTime();

		glBindVertexArray(VAO);
		//shader.use();

		// create transformations
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		//shader.setMat4("view", view);
		//shader.setMat4("projection", projection);

		#pragma region Toon Shader
		toonShader.use();
		toonShader.setVec3("_ViewPos", cameraPos);
		toonShader.setMat4("_ViewProjection", projection * view);
		toonShader.setVec3("_LightColor", lightColor);
		toonShader.setVec3("_LightPos", lightTransform.position);
		toonShader.setFloat("_Material.ambientK", material.ambientK);
		toonShader.setFloat("_Material.diffuseK", material.diffuseK);
		toonShader.setFloat("_Material.specularK", material.specularK);
		toonShader.setFloat("_Material.shininess", material.shininess);
		toonShader.setInt("_Material.blinnPhong", material.blinnPhong);

		//Set Toon settings
		toonShader.setInt("_ToonShadingEnabled", isToonShading);
		toonShader.setInt("_ToonLevels", toonShadingLevels);
		toonShader.setInt("_RimLightingEnabled", isRimLighting);
		toonShader.setFloat("_RimLightFalloff", rimLightFalloff);
		toonShader.setFloat("_RimLightIntensity", rimLightIntensity);

		//Fog settings
		toonShader.setFloat("_FogStart", fogStart);
		toonShader.setFloat("_FogEnd", fogEnd);
		toonShader.setFloat("_FogExponential", fogExponential);
		toonShader.setVec3("_FogColor", fogColor);
		#pragma endregion

		//unlitShader.use();
		//unlitShader.setMat4("_ViewProjection", projection * view);
		//unlitShader.setMat4("_Model", lightTransform.getModelMatrix());
		//unlitShader.setVec3("_Color", lightColor);

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

				toonShader.setMat4("_Model", model);
				//toonShader.setFloat("colFloat", colValues[row][col]);

				glDrawArrays(GL_TRIANGLES, 0, 36);

				col += wallCount;
			}
		}

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


		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	printf("Shutting down...");

	return 0;
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

