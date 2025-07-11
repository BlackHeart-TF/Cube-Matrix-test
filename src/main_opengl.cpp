#include "core/LEDCube.h"
#include "core/AnimationManager.h"
#include "opengl/OpenGLRenderer.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace LEDCube;

int main() {
    std::cout << "LED Cube Matrix - OpenGL Preview Mode" << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // Initialize OpenGL renderer
    OpenGLRenderer renderer;
    if (!renderer.initialize(1024, 768, "LED Cube Preview")) {
        std::cerr << "Failed to initialize OpenGL renderer!" << std::endl;
        return -1;
    }
    
    // Initialize LED cube and animation manager
    LEDCube::LEDCube cube;
    AnimationManager animationManager;
    
    // Set up renderer
    renderer.setBackgroundColor(0.1f, 0.1f, 0.1f);
    renderer.setCubeScale(1.0f);
    renderer.setCameraPosition(0.0f, 0.0f, 5.0f);
    
    // Set up rotation callback for rain animation
    renderer.setCubeRotationCallback([&](float pitch, float yaw) {
        // Update rain animation gravity direction
        auto currentAnimation = animationManager.getCurrentAnimation();
        if (auto rainAnim = std::dynamic_pointer_cast<RainAnimation>(currentAnimation)) {
            rainAnim->setGravityDirection(pitch, yaw);
        }
    });
    
    // Set up input callbacks
    renderer.setKeyboardCallback([&](int key, int action) {
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE:
                    glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
                    break;
                case GLFW_KEY_1:
                    std::cout << "Playing Rain Animation" << std::endl;
                    animationManager.playAnimation("Rain");
                    break;
                case GLFW_KEY_2:
                    std::cout << "Playing Wave Animation" << std::endl;
                    animationManager.playAnimation("Wave");
                    break;
                case GLFW_KEY_3:
                    std::cout << "Playing Cube Rotation Animation" << std::endl;
                    animationManager.playAnimation("Cube Rotation");
                    break;
                case GLFW_KEY_4:
                    std::cout << "Playing Test Pattern Animation" << std::endl;
                    break;
                case GLFW_KEY_5:
                    std::cout << "Playing Game of Life Animation" << std::endl;
                    animationManager.playAnimation("Game of Life");
                    break;
                case GLFW_KEY_SPACE:
                    std::cout << "Pause/Resume Animation" << std::endl;
                    break;
                case GLFW_KEY_R:
                    std::cout << "Reset Animation" << std::endl;
                    animationManager.resetCurrentAnimation();
                    break;
            }
        }
    });
    
    // Get available animations
    auto animations = animationManager.getAnimationNames();
    std::cout << "Available animations:" << std::endl;
    for (size_t i = 0; i < animations.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << animations[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  1-5: Switch animations" << std::endl;
    std::cout << "  Space: Pause/Resume" << std::endl;
    std::cout << "  R: Reset animation" << std::endl;
    std::cout << "  Left Mouse + Drag: Rotate cube" << std::endl;
    std::cout << "  Scroll: Zoom camera" << std::endl;
    std::cout << "  Escape: Exit" << std::endl;
    std::cout << std::endl;
    
    // Start with first animation
    if (!animations.empty()) {
        animationManager.playAnimation(animations[0]);
        std::cout << "Playing: " << animations[0] << std::endl;
    }
    
    // Main render loop
    auto lastTime = std::chrono::high_resolution_clock::now();
    
    while (!renderer.shouldClose()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Cap delta time to prevent large jumps
        if (deltaTime > 0.1) deltaTime = 0.1;
        
        // Update animation
        animationManager.update(deltaTime);
        
        // **This line fills the cube buffer with the animation**
        animationManager.render(cube);

        // Render frame
        renderer.beginFrame();
        renderer.renderCube(cube);
        renderer.endFrame();
        
        // Poll events
        renderer.pollEvents();
        
        // Small delay to prevent excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    std::cout << "Shutting down..." << std::endl;
    renderer.shutdown();
    
    return 0;
} 