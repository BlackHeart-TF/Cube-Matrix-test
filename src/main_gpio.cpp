#include "core/LEDCube.h"
#include "core/AnimationManager.h"
#include "gpio/MatrixDriver.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>

using namespace LEDCube;

// Global variables for signal handling
volatile bool shouldExit = false;
MatrixDriver* g_matrixDriver = nullptr;

void signalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ", shutting down..." << std::endl;
    shouldExit = true;
    
    if (g_matrixDriver) {
        g_matrixDriver->stopDisplay();
    }
}

int main() {
    std::cout << "LED Cube Matrix - GPIO Mode" << std::endl;
    std::cout << "===========================" << std::endl;
    
    // Set up signal handlers for clean shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize matrix driver
    MatrixDriver matrixDriver;
    g_matrixDriver = &matrixDriver;
    
    if (!matrixDriver.initialize()) {
        std::cerr << "Failed to initialize matrix driver!" << std::endl;
        return -1;
    }
    
    // Initialize LED cube and animation manager
    LEDCube cube;
    AnimationManager animationManager;
    
    // Set up matrix driver
    matrixDriver.setRefreshRate(60);
    matrixDriver.setBrightness(0.8);
    
    // Get available animations
    auto animations = animationManager.getAnimationNames();
    std::cout << "Available animations:" << std::endl;
    for (size_t i = 0; i < animations.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << animations[i] << std::endl;
    }
    std::cout << std::endl;
    
    // Start with first animation
    if (!animations.empty()) {
        animationManager.playAnimation(animations[0]);
        std::cout << "Playing: " << animations[0] << std::endl;
    }
    
    // Start display thread
    matrixDriver.startDisplay();
    
    // Main update loop
    auto lastTime = std::chrono::high_resolution_clock::now();
    int currentAnimationIndex = 0;
    auto lastAnimationChange = std::chrono::high_resolution_clock::now();
    
    std::cout << "Running on hardware. Press Ctrl+C to exit." << std::endl;
    std::cout << "Animations will cycle automatically every 10 seconds." << std::endl;
    
    while (!shouldExit) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;
        
        // Cap delta time to prevent large jumps
        if (deltaTime > 0.1) deltaTime = 0.1;
        
        // Update animation
        animationManager.update(deltaTime);
        
        // Update matrix buffer
        matrixDriver.updateBuffer(MatrixBuffer(cube.getBuffer()));
        
        // Cycle through animations every 10 seconds
        auto timeSinceChange = std::chrono::duration<double>(currentTime - lastAnimationChange).count();
        if (timeSinceChange > 10.0 && !animations.empty()) {
            currentAnimationIndex = (currentAnimationIndex + 1) % animations.size();
            animationManager.playAnimation(animations[currentAnimationIndex]);
            std::cout << "Switched to: " << animations[currentAnimationIndex] << std::endl;
            lastAnimationChange = currentTime;
        }
        
        // Small delay to prevent excessive CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
    
    std::cout << "Shutting down..." << std::endl;
    
    // Clean shutdown
    matrixDriver.stopDisplay();
    matrixDriver.shutdown();
    g_matrixDriver = nullptr;
    
    std::cout << "Shutdown complete." << std::endl;
    return 0;
} 