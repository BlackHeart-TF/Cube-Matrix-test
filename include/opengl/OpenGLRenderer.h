#pragma once

#include "Shader.h"
#include "CubeRenderer.h"
#include "../core/LEDCube.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <string>
#include <functional>

namespace LEDCube {

class OpenGLRenderer {
public:
    OpenGLRenderer();
    ~OpenGLRenderer();
    
    // Initialization
    bool initialize(int width = 1024, int height = 768, const std::string& title = "LED Cube Preview");
    void shutdown();
    bool isInitialized() const { return initialized; }
    
    // Window management
    void setWindowSize(int width, int height);
    void setWindowTitle(const std::string& title);
    bool shouldClose() const;
    void pollEvents();
    
    // Rendering
    void beginFrame();
    void endFrame();
    void renderCube(const LEDCube& cube);
    
    // Camera control
    void setCameraPosition(float x, float y, float z);
    void setCameraRotation(float pitch, float yaw);
    void setCameraDistance(float distance);
    
    // Display settings
    void setBackgroundColor(float r, float g, float b);
    void setCubeScale(float scale);
    void setWireframeMode(bool enabled);
    void setShowAxes(bool enabled);
    
    // Input handling
    void setMouseSensitivity(float sensitivity);
    void setKeyboardCallback(std::function<void(int, int)> callback);
    void setMouseCallback(std::function<void(double, double)> callback);
    
    // Utility
    void takeScreenshot(const std::string& filename);
    void setVSync(bool enabled);
    void setCubeRotationCallback(std::function<void(float, float)> cb) { rotationCallback = std::move(cb); }

private:
    GLFWwindow* window;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<CubeRenderer> cubeRenderer;
    
    // Window settings
    int windowWidth;
    int windowHeight;
    std::string windowTitle;
    
    // Camera settings
    float cameraX, cameraY, cameraZ;
    float cameraPitch, cameraYaw;
    float cameraDistance;
    
    // Display settings
    float backgroundColor[3];
    float cubeScale;
    bool wireframeMode;
    bool showAxes;
    bool vsyncEnabled;
    
    // Input settings
    float mouseSensitivity;
    std::function<void(int, int)> keyboardCallback;
    std::function<void(double, double)> mouseCallback;
    
    bool initialized;
    
    // Helper methods
    bool initializeGLFW();
    bool initializeOpenGL();
    void setupCallbacks();
    void updateCamera();
    void renderAxes();
    
    // Callback functions
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    float cubePitch = 0.0f, cubeYaw = 0.0f;
    bool dragging = false;
    double lastMouseX = 0.0, lastMouseY = 0.0;
    std::function<void(float, float)> rotationCallback;
};

} // namespace LEDCube 