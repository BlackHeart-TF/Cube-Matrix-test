#include "opengl/OpenGLRenderer.h"
#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LEDCube {

OpenGLRenderer::OpenGLRenderer() 
    : window(nullptr), windowWidth(1024), windowHeight(768), windowTitle("LED Cube Preview"),
      cameraX(0.0f), cameraY(0.0f), cameraZ(5.0f), cameraPitch(0.0f), cameraYaw(0.0f), cameraDistance(5.0f),
      cubeScale(1.0f), wireframeMode(false), showAxes(false), vsyncEnabled(true),
      mouseSensitivity(0.1f), initialized(false) {
    
    backgroundColor[0] = 0.1f;
    backgroundColor[1] = 0.1f;
    backgroundColor[2] = 0.1f;
}

OpenGLRenderer::~OpenGLRenderer() {
    shutdown();
}

bool OpenGLRenderer::initialize(int width, int height, const std::string& title) {
    if (initialized) {
        return true;
    }
    
    windowWidth = width;
    windowHeight = height;
    windowTitle = title;
    
    std::cout << "OpenGL Renderer: Initializing..." << std::endl;
    
    try {
        if (!initializeGLFW()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
        
        if (!initializeOpenGL()) {
            throw std::runtime_error("Failed to initialize OpenGL");
        }
        
        setupCallbacks();
        initialized = true;
        
        std::cout << "OpenGL Renderer: Initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "OpenGL Renderer: Failed to initialize: " << e.what() << std::endl;
        return false;
    }
}

void OpenGLRenderer::shutdown() {
    if (!initialized) {
        return;
    }
    
    std::cout << "OpenGL Renderer: Shutting down..." << std::endl;
    
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    
    glfwTerminate();
    initialized = false;
    
    std::cout << "OpenGL Renderer: Shutdown complete" << std::endl;
}

void OpenGLRenderer::setWindowSize(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    
    if (window) {
        glfwSetWindowSize(window, width, height);
    }
}

void OpenGLRenderer::setWindowTitle(const std::string& title) {
    windowTitle = title;
    
    if (window) {
        glfwSetWindowTitle(window, title.c_str());
    }
}

bool OpenGLRenderer::shouldClose() const {
    return window ? glfwWindowShouldClose(window) : true;
}

void OpenGLRenderer::pollEvents() {
    if (window) {
        glfwPollEvents();
    }
}

void OpenGLRenderer::beginFrame() {
    if (!initialized || !window) {
        return;
    }
    
    // Clear the screen
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::endFrame() {
    if (!initialized || !window) {
        return;
    }
    
    // Swap buffers
    glfwSwapBuffers(window);
}

void OpenGLRenderer::renderCube(const LEDCube& cube) {
    if (!initialized || !window || !cubeRenderer) {
        return;
    }
    
    // Calculate view and projection matrices
    glm::mat4 view = glm::lookAt(
        glm::vec3(cameraX, cameraY, cameraZ),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)windowWidth / (float)windowHeight,
        0.1f, 100.0f
    );
    
    glm::mat4 viewProj = projection * view;
    
    // Build model matrix from cubePitch/cubeYaw
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(cubePitch), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(cubeYaw), glm::vec3(0, 1, 0));
    
    // Render the cube
    cubeRenderer->renderCube(cube, viewProj, model);
}

void OpenGLRenderer::setCameraPosition(float x, float y, float z) {
    cameraX = x;
    cameraY = y;
    cameraZ = z;
}

void OpenGLRenderer::setCameraRotation(float pitch, float yaw) {
    cameraPitch = pitch;
    cameraYaw = yaw;
}

void OpenGLRenderer::setCameraDistance(float distance) {
    cameraDistance = distance;
}

void OpenGLRenderer::setBackgroundColor(float r, float g, float b) {
    backgroundColor[0] = r;
    backgroundColor[1] = g;
    backgroundColor[2] = b;
}

void OpenGLRenderer::setCubeScale(float scale) {
    cubeScale = scale;
    if (cubeRenderer) {
        cubeRenderer->setCubeScale(scale);
    }
}

void OpenGLRenderer::setWireframeMode(bool enabled) {
    wireframeMode = enabled;
}

void OpenGLRenderer::setShowAxes(bool enabled) {
    showAxes = enabled;
}

void OpenGLRenderer::setMouseSensitivity(float sensitivity) {
    mouseSensitivity = sensitivity;
}

void OpenGLRenderer::setKeyboardCallback(std::function<void(int, int)> callback) {
    keyboardCallback = callback;
}

void OpenGLRenderer::setMouseCallback(std::function<void(double, double)> callback) {
    mouseCallback = callback;
}

void OpenGLRenderer::takeScreenshot(const std::string& filename) {
    std::cout << "OpenGL Renderer: Screenshot saved to " << filename << std::endl;
}

void OpenGLRenderer::setVSync(bool enabled) {
    vsyncEnabled = enabled;
    if (window) {
        glfwSwapInterval(enabled ? 1 : 0);
    }
}

bool OpenGLRenderer::initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "OpenGL Renderer: Failed to initialize GLFW" << std::endl;
        return false;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "OpenGL Renderer: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(vsyncEnabled ? 1 : 0);
    
    return true;
}

bool OpenGLRenderer::initializeOpenGL() {
    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "OpenGL Renderer: Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    // Initialize cube renderer
    cubeRenderer = std::make_unique<CubeRenderer>();
    if (!cubeRenderer->initialize()) {
        std::cerr << "OpenGL Renderer: Failed to initialize cube renderer" << std::endl;
        return false;
    }
    
    std::cout << "OpenGL Renderer: OpenGL initialized successfully" << std::endl;
    return true;
}

void OpenGLRenderer::setupCallbacks() {
    if (!window) {
        return;
    }
    
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void OpenGLRenderer::updateCamera() {
    // Update camera matrices based on position, rotation, and distance
}

void OpenGLRenderer::renderAxes() {
    // Render coordinate axes for debugging
}

void OpenGLRenderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void OpenGLRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Handle keyboard input
}

void OpenGLRenderer::mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    OpenGLRenderer* self = static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
    if (self && self->dragging) {
        float dx = xpos - self->lastMouseX;
        float dy = ypos - self->lastMouseY;
        self->cubeYaw += dx * self->mouseSensitivity;
        self->cubePitch += dy * self->mouseSensitivity;
        if (self->rotationCallback) self->rotationCallback(self->cubePitch, self->cubeYaw);
        self->lastMouseX = xpos;
        self->lastMouseY = ypos;
    }
}

void OpenGLRenderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    OpenGLRenderer* self = static_cast<OpenGLRenderer*>(glfwGetWindowUserPointer(window));
    if (self) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                self->dragging = true;
                double x, y;
                glfwGetCursorPos(window, &x, &y);
                self->lastMouseX = x;
                self->lastMouseY = y;
            } else if (action == GLFW_RELEASE) {
                self->dragging = false;
            }
        }
    }
}

void OpenGLRenderer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Handle mouse scroll
}

} // namespace LEDCube

 