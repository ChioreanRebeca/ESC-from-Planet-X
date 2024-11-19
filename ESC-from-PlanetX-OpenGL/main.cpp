#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "dependente\glew\glew.h"
#include "dependente\glfw\glfw3.h"
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"
#include "shader.hpp"

GLFWwindow* window;
const int width = 1000, height = 700;
const float LEFT_LIMIT = -1.0f + 0.05f;
const float RIGHT_LIMIT = 1.0f - 0.05f;
glm::mat4 trans(1.0f);

void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}


int main(void)
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	window = glfwCreateWindow(width, height, "ESC-from-PlanetX-OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, width, height);

	//background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat vertices[] = {
	//triangle vertices
	0.0f, 0.05f, 0.0f,
	-0.05f, -0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,

	//bottom rectangle
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, -0.4f, 0.0f,
	-1.0f, -0.4f, 0.0f,

	//floating rectangle1
	-0.3f, 0.7f, 0.0f,
	0.3f, 0.7f, 0.0f,
	0.3f, 0.5f, 0.0f,
	-0.3f, 0.5f, 0.0f,

	//floating rectangle2
	0.7f, 0.6f, 0.0f,
	1.0f, 0.6f, 0.0f,
	1.0f, 0.4f, 0.0f,
	0.7f, 0.4f, 0.0f,

	//surveilance camera rect
	0.4f, 1.0f, 0.0f,
	0.55f, 1.0f, 0.0f,
	0.55f, 0.8f, 0.0f,
	0.4f, 0.8f, 0.0f,

	//surveilance camera triangle
	0.475f, 0.9f, 0.0f,
	0.55f, 0.8f, 0.0f,
	0.4f, 0.8f, 0.0f
	};

	GLfloat verticesMoving[] = {
		0.0f, -0.4f, 0.0f,
		0.2f, -0.4f, 0.0f,
		0.2f, -0.35f, 0.0f,
		0.0f, -0.35f, 0.0f
	};

	glm::vec3 positions[] = {
		glm::vec3(0.1f,  0.0f,  0.0f),
		glm::vec3(0.4f,  0.0f, 0.0f),
		glm::vec3(0.7f, 0.0f, 0.0f)
	};

	GLuint indices[] = {
	//triangle
	0, 1, 2,

	//bottom rectangle
	3, 4, 6,
	4, 5, 6,

	//floating rectangle1
	7, 8, 10,
	8, 10, 9,

	//floating rectangle2
	11, 12, 14,
	12, 14, 13,

	//surveilance camera square
	15, 16, 18,
	16, 18, 17,

	//surveilance camera triangle
	19, 20, 21
	};

	GLuint indicesMoving[] = {
	0, 1, 3, 
	1, 3, 2,
	};

	GLuint vbo1, vao1, ibo1,
		vbo2, vao2, ibo2;

	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	glBindVertexArray(vao1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	//for moving rectangles
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	glBindVertexArray(vao2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesMoving), verticesMoving, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesMoving), indicesMoving, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	glfwSetFramebufferSizeCallback(window, window_callback);

	// translate triangle to LEFT_LIMIT
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
	float currentX = LEFT_LIMIT;

	float lastTime = 0.0f;
	float deltaTime = 0.0f;
	int k = 0;

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{

		glfwSwapBuffers(window);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(programID);

		deltaTime = glfwGetTime() - lastTime;

		//moving rectangles loop
		if (deltaTime > 1.0f) {
			lastTime = glfwGetTime();
			glBindVertexArray(vao2);
			glm::mat4 model;
			model = glm::translate(model, positions[k]);
			glm::vec4 colorRect = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			unsigned int transformLocation = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
			unsigned int transformColor = glGetUniformLocation(programID, "color");
			glUniform4fv(transformColor, 1, glm::value_ptr(colorRect));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
			k++;
			if (k == 3) k = 0;
		} else {
			glBindVertexArray(vao2);
			glm::mat4 model;
			model = glm::translate(model, positions[k]);
			glm::vec4 colorRect = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
			unsigned int transformLocation = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
			unsigned int transformColor = glGetUniformLocation(programID, "color");
			glUniform4fv(transformColor, 1, glm::value_ptr(colorRect));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		}
		

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && currentX > LEFT_LIMIT) {
			trans = glm::translate(trans, glm::vec3(-0.0005f, 0.0f, 0.0f));
			currentX -= 0.0005f;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && currentX < RIGHT_LIMIT) {
			trans = glm::translate(trans, glm::vec3(0.0005f, 0.0f, 0.0f));
			currentX += 0.0005f;
		}


		unsigned int transformLoc = glGetUniformLocation(programID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		unsigned int transformColor = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f);
		glUniform4fv(transformColor, 1, glm::value_ptr(color));
		glBindVertexArray(0);
		glBindVertexArray(vao1);

		//triangle
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glm::vec4 color2 = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glUniform4fv(transformColor, 1, glm::value_ptr(color2)); 

		//bottom rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint)));
		
		//floating rectangles
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint)));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(15 * sizeof(GLuint)));

		//surveilance camera
		glm::vec4 cameraColor1 = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
		glm::vec4 cameraColor2 = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glUniform4fv(transformColor, 1, glm::value_ptr(cameraColor1));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(21 * sizeof(GLuint)));
		glUniform4fv(transformColor, 1, glm::value_ptr(cameraColor2));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(27 * sizeof(GLuint)));

		//collision check
		if (k == 0 && currentX >= 0.06f && currentX <= 0.3f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		else if (k == 1 && currentX >= 0.3f && currentX <= 0.6f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		else if (k == 2 && currentX >= 0.6f && currentX <= 0.9f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		glBindVertexArray(0);
	}

	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ibo1);
	glDeleteBuffers(1, &ibo2);
	glDeleteVertexArrays(1, &vao1);
	glDeleteVertexArrays(1, &vao2);
	glDeleteProgram(programID);

	glfwTerminate();

	return 0;
}