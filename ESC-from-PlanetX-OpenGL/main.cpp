// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include "shader.hpp"

//variables
GLFWwindow* window;
const int width = 1000, height = 700;

//window limits
const float LEFT_LIMIT = -1.0f + 0.05f;
const float RIGHT_LIMIT = 1.0f - 0.05f;

//moving rectangles
const float LEFT_LIMIT_RECT = 0.3f;


//create matrices for transforms
glm::mat4 trans(1.0f);

//Callback for adjusting the viewport when resizing the window
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	glViewport(0, 0, new_width, new_height);
}

// Function to check collision
bool checkCollision(float triangleX, float rectPosX) {
	const float HALF_TRIANGLE_LENGTH = 0.05f; 
	return (triangleX + HALF_TRIANGLE_LENGTH >= rectPosX && 
		triangleX - HALF_TRIANGLE_LENGTH <= rectPosX + 0.3f);
}


int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "ESC-from-PlanetX-OpenGL", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	GLfloat vertices[] = {
		// Triangle vertices
	0.0f, 0.05f, 0.0f, // top
	-0.05f, -0.05f, 0.0f, // bottom left
	0.05f, -0.05f, 0.0f, // bottom right

	// Bottom rectangle vertices
	-1.0f, -1.0f, 0.0f,  // bottom left
	1.0f, -1.0f, 0.0f,  // bottom right
	1.0f, -0.4f, 0.0f,  // top right
	-1.0f, -0.4f, 0.0f,   // top left

	// floating rectangle1 vertices
	-0.3f, 0.7f, 0.0f,  // top left
	0.3f, 0.7f, 0.0f,  // top right
	0.3f, 0.5f, 0.0f,  // bottom right
	-0.3f, 0.5f, 0.0f,   // bottom left

	// floating rectangle2 vertices
	0.7f, 0.6f, 0.0f,  // top left
	1.0f, 0.6f, 0.0f,  // top right
	1.0f, 0.4f, 0.0f,  // bottom right
	0.7f, 0.4f, 0.0f,   // bottom left

	// camera de luat vederi vertices
	0.4f, 1.0f, 0.0f,  // top left
	0.55f, 1.0f, 0.0f,  // top right
	0.55f, 0.8f, 0.0f,  // bottom right
	0.4f, 0.8f, 0.0f   // bottom left

	};

	GLfloat verticesMoving[] = { // pozitiile dreptunghiurilor care se misca in loop
		//rect01
		0.0f, -0.4f, 0.0f,  // bottom left
		0.2f, -0.4f, 0.0f,  // bottom right
		0.2f, -0.3f, 0.0f,  // top right
		0.0f, -0.3f, 0.0f,   // top left
	};

	glm::vec3 positions[] = {
		glm::vec3(0.1f,  0.0f,  0.0f),
		glm::vec3(0.4f,  0.0f, 0.0f),
		glm::vec3(0.7f, 0.0f, 0.0f)
	};

	GLuint indices[] = {
	// Triangle indices
	0, 1, 2, //Triangle

	// Bottom rectangle indices
	3, 4, 6,  // First triangle
	4, 5, 6,   // Second triangle

	// floating rectangle1
	7, 8, 10,  // First triangle
	8, 10, 9,   // Second triangle

	// floating rectangle2
	11, 12, 14,  // First triangle
	12, 14, 13,   // Second triangle

	// floating rectangle2
	15, 16, 18,  // First triangle
	16, 18, 17   // Second triangle
	};

	GLuint indicesMoving[] = {
	//rect01
	0, 1, 3,  // First triangle
	1, 3, 2,   // Second triangle
	};

	GLuint vbo1, vao1, ibo1,
		vbo2, vao2, ibo2;

	//Bindings for cubes
	glGenVertexArrays(1, &vao1);
	glGenBuffers(1, &vbo1);
	glGenBuffers(1, &ibo1);

	glBindVertexArray(vao1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//Unbinding VAO
	glBindVertexArray(0);

	//Bindings for pyramid
	glGenVertexArrays(1, &vao2);
	glGenBuffers(1, &vbo2);
	glGenBuffers(1, &ibo2);

	glBindVertexArray(vao2);

	glBindBuffer(GL_ARRAY_BUFFER, vbo2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesMoving), verticesMoving, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesMoving), indicesMoving, GL_STATIC_DRAW);

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),                  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//Unbinding VAO
	glBindVertexArray(0);

	//Callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);

	// Set up transformation and initial position
	glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f)); //modify the second value for y-coord
	float currentX = LEFT_LIMIT; // Track the square's current X position

	//calculate delta time 
	float lastTime = 0.0f;
	float deltaTime = 0.0f;
	int k = 0;

	// Check if the window was closed
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
	{


		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		deltaTime = glfwGetTime() - lastTime;

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
			//lastTime = glfwGetTime();
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
		unsigned int transformLoc2 = glGetUniformLocation(programID, "color");
		glm::vec4 color = glm::vec4(0.5f, 0.0f, 0.0f, 1.0f); //dark red
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color));
		glBindVertexArray(0);
		glBindVertexArray(vao1);

		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0); //triangle

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // No transformations
		glm::vec4 color2 = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
		glUniform4fv(transformLoc2, 1, glm::value_ptr(color2));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(3 * sizeof(GLuint))); // Next 6 indices for the rectangle

		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); // No transformations
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(9 * sizeof(GLuint))); // Next 6 indices for the rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(15 * sizeof(GLuint))); // Next 6 indices for the rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(21 * sizeof(GLuint))); // Next 6 indices for camera de luat vederi

		//k=0 -> 0.1-0.3
		//k=1 -> 0.4-0.6
		// k=2 -> 0.7-0.9

		if (k == 0 && currentX >= 0.1f && currentX <= 0.3f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		else if (k == 1 && currentX >= 0.4f && currentX <= 0.6f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		else if (k == 2 && currentX >= 0.7f && currentX <= 0.9f) {
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			currentX = LEFT_LIMIT;
		}
		

		glBindVertexArray(0);
	}

		//glm::vec4 color3 = glm::vec4(0.545f, 0.271f, 0.075f, 1.0f); brown for boxes


	// Cleanup
	glDeleteBuffers(1, &vbo1);
	glDeleteBuffers(1, &vbo2);
	glDeleteBuffers(1, &ibo1);
	glDeleteBuffers(1, &ibo2);
	glDeleteVertexArrays(1, &vao1);
	glDeleteVertexArrays(1, &vao2);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}