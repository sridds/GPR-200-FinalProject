#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <ew/external/stb_image.h>

#include <ew/procGen.h>
#include <ew/mesh.h>
#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/player.h>

#include <glm/gtc/matrix_transform.hpp>

// Include IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 2000;
const int SCREEN_HEIGHT = 1500;

#pragma region Maze Settings
const int MAZE_SIZE = 19;
float WALL_SIZE = 1.0f;
float WALL_HEIGHT = 1.0f;

float tempMaze[19][19] = {
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
float mazeCenterX = (((float)MAZE_SIZE) / 2.0f) * WALL_SIZE * 2.0f;
float mazeCenterZ = (((float)MAZE_SIZE) / 2.0f) * WALL_SIZE * 2.0f;
#pragma endregion

// function identifiers
// input functions
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(std::vector<std::string> faces);

void handlePlayerMovement(GLFWwindow* window);
void handleFreecamMovement(GLFWwindow* window);

// mesh functions
void createCubeMaze(float size, ew::MeshData* mesh);
static void createCubeFaceMaze(const glm::vec3& normal, float size, ew::MeshData* mesh);

// Player
Player player = Player(0.4f, glm::vec3(mazeCenterX, 1.0f, mazeCenterZ - (WALL_SIZE)));

// Free cam
glm::vec3 cameraPos = glm::vec3(mazeCenterX, 1.0f, mazeCenterZ + WALL_SIZE);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool freeCamEnabled = false;


float yaw = -90.0f, pitch = 0;
float lastX = 400, lastY = 300;
bool firstMouse = true;
float fov = 60.0f;
float lightHeightOffset = 0.0f;

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
	float ambientK = 0.35f;
	float diffuseK = 0.35f;
	float specularK = 0.5f;
	float shininess = 64.0f;
	bool blinnPhong = true;
} material;

bool wireFrame = false;
bool pointRender = false;

//Toon shading Settings
bool isToonShading = true;
int toonShadingLevels = 8;

bool isRimLighting = false;
float rimLightFalloff = 8.0;
float rimLightIntensity = 0.3;

//Fog Settings
bool isFogEnabled = true;
float fogStart = 5.0;
float fogEnd = 15.0;
float fogExponential = 3.0;
glm::vec3 fogColor = glm::vec3(0);

//Pixelation
bool isPixelationEnabled = true;
float widthPixelation = 256;
float heightPixelation = 128;
float colorPrecision = 8;

//Dithering
bool isDitherEnabled = true;
float ditherThreshold = 3.5f;
float ditherScale = 0.1f;
float texelSize = 0.03f;

// Skybox
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

#pragma endregion

int main() {
	//setup player
	player.cellPos = glm::vec2(((MAZE_SIZE + 1) / 2.0f) - 1, ((MAZE_SIZE + 1) / 2.0f) - 1);

	#pragma region Window Creation
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Generation", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//AFTER gladLoadGL

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// temp meshes not yet dealt with
	ew::MeshData planeMeshData;
	ew::MeshData sphereMeshData;
	// plane sits under maze, set to the size of the maze
	float mazeLength = (MAZE_SIZE * (WALL_SIZE * 2)) / 3.0f;
	ew::createPlaneXY(mazeLength, mazeLength, 5, &planeMeshData);
	ew::createSphere(2.0f, 64, &sphereMeshData);
	ew::Mesh planeMesh = ew::Mesh(planeMeshData);
	ew::Mesh sphereMesh = ew::Mesh(sphereMeshData);

	// cube mesh initializations
	ew::MeshData cubeMeshData;
	createCube(1.0f, &cubeMeshData);
	ew::Mesh cubeMesh = ew::Mesh(cubeMeshData);

	// shaders and textures
	ew::Shader litShader = ew::Shader("assets/lit.vert", "assets/lit.frag");

	unsigned int wallTex = ew::loadTexture("assets/sethWall.png", GL_REPEAT, GL_NEAREST);
	unsigned int floorTex = ew::loadTexture("assets/sethFloor.png", GL_REPEAT, GL_NEAREST);
	unsigned int roofTex = ew::loadTexture("assets/sethRoof.png", GL_REPEAT, GL_NEAREST);

	// Skybox
	std::vector<std::string> faces
	{
		"assets/right.jpg",
		"assets/left.jpg",
		"assets/top.jpg",
		"assets/bottom.jpg",
		"assets/front.jpg",
		"assets/back.jpg"
	};

	unsigned int cubemapTexture = loadCubemap(faces);
	ew::Shader skyboxShader = ew::Shader("assets/skybox.vert", "assets/skybox.frag");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPointSize(4.0);

	//Set Plane transform
	planeTransform.rotation = glm::vec3(1.0f, 0.0f, 0.0f);
	planeTransform.rotationAngle = 90.0f;
	// first cube is generated by an offset from true 0, 0, 0 by one length WALL_SIZE
	planeTransform.position = glm::vec3(0.0f, 0.0f, -WALL_SIZE);
	planeTransform.scale = glm::vec3(3.0f, 3.0f, 1.0f);

	// binding textures to an array for easier usage in shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, roofTex);

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	litShader.use();

	GLint texturesLocation = glGetUniformLocation(litShader.getShaderID(), "textures");
	int textureUnits[] = {0, 1, 2};
	glUniform1iv(texturesLocation, 3
		, textureUnits);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		mazeCenterX = (MAZE_SIZE / 2.0f) * WALL_SIZE * 2.0f;
		mazeCenterZ = ((MAZE_SIZE / 2.0f) - 1) * WALL_SIZE * 2.0f;

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

		player.update(deltaTime);
		ew::DrawMode drawMode = pointRender ? ew::DrawMode::POINTS : ew::DrawMode::TRIANGLES;
		glm::mat4 view;

		if (freeCamEnabled) {
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		} 
		else 
		{
			cameraPos = glm::vec3(((player.getTransform().position.x * WALL_SIZE)), player.getTransform().position.y, ((player.getTransform().position.z) * WALL_SIZE));

			view = glm::lookAt(cameraPos, cameraPos + player.getFrontDir(), cameraUp);
		}

		glm::mat4 projMatrix = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);

		#pragma region Shader Programs
		//Draw light source as cube
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

		//Set Pixelation settings
		litShader.setInt("_PixelationEnabled", isPixelationEnabled);
		litShader.setFloat("_WidthPixelation", widthPixelation);
		litShader.setFloat("_HeightPixelation", heightPixelation);
		litShader.setFloat("_ColorPrecision", colorPrecision);

		//Fog settings
		litShader.setInt("_FogEnabled", isFogEnabled);
		litShader.setFloat("_FogStart", fogStart);
		litShader.setFloat("_FogEnd", fogEnd);
		litShader.setFloat("_FogExponential", fogExponential);
		litShader.setVec3("_FogColor", fogColor);

		//Set Dither settings
		litShader.setInt("_DitherEnabled", isDitherEnabled);
		litShader.setFloat("_DitherThreshold", ditherThreshold);
		litShader.setFloat("_DitherScale", ditherScale);
		litShader.setFloat("_TexelSize", texelSize);

		//Draw plane
		litShader.setMat4("_Model", planeTransform.getModelMatrix());
		litShader.setInt("_MazeSize", MAZE_SIZE);
		// setting to floor texture
		litShader.setInt("_ActiveTexture", 1);
		planeMesh.draw(drawMode);
		// setting to wall texture
		litShader.setInt("_ActiveTexture", 0);

		// draw sphere
		sphereTransform.position = glm::vec3(0.0f, 8.0f, 0.0f);
		sphereTransform.scale = glm::vec3(0.75f);
		litShader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw(drawMode);
		#pragma endregion

		// if wall size is changed, scale the plane with it
		planeTransform.scale = glm::vec3(3.0f * WALL_SIZE, 3.0f * WALL_SIZE, 1.0f);

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
				float centerZ = row * WALL_SIZE * 2.0f;

				// create model matrix
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(centerX, WALL_SIZE * WALL_HEIGHT, centerZ + (1 - WALL_SIZE)));
				model = glm::scale(model, glm::vec3(wallCount, WALL_HEIGHT, 1.0f) * WALL_SIZE * 2.0f);

				litShader.setMat4("_Model", model);
				litShader.setInt("_WallCount", wallCount);

				// using winebrenner mesh draws
				cubeMesh.draw(drawMode);

				col += wallCount;
			}
		}
		#pragma endregion

		glm::mat4 skyView;
		if (freeCamEnabled) {
			skyView = glm::lookAt(glm::vec3(0), glm::vec3(0) + cameraFront, cameraUp);
		}
		else {
			skyView = glm::lookAt(glm::vec3(0), glm::vec3(0) + player.getFrontDir(), cameraUp);
		}
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		skyboxShader.setMat4("view", skyView);
		skyboxShader.setMat4("projection", projMatrix);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

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

		if (ImGui::CollapsingHeader("Shading Settings")) {
			if (ImGui::CollapsingHeader("Toon & Rimlight Shading")) {
				ImGui::Checkbox("Enable Toon Shading", &isToonShading);
				ImGui::SliderInt("Toon Levels", &toonShadingLevels, 2.0f, 64.0f);
				ImGui::Checkbox("Enable Rim Lighting", &isRimLighting);
				ImGui::SliderFloat("Rim Lighting Falloff", &rimLightFalloff, 0.0f, 64.0f);
				ImGui::SliderFloat("Rim Light Intensity", &rimLightIntensity, 0.0f, 5.0f);
			}
			if (ImGui::CollapsingHeader("Pixelation & Dither Shading")) {
				ImGui::Checkbox("Enabled Pixelation", &isPixelationEnabled);
				ImGui::InputFloat("Pixelation Width", &widthPixelation);
				ImGui::InputFloat("Pixelation Height", &heightPixelation);
				ImGui::InputFloat("Color Precision", &colorPrecision, 1, 256);
				ImGui::Checkbox("Enable Dithering", &isDitherEnabled);
				ImGui::InputFloat("Dither Threshold", &ditherThreshold, 0.1f);
				ImGui::InputFloat("Dither Scale", &ditherScale, 0.1f);
				ImGui::InputFloat("Texel Size", &texelSize, 0.1f);
			}
		}

		if (ImGui::CollapsingHeader("Fog Settings")) {
			ImGui::Checkbox("Enable Fog", &isFogEnabled);
			ImGui::InputFloat("Fog start", &fogStart);
			ImGui::InputFloat("Fog end", &fogEnd);
			ImGui::SliderFloat("Fog density", &fogExponential, 0.0f, 10.0f);
			ImGui::ColorEdit3("Fog Color", &fogColor.r);
		}

		if (ImGui::CollapsingHeader("Maze Settings")) {
			ImGui::SliderFloat("Wall Height", &WALL_HEIGHT, 0.2f, 3.5f);
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

bool isKey = false;

#pragma region Input Functions
void processInput(GLFWwindow* window)
{
	// Swap modes
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !isKey) {
		freeCamEnabled = !freeCamEnabled;

		if (freeCamEnabled) {
			cameraPos = player.getTransform().position;
			//cameraPos = glm::vec3(((player.getTransform().position.x * WALL_SIZE)), player.getTransform().position.y, ((player.getTransform().position.z + (WALL_SIZE)) * WALL_SIZE));

			cameraFront = player.getFrontDir();
			
			lastX = 0.0f;
			lastY = 0.0f;

			pitch = 0.0f;
			yaw = player.getYaw();

			firstMouse = true;
		}

		isKey = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) != GLFW_PRESS) {
		isKey = false;
	}

	//Only allow camera input
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) && freeCamEnabled) {
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
	glm::vec3 camUp = glm::normalize(glm::cross(camRight, player.getFrontDir()));

	if (!freeCamEnabled) handlePlayerMovement(window);
	else handleFreecamMovement(window);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL || !freeCamEnabled)
		return;

	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
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

void handlePlayerMovement(GLFWwindow* window) {
	if (player.isMoving() || player.isTurning()) return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		glm::vec2 cell = player.getProjectedForwardCell();

		int x = cell.x;
		int y = cell.y;

		float val = tempMaze[x][y];

		if (val == 0) {
			player.moveForward();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		glm::vec2 cell = player.getProjectedBackwardCell();

		int x = cell.x;
		int y = cell.y;

		float val = tempMaze[x][y];

		if (val == 0) {
			player.moveBackward();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		player.turnLeft();
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		player.turnRight();
	}
}

void handleFreecamMovement(GLFWwindow* window) {
	float cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 2.0f;

	glm::vec3 camRight = glm::normalize(glm::cross(cameraFront, cameraUp));
	glm::vec3 camUp = glm::normalize(glm::cross(camRight, cameraFront));


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		cameraPos += cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * camRight;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cameraPos += cameraSpeed * camRight;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 90.0f)
		fov = 90.0f;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(false);

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
#pragma endregion