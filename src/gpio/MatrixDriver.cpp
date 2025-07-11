#include "gpio/MatrixDriver.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace LEDCube {

MatrixDriver::MatrixDriver() 
    : initialized(false), refreshRate(60), brightness(1.0), currentLayer(0),
      displayThreadRunning(false), shouldStop(false) {
}

MatrixDriver::~MatrixDriver() {
    shutdown();
}

bool MatrixDriver::initialize() {
    if (initialized) {
        return true;
    }
    
    std::cout << "Matrix Driver: Initializing..." << std::endl;
    
    try {
        initializeGPIO();
        setupTiming();
        initialized = true;
        std::cout << "Matrix Driver: Initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Matrix Driver: Failed to initialize: " << e.what() << std::endl;
        return false;
    }
}

void MatrixDriver::shutdown() {
    if (!initialized) {
        return;
    }
    
    std::cout << "Matrix Driver: Shutting down..." << std::endl;
    
    stopDisplay();
    cleanupGPIO();
    initialized = false;
    
    std::cout << "Matrix Driver: Shutdown complete" << std::endl;
}

void MatrixDriver::setBuffer(const MatrixBuffer& buffer) {
    displayBuffer = buffer;
}

void MatrixDriver::updateBuffer(const MatrixBuffer& buffer) {
    displayBuffer = buffer;
}

void MatrixDriver::startDisplay() {
    if (!initialized || displayThreadRunning) {
        return;
    }
    
    std::cout << "Matrix Driver: Starting display thread..." << std::endl;
    
    shouldStop = false;
    displayThreadRunning = true;
    displayThread = std::thread(&MatrixDriver::displayLoop, this);
}

void MatrixDriver::stopDisplay() {
    if (!displayThreadRunning) {
        return;
    }
    
    std::cout << "Matrix Driver: Stopping display thread..." << std::endl;
    
    shouldStop = true;
    if (displayThread.joinable()) {
        displayThread.join();
    }
    displayThreadRunning = false;
}

void MatrixDriver::setRefreshRate(int fps) {
    refreshRate = fps;
    std::cout << "Matrix Driver: Refresh rate set to " << fps << " FPS" << std::endl;
}

void MatrixDriver::setBrightness(double level) {
    brightness = std::max(0.0, std::min(1.0, level));
    std::cout << "Matrix Driver: Brightness set to " << (brightness * 100) << "%" << std::endl;
}

void MatrixDriver::setCurrentLayer(int layer) {
    if (layer >= 0 && layer < CUBE_DEPTH) {
        currentLayer = layer;
    }
}

void MatrixDriver::clearDisplay() {
    displayBuffer.clear();
}

void MatrixDriver::testPattern() {
    std::cout << "Matrix Driver: Running test pattern..." << std::endl;
    
    // Create a simple test pattern
    for (int x = 0; x < CUBE_SIZE; ++x) {
        for (int y = 0; y < CUBE_SIZE; ++y) {
            for (int z = 0; z < CUBE_DEPTH; ++z) {
                Color color;
                if (x == y || x == (CUBE_SIZE - 1 - y)) {
                    color = Color::Red();
                } else if (x == z || y == z) {
                    color = Color::Green();
                } else {
                    color = Color::Blue();
                }
                displayBuffer.setLED(Position(x, y, z), color);
            }
        }
    }
}

void MatrixDriver::setAllLEDs(const Color& color) {
    displayBuffer.fill(color);
}

void MatrixDriver::displayLoop() {
    std::cout << "Matrix Driver: Display loop started" << std::endl;
    
    auto frameTime = std::chrono::microseconds(1000000 / refreshRate);
    
    while (!shouldStop) {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Render current frame
        renderFrame();
        
        // Calculate sleep time to maintain frame rate
        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        auto sleepTime = frameTime - elapsed;
        
        if (sleepTime.count() > 0) {
            std::this_thread::sleep_for(sleepTime);
        }
    }
    
    std::cout << "Matrix Driver: Display loop stopped" << std::endl;
}

void MatrixDriver::renderLayer(int layer) {
    if (!initialized) {
        return;
    }
    
    // Set the current layer
    setCurrentLayer(layer);
    
    // In real implementation, this would:
    // 1. Set address pins for layer selection
    // 2. Send RGB data for this layer
    // 3. Latch the data
    // 4. Enable output for this layer
    
    // For simulation, we'll just simulate the timing
    std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void MatrixDriver::renderFrame() {
    if (!initialized) {
        return;
    }
    
    // Render each layer in sequence
    for (int layer = 0; layer < CUBE_DEPTH; ++layer) {
        renderLayer(layer);
    }
}

void MatrixDriver::initializeGPIO() {
    gpio = std::make_unique<GPIOController>();
    if (!gpio->initialize()) {
        throw std::runtime_error("Failed to initialize GPIO controller");
    }
}

void MatrixDriver::cleanupGPIO() {
    if (gpio) {
        gpio->shutdown();
        gpio.reset();
    }
}

void MatrixDriver::setupTiming() {
    // Set up timing parameters for the LED matrix
    // In real implementation, this would configure SPI timing, etc.
}

uint16_t MatrixDriver::colorToHardwareFormat(const Color& color) const {
    // Convert color to hardware format (e.g., RGB565)
    uint8_t r = static_cast<uint8_t>(color.r * brightness);
    uint8_t g = static_cast<uint8_t>(color.g * brightness);
    uint8_t b = static_cast<uint8_t>(color.b * brightness);
    
    // Convert to RGB565
    uint16_t r5 = (r * 31) / 255;
    uint16_t g6 = (g * 63) / 255;
    uint16_t b5 = (b * 31) / 255;
    
    return (r5 << 11) | (g6 << 5) | b5;
}

Color MatrixDriver::hardwareFormatToColor(uint16_t hwColor) const {
    // Convert hardware format back to Color
    uint8_t r = ((hwColor >> 11) & 0x1F) * 255 / 31;
    uint8_t g = ((hwColor >> 5) & 0x3F) * 255 / 63;
    uint8_t b = (hwColor & 0x1F) * 255 / 31;
    
    return Color(r, g, b);
}

} // namespace LEDCube 