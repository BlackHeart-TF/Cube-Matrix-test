#pragma once

#include "LEDCube.h"
#include <vector>
#include <cstdint>

namespace LEDCube {

// Matrix buffer for efficient LED data handling
class MatrixBuffer {
public:
    MatrixBuffer();
    ~MatrixBuffer();
    
    // Buffer management
    void setBuffer(const std::vector<Color>& colors);
    const std::vector<Color>& getBuffer() const { return buffer; }
    
    // LED control
    void setLED(const Position& pos, const Color& color);
    Color getLED(const Position& pos) const;
    
    // Buffer operations
    void clear();
    void fill(const Color& color);
    void copyFrom(const MatrixBuffer& other);
    
    // Data conversion for different output formats
    std::vector<uint8_t> toRGB888() const;
    std::vector<uint8_t> toRGB565() const;
    std::vector<uint8_t> toRawBytes() const;
    
    // Utility
    bool isValidPosition(const Position& pos) const;
    int positionToIndex(const Position& pos) const;
    Position indexToPosition(int index) const;
    
    // Buffer info
    size_t getSize() const { return buffer.size(); }
    bool isEmpty() const { return buffer.empty(); }

private:
    std::vector<Color> buffer;
    
    // Helper methods for data conversion
    uint16_t colorToRGB565(const Color& color) const;
    Color rgb565ToColor(uint16_t rgb565) const;
};

} // namespace LEDCube 