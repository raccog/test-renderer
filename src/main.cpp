#include <cstdint>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

constexpr uint32_t WIDTH = 800;
constexpr uint32_t HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
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
    glViewport(0, 0, WIDTH, HEIGHT);

    // set glfw callbacks
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // set the screen to a static color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // exit glfw
    glfwTerminate();

    return 0;
}
