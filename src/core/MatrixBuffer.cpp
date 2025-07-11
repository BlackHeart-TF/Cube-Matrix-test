#include "core/MatrixBuffer.h"
#include <stdexcept>

namespace LEDCube {

MatrixBuffer::MatrixBuffer() {
    buffer.resize(TOTAL_LEDS, Color::Black());
}

MatrixBuffer::~MatrixBuffer() {
}

void MatrixBuffer::setBuffer(const std::vector<Color>& colors) {
    if (colors.size() != TOTAL_LEDS) {
        throw std::invalid_argument("Buffer size must match total LED count");
    }
    buffer = colors;
}

void MatrixBuffer::setLED(const Position& pos, const Color& color) {
    if (!isValidPosition(pos)) {
        return;
    }
    
    int index = positionToIndex(pos);
    buffer[index] = color;
}

Color MatrixBuffer::getLED(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return Color::Black();
    }
    
    int index = positionToIndex(pos);
    return buffer[index];
}

void MatrixBuffer::clear() {
    fill(Color::Black());
}

void MatrixBuffer::fill(const Color& color) {
    for (auto& led : buffer) {
        led = color;
    }
}

void MatrixBuffer::copyFrom(const MatrixBuffer& other) {
    buffer = other.buffer;
}

std::vector<uint8_t> MatrixBuffer::toRGB888() const {
    std::vector<uint8_t> rgb888;
    rgb888.reserve(TOTAL_LEDS * 3);
    
    for (const auto& color : buffer) {
        rgb888.push_back(color.r);
        rgb888.push_back(color.g);
        rgb888.push_back(color.b);
    }
    
    return rgb888;
}

std::vector<uint8_t> MatrixBuffer::toRGB565() const {
    std::vector<uint8_t> rgb565;
    rgb565.reserve(TOTAL_LEDS * 2);
    
    for (const auto& color : buffer) {
        uint16_t rgb = colorToRGB565(color);
        rgb565.push_back((rgb >> 8) & 0xFF);
        rgb565.push_back(rgb & 0xFF);
    }
    
    return rgb565;
}

std::vector<uint8_t> MatrixBuffer::toRawBytes() const {
    return toRGB888();
}

bool MatrixBuffer::isValidPosition(const Position& pos) const {
    return pos.isValid();
}

int MatrixBuffer::positionToIndex(const Position& pos) const {
    return pos.z * (CUBE_SIZE * CUBE_SIZE) + pos.y * CUBE_SIZE + pos.x;
}

Position MatrixBuffer::indexToPosition(int index) const {
    if (index < 0 || index >= TOTAL_LEDS) {
        return Position();
    }
    
    int z = index / (CUBE_SIZE * CUBE_SIZE);
    int remainder = index % (CUBE_SIZE * CUBE_SIZE);
    int y = remainder / CUBE_SIZE;
    int x = remainder % CUBE_SIZE;
    
    return Position(x, y, z);
}

uint16_t MatrixBuffer::colorToRGB565(const Color& color) const {
    // Convert 8-bit RGB to 5-6-5 format
    uint8_t r = (color.r * 31) / 255;  // 5 bits
    uint8_t g = (color.g * 63) / 255;  // 6 bits
    uint8_t b = (color.b * 31) / 255;  // 5 bits
    
    return (r << 11) | (g << 5) | b;
}

Color MatrixBuffer::rgb565ToColor(uint16_t rgb565) const {
    // Convert 5-6-5 format to 8-bit RGB
    uint8_t r = ((rgb565 >> 11) & 0x1F) * 255 / 31;
    uint8_t g = ((rgb565 >> 5) & 0x3F) * 255 / 63;
    uint8_t b = (rgb565 & 0x1F) * 255 / 31;
    
    return Color(r, g, b);
}

} // namespace LEDCube 