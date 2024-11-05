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

float vertices[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	+1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	+1.0f, +1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, +1.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};

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
	
	ew::Shader bgShader = ew::Shader("assets/bg.vert", "assets/bg.frag");
	ew::Shader charShader = ew::Shader("assets/character.vert", "assets/character.frag");
	unsigned int brickTex = ew::loadTexture("assets/brick.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	unsigned int decalTex = ew::loadTexture("assets/decal.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
	unsigned int charTex = ew::loadTexture("assets/character.png", GL_REPEAT, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, brickTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, decalTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, charTex);
	//QUAD MESH
	unsigned int VAO, VBO, EBO;
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)0);
		glEnableVertexAttribArray(0);
		//UV attribute
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(1);
	}
	
	//Enable blending w/ transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		const float time = (float)glfwGetTime();

		glBindVertexArray(VAO);

		//Draw background
		bgShader.use();
		bgShader.setFloat("_Time", time);
		bgShader.setInt("_MainTex", 0);
		bgShader.setInt("_DecalTex", 1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Draw character
		charShader.use();
		charShader.setFloat("_Time", time);
		charShader.setInt("_MainTex", 2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

