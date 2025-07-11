#pragma once

#include <cstdint>
#include <vector>
#include <memory>

namespace LEDCube {

// GPIO pin definitions for Adafruit 64x64x6 panel cube
struct GPIOPins {
    // Data pins for LED matrix
    static constexpr int DATA_PIN = 10;      // GPIO 10 (SPI0 MOSI)
    static constexpr int CLOCK_PIN = 11;     // GPIO 11 (SPI0 SCLK)
    static constexpr int LATCH_PIN = 8;      // GPIO 8
    static constexpr int OE_PIN = 9;         // GPIO 9 (Output Enable)
    
    // Address pins for layer selection
    static constexpr int ADDR_A = 12;        // GPIO 12
    static constexpr int ADDR_B = 13;        // GPIO 13
    static constexpr int ADDR_C = 14;        // GPIO 14
    static constexpr int ADDR_D = 15;        // GPIO 15
    
    // Additional control pins
    static constexpr int RESET_PIN = 16;     // GPIO 16
    static constexpr int BLANK_PIN = 17;     // GPIO 17
};

class GPIOController {
public:
    GPIOController();
    ~GPIOController();
    
    // Initialization
    bool initialize();
    void shutdown();
    bool isInitialized() const { return initialized; }
    
    // Pin control
    void setPin(int pin, bool state);
    bool getPin(int pin) const;
    void setPinMode(int pin, int mode);
    
    // SPI operations
    void spiWrite(const std::vector<uint8_t>& data);
    void spiWriteByte(uint8_t byte);
    
    // Timing control
    void delayMicroseconds(unsigned int microseconds);
    void delayMilliseconds(unsigned int milliseconds);
    
    // Matrix-specific operations
    void setLayer(int layer);
    void enableOutput(bool enable);
    void latchData();
    void resetMatrix();
    
    // Utility
    void setAllPinsLow();
    void setAllPinsHigh();

private:
    bool initialized;
    
    // Pin state tracking
    std::vector<bool> pinStates;
    std::vector<int> pinModes;
    
    // Helper methods
    bool isValidPin(int pin) const;
    void initializePins();
    void cleanupPins();
};

} // namespace LEDCube 