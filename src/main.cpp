#include <cstdint>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaders.h"
#include "utils.h"

namespace fs = std::filesystem;

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
#ifdef __APPLE__
	glViewport(0, 0, width * 2, height * 2);
#else
    glViewport(0, 0, width, height);
#endif
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
        // set the screen to a static color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		shader.bind();
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // exit glfw
    glfwTerminate();

    return 0;
}
