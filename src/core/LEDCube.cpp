#include "core/LEDCube.h"
#include <cstring>
#include <stdexcept>

namespace LEDCube {

LEDCube::LEDCube() {
    buffer.resize(TOTAL_LEDS, Color::Black());
}

LEDCube::~LEDCube() {
}

void LEDCube::setLED(const Position& pos, const Color& color) {
    if (!isValidPosition(pos)) {
        return;
    }
    
    int index = positionToIndex(pos);
    buffer[index] = color;
}

Color LEDCube::getLED(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return Color::Black();
    }
    
    int index = positionToIndex(pos);
    return buffer[index];
}

void LEDCube::clear() {
    fill(Color::Black());
}

void LEDCube::fill(const Color& color) {
    for (auto& led : buffer) {
        led = color;
    }
}

void LEDCube::setBuffer(const std::vector<Color>& newBuffer) {
    if (newBuffer.size() != TOTAL_LEDS) {
        throw std::invalid_argument("Buffer size must match total LED count");
    }
    buffer = newBuffer;
}

bool LEDCube::isValidPosition(const Position& pos) const {
    return pos.isValid();
}

int LEDCube::positionToIndex(const Position& pos) const {
    // Convert 3D position to linear index
    // Layout: z * (CUBE_SIZE * CUBE_SIZE) + y * CUBE_SIZE + x
    return pos.z * (CUBE_SIZE * CUBE_SIZE) + pos.y * CUBE_SIZE + pos.x;
}

Position LEDCube::indexToPosition(int index) const {
    if (index < 0 || index >= TOTAL_LEDS) {
        return Position();
    }
    
    // Convert linear index to 3D position
    int z = index / (CUBE_SIZE * CUBE_SIZE);
    int remainder = index % (CUBE_SIZE * CUBE_SIZE);
    int y = remainder / CUBE_SIZE;
    int x = remainder % CUBE_SIZE;
    
    return Position(x, y, z);
}

} // namespace LEDCube 