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

#include "../core/ew/transform.h"

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;
const int MAZE_SIZE = 5;

float vertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

float tempMaze[MAZE_SIZE][MAZE_SIZE] = {
	{1, 1, 0, 1, 1},
	{1, 1, 0, 1, 1},
	{0, 0, 0, 0, 0},
	{1, 1, 0, 1, 1},
	{1, 1, 0, 1, 1}
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
	
	ew::Shader shader = ew::Shader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &EBO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);
	glEnableVertexAttribArray(0);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		//Clear framebuffer
		glClearColor(0.3f, 0.4f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		const float time = (float)glfwGetTime();

		glBindVertexArray(VAO);
		shader.use();
		shader.setFloat("_Time", time); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		for (int i = 0; i < MAZE_SIZE; i++)
		{
			for (int j = 0; j < MAZE_SIZE; j++)
			{
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}

		glfwSwapBuffers(window);
	}

	printf("Shutting down...");
}

