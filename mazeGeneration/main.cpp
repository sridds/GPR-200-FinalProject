#include <stdio.h>
#include <math.h>
#include <vector>
#include <iostream>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ew/external/stb_image.h>

#include <ew/procGen.h>
#include <ew/mesh.h>
#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/player.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

#pragma region Maze Settings
// maze global variables
const int MAZE_SIZE = 19;
int currentMazeNumber = 0;
float WALL_SIZE = 1.0f;
float WALL_HEIGHT = 1.0f;
float mazeCenterX = (((float)MAZE_SIZE) / 2.0f) * WALL_SIZE * 2.0f;
float mazeCenterZ = (((float)MAZE_SIZE) / 2.0f) * WALL_SIZE * 2.0f;
float currentMaze[19][19];

float maze1[19][19] = {
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

float maze2[19][19] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

float maze3[19][19] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};
#pragma endregion

#pragma region Function Headers
// input functions
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(std::vector<std::string> faces);

void handlePlayerMovement(GLFWwindow* window);
void handleFreecamMovement(GLFWwindow* window);

// maze functions
void updateMaze(int currentMazeNumber);
#pragma endregion

#pragma region Player and Camera variables
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
#pragma endregion

#pragma region Lighting Settings
glm::vec3 lightColor = glm::vec3(1);
struct Material {
	float ambientK = 0.65f;
	float diffuseK = 0.45f;
	float specularK = 0.5f;
	float shininess = 64.0f;
	bool blinnPhong = true;
} material;

bool wireFrame = false;
bool pointRender = false;

//Toon shading Settings
bool isToonShading = true;
int toonShadingLevels = 8;

bool isRimLighting = true;
float rimLightFalloff = 8.0;
float rimLightIntensity = 0.15;

//Fog Settings
bool isFogEnabled = true;
float fogStart = 5.0;
float fogEnd = 17.0;
float fogExponential = 2.5;
glm::vec3 fogColor = glm::vec3(0);

//Pixelation
bool isPixelationEnabled = true;
float widthPixelation = 256;
float heightPixelation = 128;
float colorPrecision = 8;

//Dithering
bool isDitherEnabled = true;
float ditherThreshold = 3.5f;
float ditherScale = 0.05f;
float texelSize = 0.1f;
int ditherIndex = 0;
float ditherNear = 1;
#pragma endregion

#pragma region Skybox Vertices
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

	#pragma region GL and ImGui Settings
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glPointSize(4.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	#pragma endregion

	#pragma region Transforms, Meshes, and Shaders
	Transform lightTransform;
	Transform planeTransform;
	Transform mazeTransform;

	// plane mesh calculations and loading
	ew::MeshData planeMeshData;
	float mazeLength = (MAZE_SIZE * (WALL_SIZE * 2)) / 3.0f;
	ew::createPlaneXY(mazeLength, mazeLength, 5, &planeMeshData);
	ew::Mesh planeMesh = ew::Mesh(planeMeshData);

	//Set Plane transform
	planeTransform.rotation = glm::vec3(1.0f, 0.0f, 0.0f);
	planeTransform.rotationAngle = 270.0f;
	// first cube is generated by an offset from true 0, 0, 0 by one length WALL_SIZE
	planeTransform.position = glm::vec3(0.0f, 0.0f, (MAZE_SIZE * WALL_SIZE * 2.0f) - WALL_SIZE);
	planeTransform.scale = glm::vec3(3.0f, 3.0f, 1.0f);

	// cube mesh initializations
	ew::MeshData cubeMeshData;
	createCube(1.0f, &cubeMeshData);
	ew::Mesh cubeMesh = ew::Mesh(cubeMeshData);

	// shaders and textures
	ew::Shader litShader = ew::Shader("assets/lit.vert", "assets/lit.frag");

	unsigned int wallTex = ew::loadTexture("assets/textures/sethWallv2.png", GL_REPEAT, GL_NEAREST);
	unsigned int floorTex = ew::loadTexture("assets/textures/sethFloorv2.png", GL_REPEAT, GL_NEAREST);
	unsigned int roofTex = ew::loadTexture("assets/textures/sethRoofv2.png", GL_REPEAT, GL_NEAREST);
	unsigned int easterEggTex = ew::loadTexture("assets/textures/seth.png", GL_REPEAT, GL_NEAREST);

	// binding textures of maze to an array for easier usage in shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wallTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, floorTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, roofTex);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, easterEggTex);

	// skybox textures
	std::vector<std::string> faces
	{
		"assets/textures/right.jpg",
		"assets/textures/left.jpg",
		"assets/textures/top.jpg",
		"assets/textures/bottom.jpg",
		"assets/textures/front.jpg",
		"assets/textures/back.jpg"
	};

	unsigned int cubemapTexture = loadCubemap(faces);
	ew::Shader skyboxShader = ew::Shader("assets/skybox.vert", "assets/skybox.frag");
	#pragma endregion

	#pragma region Easter Egg Initialization
	Transform sphereTransform;
	ew::MeshData sphereMeshData;

	ew::createSphere(2.0f, 64, &sphereMeshData);
	ew::Mesh sphereMesh = ew::Mesh(sphereMeshData);

	#pragma endregion

	#pragma region Skybox Initialization data
	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// setting skybox uniform
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	#pragma endregion

	#pragma region Preloading Variables
	litShader.use();
	// texture unit array for switching between textures
	GLint texturesLocation = glGetUniformLocation(litShader.getShaderID(), "textures");
	int textureUnits[] = {0, 1, 2, 3};
	glUniform1iv(texturesLocation, 4, textureUnits);

	// player initial position
	player.cellPos = glm::vec2(((MAZE_SIZE + 1) / 2.0f) - 1, ((MAZE_SIZE + 1) / 2.0f) - 1);
	
	// maze number for imgui menu
	int mazeNumber = 1;

	glm::mat4 projMatrix = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view;
	glm::mat4 skyView;
	#pragma endregion

#pragma region Render Loop
	while (!glfwWindowShouldClose(window)) {
		#pragma region Misc. Loop Functions
		glfwPollEvents();
		processInput(window);

		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// clear framebuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw mode selection
		ew::DrawMode drawMode = pointRender ? ew::DrawMode::POINTS : ew::DrawMode::TRIANGLES;
		#pragma endregion

		#pragma region Light Transform
		// set light to follow player
		lightTransform.position.x = cameraPos.x;
		lightTransform.position.y = cameraPos.y + lightHeightOffset;
		lightTransform.position.z = cameraPos.z;
		#pragma endregion

		#pragma region Camera Switch
		player.update(deltaTime);

		if (freeCamEnabled) 
		{
			// in freecam mode
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		} 
		else 
		{
			// in player mode
			cameraPos = glm::vec3(((player.getTransform().position.x * WALL_SIZE)), player.getTransform().position.y, ((player.getTransform().position.z) * WALL_SIZE));

			view = glm::lookAt(cameraPos, cameraPos + player.getFrontDir(), cameraUp);
		}
		#pragma endregion

		#pragma region Shader Programs
		//Set lighting settings
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
		litShader.setInt("_DitherIndex", ditherIndex);
		litShader.setFloat("_DitherNear", ditherNear);

		//Drawing plane
		litShader.setMat4("_Model", planeTransform.getModelMatrix());
		litShader.setInt("_MazeSize", MAZE_SIZE);
		// setting to floor texture
		litShader.setInt("_ActiveTexture", 1);
		planeMesh.draw(drawMode);

		// drawing "easter egg"
		litShader.setInt("_ActiveTexture", 3);
		sphereTransform.position = glm::vec3(WALL_SIZE, 1 + sin((float)glfwGetTime() * 3.0f) * 0.2f, 0.0f);
		sphereTransform.scale = glm::vec3(0.4f);
		sphereTransform.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
		sphereTransform.rotationAngle = (float)glfwGetTime() * 50.0f;

		litShader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw(drawMode);

		// setting to wall texture
		litShader.setInt("_ActiveTexture", 0);
		#pragma endregion

		#pragma region Draw Maze
		updateMaze(mazeNumber);

		// reading through tempMaze matrix
		for (int row = 0; row < MAZE_SIZE; row++)
		{
			int col = 0;
			while (col < MAZE_SIZE)
			{
				// if it is an empty space in the maze, we dont draw a square
				if (currentMaze[row][col] == 0)
				{
					col++;
					continue;
				}

				int wallCount = 1;
				// tracking wall count and ensuring we dont somehow exceed the size of the maze
				while (col + wallCount < MAZE_SIZE && currentMaze[row][col + wallCount] == 1)
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

		#pragma region Draw Skybox
		if (freeCamEnabled) 
		{
			skyView = glm::lookAt(glm::vec3(0), glm::vec3(0) + cameraFront, cameraUp);
		}
		else 
		{
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
		#pragma endregion

		#pragma region ImGui Settings
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
				ImGui::SliderFloat("Dither Scale", &ditherScale, 0, 5);
				ImGui::SliderInt("Dither Pattern", &ditherIndex, 0, 4);
				ImGui::SliderFloat("Dither Near Plane", &ditherNear, 0, 15);
			}
		}

		if (ImGui::CollapsingHeader("Fog Settings")) {
			ImGui::Checkbox("Enable Fog", &isFogEnabled);
			ImGui::SliderFloat("Fog start", &fogStart, 0, 100);
			ImGui::SliderFloat("Fog end", &fogEnd, 0, 100);
			ImGui::SliderFloat("Fog density", &fogExponential, 0.0f, 10.0f);
			ImGui::ColorEdit3("Fog Color", &fogColor.r);
		}

		if (ImGui::CollapsingHeader("Maze Settings")) {
			ImGui::SliderFloat("Wall Height", &WALL_HEIGHT, 0.2f, 3.5f);
			ImGui::SliderInt("Maze Pattern", &mazeNumber, 1, 3);
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
#pragma endregion

#pragma region Functions
bool isKey = false;

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

	const float sensitivity = 0.05f;
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

		float val = currentMaze[x][y];

		if (val == 0) {
			player.moveForward();
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		glm::vec2 cell = player.getProjectedBackwardCell();

		int x = cell.x;
		int y = cell.y;

		float val = currentMaze[x][y];

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
	float cameraSpeed = 5.0f * deltaTime;

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


	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cameraPos += cameraSpeed * camUp;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cameraPos -= cameraSpeed * camUp;
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

// updating current maze only if global maze number changes
void updateMaze(int newMazeNumber) 
{
	if (newMazeNumber != currentMazeNumber) 
	{
		switch (newMazeNumber) 
		{
		case 1:
			for (int i = 0; i < MAZE_SIZE; i++) 
			{
				for (int j = 0; j < MAZE_SIZE; j++) 
				{
					currentMaze[i][j] = maze1[i][j];
				}
			}
			break;
		case 2:
			for (int i = 0; i < MAZE_SIZE; i++) 
			{
				for (int j = 0; j < MAZE_SIZE; j++) 
				{
					currentMaze[i][j] = maze2[i][j];
				}
			}
			break;
		case 3:
			for (int i = 0; i < MAZE_SIZE; i++) 
			{
				for (int j = 0; j < MAZE_SIZE; j++) 
				{
					currentMaze[i][j] = maze3[i][j];
				}
			}
			break;
		default:
			break;
		}

		// update global
		currentMazeNumber = newMazeNumber;
		// SETH PUT RESPAWN TO CENTER OF MAZE FUNCTION HERE
	}
}
#pragma endregion