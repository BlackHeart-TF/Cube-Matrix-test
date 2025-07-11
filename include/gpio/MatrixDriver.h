#pragma once

#include "GPIOController.h"
#include "../core/MatrixBuffer.h"
#include <memory>
#include <thread>
#include <atomic>

namespace LEDCube {

class MatrixDriver {
public:
    MatrixDriver();
    ~MatrixDriver();
    
    // Initialization
    bool initialize();
    void shutdown();
    bool isInitialized() const { return initialized; }
    
    // Buffer management
    void setBuffer(const MatrixBuffer& buffer);
    void updateBuffer(const MatrixBuffer& buffer);
    
    // Display control
    void startDisplay();
    void stopDisplay();
    bool isDisplaying() const { return displayThreadRunning; }
    
    // Display settings
    void setRefreshRate(int fps);
    int getRefreshRate() const { return refreshRate; }
    
    void setBrightness(double brightness); // 0.0 to 1.0
    double getBrightness() const { return brightness; }
    
    // Layer control
    void setCurrentLayer(int layer);
    int getCurrentLayer() const { return currentLayer; }
    
    // Utility
    void clearDisplay();
    void testPattern();
    void setAllLEDs(const Color& color);

private:
    std::unique_ptr<GPIOController> gpio;
    MatrixBuffer displayBuffer;
    
    // Display thread
    std::thread displayThread;
    std::atomic<bool> displayThreadRunning;
    std::atomic<bool> shouldStop;
    
    // Display settings
    int refreshRate;
    double brightness;
    int currentLayer;
    bool initialized;
    
    // Display loop
    void displayLoop();
    void renderLayer(int layer);
    void renderFrame();
    
    // Helper methods
    void initializeGPIO();
    void cleanupGPIO();
    void setupTiming();
    
    // Color conversion for hardware
    uint16_t colorToHardwareFormat(const Color& color) const;
    Color hardwareFormatToColor(uint16_t hwColor) const;
};

} // namespace LEDCube 