#include <cstdint>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shaders.h"
#include "utils.h"

namespace fs = std::filesystem;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

Camera camera(glm::vec3{0.0f, 0.0f, 3.0f});
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
#ifdef __APPLE__
	glViewport(0, 0, width * 2, height * 2);
#else
    glViewport(0, 0, width, height);
#endif
}

void mouseCallback(GLFWwindow *window, double xPosIn, double yPosIn) {
    float xPos = static_cast<float>(xPosIn);
    float yPos = static_cast<float>(yPosIn);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.rotateCamera(xOffset, yOffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.movePosition(Movement::Forward, deltaTime);
    } 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.movePosition(Movement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.movePosition(Movement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.movePosition(Movement::Right, deltaTime);
    }
}

void scrollCallback(GLFWwindow *window, double _, double yOffset) {
    camera.zoomCamera(static_cast<float>(yOffset));
}

int main() {
    // initialize glfw library
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create glfw window
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Test Renderer", nullptr, nullptr);
    if (!window) {
        std::cout << "glfwCreateWindow() failed to create a window\n";
        glfwTerminate();
        return -1;
    }

    // set opengl context
    glfwMakeContextCurrent(window);

    // get opengl function addresses using glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "gladLoadGLLoader() failed to load OpenGL functions\n";
        glfwTerminate();
        return -1;
    }

    // set opengl viewport
	// apple displays that use retina have double resolution
	// TODO: detect retina display or get window size from glfw
#ifdef __APPLE__
	glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
#else
    glViewport(0, 0, WIDTH, HEIGHT);
#endif

    // set glfw callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
	
	// triangle vertices and indices
	constexpr float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	};
	constexpr uint32_t indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// setup VAO
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// transfer vertices to VBO
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// setup EBO
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// set shader attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// compile and link shader program
	std::string vertexSource = utils::fileReadString(fs::path{"shaders/basic.vs"});
	std::string fragmentSource = utils::fileReadString(fs::path{"shaders/basic.fs"});
	Shader shader = Shader::createProgram(vertexSource, fragmentSource);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // set the screen to a static color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw triangle
		shader.bind();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        glm::mat4 view = camera.getViewMatrix();
        shader.setMat4("view", view);
		glBindVertexArray(vao);
        glm::mat4 model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3{0.0f, 0.0f, 0.0f});
        shader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // exit glfw
    glfwTerminate();

    return 0;
}
