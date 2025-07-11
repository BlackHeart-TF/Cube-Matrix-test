#include "gpio/GPIOController.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace LEDCube {

GPIOController::GPIOController() : initialized(false) {
    pinStates.resize(64, false);
    pinModes.resize(64, 0);
}

GPIOController::~GPIOController() {
    shutdown();
}

bool GPIOController::initialize() {
    if (initialized) {
        return true;
    }
    
    std::cout << "GPIO Controller: Initializing..." << std::endl;
    
    // In a real implementation, this would initialize WiringPi
    // For now, we'll just simulate the initialization
    
    try {
        initializePins();
        initialized = true;
        std::cout << "GPIO Controller: Initialized successfully" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "GPIO Controller: Failed to initialize: " << e.what() << std::endl;
        return false;
    }
}

void GPIOController::shutdown() {
    if (!initialized) {
        return;
    }
    
    std::cout << "GPIO Controller: Shutting down..." << std::endl;
    
    try {
        cleanupPins();
        initialized = false;
        std::cout << "GPIO Controller: Shutdown complete" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "GPIO Controller: Error during shutdown: " << e.what() << std::endl;
    }
}

void GPIOController::setPin(int pin, bool state) {
    if (!isValidPin(pin)) {
        std::cerr << "GPIO Controller: Invalid pin " << pin << std::endl;
        return;
    }
    
    if (initialized) {
        // In real implementation: digitalWrite(pin, state ? HIGH : LOW);
        pinStates[pin] = state;
    }
}

bool GPIOController::getPin(int pin) const {
    if (!isValidPin(pin)) {
        return false;
    }
    
    return pinStates[pin];
}

void GPIOController::setPinMode(int pin, int mode) {
    if (!isValidPin(pin)) {
        std::cerr << "GPIO Controller: Invalid pin " << pin << std::endl;
        return;
    }
    
    if (initialized) {
        // In real implementation: pinMode(pin, mode);
        pinModes[pin] = mode;
    }
}

void GPIOController::spiWrite(const std::vector<uint8_t>& data) {
    if (!initialized) {
        return;
    }
    
    // In real implementation, this would write SPI data
    // For now, just simulate the timing
    for (uint8_t byte : data) {
        spiWriteByte(byte);
    }
}

void GPIOController::spiWriteByte(uint8_t byte) {
    if (!initialized) {
        return;
    }
    
    // In real implementation, this would write a single byte via SPI
    // For now, just simulate the timing
    std::this_thread::sleep_for(std::chrono::microseconds(10));
}

void GPIOController::delayMicroseconds(unsigned int microseconds) {
    std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

void GPIOController::delayMilliseconds(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

void GPIOController::setLayer(int layer) {
    if (!initialized) {
        return;
    }
    
    // Set address pins for layer selection
    setPin(GPIOPins::ADDR_A, (layer & 0x01) != 0);
    setPin(GPIOPins::ADDR_B, (layer & 0x02) != 0);
    setPin(GPIOPins::ADDR_C, (layer & 0x04) != 0);
    setPin(GPIOPins::ADDR_D, (layer & 0x08) != 0);
}

void GPIOController::enableOutput(bool enable) {
    if (!initialized) {
        return;
    }
    
    setPin(GPIOPins::OE_PIN, !enable); // Active low
}

void GPIOController::latchData() {
    if (!initialized) {
        return;
    }
    
    // Latch the data into the matrix
    setPin(GPIOPins::LATCH_PIN, true);
    delayMicroseconds(1);
    setPin(GPIOPins::LATCH_PIN, false);
}

void GPIOController::resetMatrix() {
    if (!initialized) {
        return;
    }
    
    setPin(GPIOPins::RESET_PIN, true);
    delayMicroseconds(100);
    setPin(GPIOPins::RESET_PIN, false);
}

void GPIOController::setAllPinsLow() {
    if (!initialized) {
        return;
    }
    
    for (int i = 0; i < 64; ++i) {
        if (isValidPin(i)) {
            setPin(i, false);
        }
    }
}

void GPIOController::setAllPinsHigh() {
    if (!initialized) {
        return;
    }
    
    for (int i = 0; i < 64; ++i) {
        if (isValidPin(i)) {
            setPin(i, true);
        }
    }
}

bool GPIOController::isValidPin(int pin) const {
    return pin >= 0 && pin < 64;
}

void GPIOController::initializePins() {
    // Initialize all pins to output mode
    for (int pin : {GPIOPins::DATA_PIN, GPIOPins::CLOCK_PIN, GPIOPins::LATCH_PIN,
                     GPIOPins::OE_PIN, GPIOPins::ADDR_A, GPIOPins::ADDR_B,
                     GPIOPins::ADDR_C, GPIOPins::ADDR_D, GPIOPins::RESET_PIN,
                     GPIOPins::BLANK_PIN}) {
        setPinMode(pin, 1); // OUTPUT mode
        setPin(pin, false);  // Start low
    }
}

void GPIOController::cleanupPins() {
    // Set all pins to safe state
    setAllPinsLow();
}

} // namespace LEDCube 