#pragma once

#include <vector>
#include <cstdint>
#include <memory>

namespace LEDCube {

// Cube dimensions
constexpr int CUBE_SIZE = 64;
constexpr int CUBE_DEPTH = 6;
constexpr int TOTAL_LEDS = CUBE_SIZE * CUBE_SIZE * CUBE_DEPTH;

// Color structure (RGB)
struct Color {
    uint8_t r, g, b;
    
    Color() : r(0), g(0), b(0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue) {}
    
    // Common colors
    static Color Black() { return Color(0, 0, 0); }
    static Color White() { return Color(255, 255, 255); }
    static Color Red() { return Color(255, 0, 0); }
    static Color Green() { return Color(0, 255, 0); }
    static Color Blue() { return Color(0, 0, 255); }
    static Color Yellow() { return Color(255, 255, 0); }
    static Color Cyan() { return Color(0, 255, 255); }
    static Color Magenta() { return Color(255, 0, 255); }
};

// 3D position in the cube
struct Position {
    int x, y, z;
    
    Position() : x(0), y(0), z(0) {}
    Position(int xPos, int yPos, int zPos) : x(xPos), y(yPos), z(zPos) {}
    
    bool isValid() const {
        return x >= 0 && x < CUBE_SIZE && 
               y >= 0 && y < CUBE_SIZE && 
               z >= 0 && z < CUBE_DEPTH;
    }
};

// Main LED Cube class
class LEDCube {
public:
    LEDCube();
    ~LEDCube();
    
    // Core functionality
    void setLED(const Position& pos, const Color& color);
    Color getLED(const Position& pos) const;
    void clear();
    void fill(const Color& color);
    
    // Buffer management
    const std::vector<Color>& getBuffer() const { return buffer; }
    void setBuffer(const std::vector<Color>& newBuffer);
    
    // Utility functions
    bool isValidPosition(const Position& pos) const;
    int positionToIndex(const Position& pos) const;
    Position indexToPosition(int index) const;
    
    // Cube dimensions
    static int getWidth() { return CUBE_SIZE; }
    static int getHeight() { return CUBE_SIZE; }
    static int getDepth() { return CUBE_DEPTH; }
    static int getTotalLEDs() { return TOTAL_LEDS; }

private:
    std::vector<Color> buffer;
};

} // namespace LEDCube 