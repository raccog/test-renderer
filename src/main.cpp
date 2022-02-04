#include <cstdint>
#include <filesystem>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "spdlog/spdlog.h"

#include "camera.h"
#include "shaders.h"
#include "utils.h"

namespace fs = std::filesystem;

// Screen size
constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

/*
 *  Globals
 */

// Class globals
Camera camera(glm::vec3{0.0f, 0.0f, 3.0f});
Shader shader(0);
Shader lightingShader(0);
glm::vec3 lightPos(2.0f, 2.0f, -2.0f);

// Mouse globals
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// Time globals
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/*
 *  Callbacks
 */

// Called when window is resized by used
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    spdlog::debug("Window resized. Setting OpenGL viewport");
#ifdef __APPLE__
	glViewport(0, 0, width * 2, height * 2);
#else
    glViewport(0, 0, width, height);
#endif
}

// Called when the mouse is moved
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

    camera.rotate(xOffset, yOffset);
}

void scrollCallback(GLFWwindow *window, double _, double yOffset) {
    camera.zoom(static_cast<float>(yOffset));
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Close window on ESC
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true);
        }

        // Recompile shaders on R
        if (key == GLFW_KEY_R) {
            // delete old shaders
            spdlog::debug("Recompiling shader programs");
            shader.deleteShader();
			lightingShader.deleteShader();
            
            // recompile shaders
            std::string vertexSource = utils::fileReadString(fs::path{"shaders/basic.vs"});
            std::string fragmentSource = utils::fileReadString(fs::path{"shaders/basic.fs"});
            shader = Shader::createProgram(vertexSource, fragmentSource);

			vertexSource = utils::fileReadString(fs::path{"shaders/basic.vs"});
			fragmentSource = utils::fileReadString(fs::path{"shaders/lighting.fs"});
			lightingShader = Shader::createProgram(vertexSource, fragmentSource);

			lightingShader.bind();
			lightingShader.setFloat3("objectColor", glm::vec3{1.0f, 0.5f, 0.31f});
			lightingShader.setFloat3("lightColor",  glm::vec3{1.0f, 1.0f, 1.0f});
        }
    }
}


/*
 *  Other Functions
 */

// Reads inputs to update global state
void processInput(GLFWwindow *window) {
    // Move camera
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.move(Movement::Forward, deltaTime);
    } 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.move(Movement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.move(Movement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.move(Movement::Right, deltaTime);
    }
}

/*
 *  Main function
 */

int main() {
    spdlog::info("Starting renderer test program");

    // Set logging configuration
#ifdef _DEBUG
    spdlog::info("Running in debug mode");
    spdlog::set_level(spdlog::level::debug);
#endif

    // initialize glfw library
    spdlog::debug("Initializing GLFW");
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // create glfw window
    spdlog::debug("Creating GLFW window");
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Test Renderer", nullptr, nullptr);
    if (!window) {
        spdlog::critical("Failed to create a GLFW window");
        glfwTerminate();
        return -1;
    }

    // set opengl context
    spdlog::debug("Setting OpenGL context");
    glfwMakeContextCurrent(window);

    // get opengl function addresses using glad
    spdlog::debug("Retrieving OpenGL function pointers using glad loader");
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("Failed to retrieve OpenGL function pointers");
        glfwTerminate();
        return -1;
    }

    // set opengl viewport
	// apple displays that use retina have double resolution
	// TODO: detect retina display or get window size from glfw
    spdlog::debug("Setting OpenGL viewport");
#ifdef __APPLE__
	glViewport(0, 0, WIDTH * 2, HEIGHT * 2);
#else
    glViewport(0, 0, WIDTH, HEIGHT);
#endif

    // set glfw callbacks
    spdlog::debug("Setting GLFW callbacks");
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetKeyCallback(window, keyCallback);

    // Capture cursor
    spdlog::debug("Setting OpenGL and GLFW configuration");
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
	
	// vertices and indices
	constexpr float vertices[] = {
		// front vertices
		 0.5f,  0.5f,  0.5f,	// top right
		 0.5f, -0.5f,  0.5f,	// bottom right
		-0.5f, -0.5f,  0.5f,	// bottom left
		-0.5f,  0.5f,  0.5f,	// top left
		// back vertices
		 0.5f,  0.5f, -0.5f,	// top right
		 0.5f, -0.5f, -0.5f,	// bottom right
		-0.5f, -0.5f, -0.5f,	// bottom left
		-0.5f,  0.5f, -0.5f 	// top left
	};
	constexpr uint32_t indices[] = {
		// front face
		0, 1, 3,
		1, 2, 3,
		// top face
		0, 3, 4,
		3, 4, 7,
		// left face
		2, 3, 6,
		3, 6, 7,
		// right face
		0, 1, 4,
		1, 4, 5,
		// bottom face
		1, 2, 5,
		2, 5, 6,
		// back face
		4, 5, 7,
		5, 6, 7
	};
	constexpr size_t INDEX_COUNT = sizeof(indices) / sizeof(uint32_t);

	// setup cube VAO
    spdlog::debug("Setting up VAOs, VBO, and EBO");
	GLuint vao;
	glGenVertexArrays(1, &vao);

	// setup light VAO
	GLuint vaoLight;
	glGenVertexArrays(1, &vaoLight);

	// transfer vertices to VBO
	GLuint vbo;
	glBindVertexArray(vao);
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
	glBindVertexArray(vaoLight);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	// compile and link shader programs
    spdlog::debug("Compiling basic shader program");
	std::string vertexSource = utils::fileReadString(fs::path{"shaders/basic.vs"});
	std::string fragmentSource = utils::fileReadString(fs::path{"shaders/basic.fs"});
	shader = Shader::createProgram(vertexSource, fragmentSource);

    spdlog::debug("Compiling lighting shader program");
	fragmentSource = utils::fileReadString(fs::path{"shaders/lighting.fs"});
	lightingShader = Shader::createProgram(vertexSource, fragmentSource);

	// set shader uniforms
	lightingShader.bind();
	glBindVertexArray(vaoLight);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	lightingShader.setFloat3("objectColor", glm::vec3{1.0f, 0.5f, 0.31f});
	lightingShader.setFloat3("lightColor",  glm::vec3{1.0f, 1.0f, 1.0f});
	glBindVertexArray(0);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // update time globals
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // read input and update global state
        processInput(window);

        // set the screen to a static color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
         *  Draw triangles
         */

        // bind opengl objects
		glBindVertexArray(vao);

        // set projection and view matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoomOffset), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();
		shader.bind();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
		lightingShader.bind();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

        // draw normal triangles
		shader.bind();
        glm::mat4 model = glm::mat4{1.0f};
        model = glm::translate(model, glm::vec3{0.0f, 0.0f, 0.0f});
        shader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, 0);

		// draw lighting triangles
		lightingShader.bind();
		glBindVertexArray(vaoLight);
		model = glm::mat4{1.0f};
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightingShader.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, 0);

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // delete opengl objects
    spdlog::debug("Deleting OpenGL objects");
    shader.deleteShader();
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    // exit glfw
    spdlog::debug("Terminating GLFW");
    glfwTerminate();

    spdlog::info("Test renderer finished. Exiting.");

    return 0;
}
