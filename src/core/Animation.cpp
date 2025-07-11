#include "core/Animation.h"
#include <cmath>
#include <random>
#include <algorithm>

namespace LEDCube {

// FunctionAnimation implementation
FunctionAnimation::FunctionAnimation(const std::string& name, 
                                   AnimationFunction func, 
                                   double duration)
    : name(name), animationFunc(func), duration(duration) {
}

void FunctionAnimation::update(double deltaTime) {
    if (finished) return;
    
    currentTime += deltaTime * animationSpeed;
    
    if (duration > 0.0 && currentTime >= duration) {
        if (isLooping) {
            currentTime = 0.0;
        } else {
            finished = true;
        }
    }
}

void FunctionAnimation::render(LEDCube& cube) {
    if (animationFunc) {
        animationFunc(cube, currentTime);
    }
}

void FunctionAnimation::reset() {
    currentTime = 0.0;
    finished = false;
}

bool FunctionAnimation::isFinished() const {
    return finished;
}

// RainAnimation implementation
RainAnimation::RainAnimation() {
    drops.reserve(50); // Pre-allocate space for drops
}

void RainAnimation::init() {
    drops.clear();
    spawnTimer = 0.0;
}

void RainAnimation::setGravityDirection(float pitch, float yaw) {
    // Convert pitch/yaw to gravity direction
    // Pitch affects Y and Z, Yaw affects X and Z
    float pitchRad = pitch * M_PI / 180.0f;
    float yawRad = yaw * M_PI / 180.0f;
    
    // Calculate gravity direction based on cube orientation
    gravityX = -std::sin(yawRad) * std::cos(pitchRad);
    gravityY = -std::sin(pitchRad);
    gravityZ = -std::cos(yawRad) * std::cos(pitchRad);
    
    // Normalize the gravity vector
    float length = std::sqrt(gravityX * gravityX + gravityY * gravityY + gravityZ * gravityZ);
    if (length > 0.001f) {
        gravityX /= length;
        gravityY /= length;
        gravityZ /= length;
    }
}

void RainAnimation::update(double deltaTime) {
    currentTime += deltaTime * animationSpeed;
    spawnTimer += deltaTime * animationSpeed;
    
    // Spawn new drops at the "top" of the cube based on gravity
    if (spawnTimer > 0.1) { // Spawn every 0.1 seconds
        spawnTimer = 0.0;
        
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> posDist(0, CUBE_SIZE - 1);
        static std::uniform_real_distribution<float> speedDist(20.0f, 50.0f);
        static std::uniform_real_distribution<float> colorDist(0.0f, 1.0f);
        
        RainDrop drop;
        
        // Spawn drops at the "top" face based on gravity direction
        if (std::abs(gravityY) > std::abs(gravityX) && std::abs(gravityY) > std::abs(gravityZ)) {
            // Gravity mostly Y direction
            if (gravityY > 0) {
                // Gravity pointing up, spawn at bottom
                drop.pos = Position(static_cast<int>(posDist(gen)), 0, static_cast<int>(posDist(gen)));
            } else {
                // Gravity pointing down, spawn at top
                drop.pos = Position(static_cast<int>(posDist(gen)), CUBE_SIZE - 1, static_cast<int>(posDist(gen)));
            }
        } else if (std::abs(gravityX) > std::abs(gravityZ)) {
            // Gravity mostly X direction
            if (gravityX > 0) {
                // Gravity pointing right, spawn at left
                drop.pos = Position(0, static_cast<int>(posDist(gen)), static_cast<int>(posDist(gen)));
            } else {
                // Gravity pointing left, spawn at right
                drop.pos = Position(CUBE_SIZE - 1, static_cast<int>(posDist(gen)), static_cast<int>(posDist(gen)));
            }
        } else {
            // Gravity mostly Z direction
            if (gravityZ > 0) {
                // Gravity pointing forward, spawn at back
                drop.pos = Position(static_cast<int>(posDist(gen)), static_cast<int>(posDist(gen)), 0);
            } else {
                // Gravity pointing back, spawn at front
                drop.pos = Position(static_cast<int>(posDist(gen)), static_cast<int>(posDist(gen)), CUBE_DEPTH - 1);
            }
        }
        
        drop.speed = speedDist(gen);
        drop.color = Color(
            static_cast<uint8_t>(colorDist(gen) * 255),
            static_cast<uint8_t>(colorDist(gen) * 255),
            static_cast<uint8_t>(colorDist(gen) * 255)
        );
        
        drops.push_back(drop);
    }
    
    // Update existing drops based on gravity direction
    for (auto it = drops.begin(); it != drops.end();) {
        // Move drops in gravity direction
        it->pos.x += gravityX * it->speed * deltaTime * animationSpeed;
        it->pos.y += gravityY * it->speed * deltaTime * animationSpeed;
        it->pos.z += gravityZ * it->speed * deltaTime * animationSpeed;
        
        // Remove drops that are outside the cube
        if (it->pos.x < 0 || it->pos.x >= CUBE_SIZE ||
            it->pos.y < 0 || it->pos.y >= CUBE_SIZE ||
            it->pos.z < 0 || it->pos.z >= CUBE_DEPTH) {
            it = drops.erase(it);
        } else {
            ++it;
        }
    }
}

void RainAnimation::render(LEDCube& cube) {
    // Clear cube
    cube.clear();
    
    // Render drops
    for (const auto& drop : drops) {
        if (drop.pos.isValid()) {
            cube.setLED(drop.pos, drop.color);
        }
    }
}

void RainAnimation::reset() {
    drops.clear();
    spawnTimer = 0.0;
    currentTime = 0.0;
}

// WaveAnimation implementation
WaveAnimation::WaveAnimation() {
}

void WaveAnimation::init() {
    waveTime = 0.0;
}

void WaveAnimation::update(double deltaTime) {
    waveTime += deltaTime * animationSpeed;
}

void WaveAnimation::render(LEDCube& cube) {
    cube.clear();
    
    for (int x = 0; x < CUBE_SIZE; ++x) {
        for (int y = 0; y < CUBE_SIZE; ++y) {
            for (int z = 0; z < CUBE_DEPTH; ++z) {
                // Create a wave pattern
                float wave = std::sin(waveTime + x * 0.2f + y * 0.1f + z * 0.3f);
                float intensity = (wave + 1.0f) * 0.5f;
                
                Color color(
                    static_cast<uint8_t>(waveColor.r * intensity),
                    static_cast<uint8_t>(waveColor.g * intensity),
                    static_cast<uint8_t>(waveColor.b * intensity)
                );
                
                cube.setLED(Position(x, y, z), color);
            }
        }
    }
}

void WaveAnimation::reset() {
    waveTime = 0.0;
    currentTime = 0.0;
}

// CubeRotationAnimation implementation
CubeRotationAnimation::CubeRotationAnimation() {
}

void CubeRotationAnimation::init() {
    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;
}

void CubeRotationAnimation::update(double deltaTime) {
    currentTime += deltaTime * animationSpeed;
    
    rotationX += deltaTime * 0.5f * animationSpeed;
    rotationY += deltaTime * 0.3f * animationSpeed;
    rotationZ += deltaTime * 0.2f * animationSpeed;
}

void CubeRotationAnimation::render(LEDCube& cube) {
    cube.clear();
    
    // Create a rotating cube pattern
    for (int x = 0; x < CUBE_SIZE; ++x) {
        for (int y = 0; y < CUBE_SIZE; ++y) {
            for (int z = 0; z < CUBE_DEPTH; ++z) {
                // Apply rotation transformation
                float rx = x * std::cos(rotationY) - z * std::sin(rotationY);
                float ry = y * std::cos(rotationX) - z * std::sin(rotationX);
                float rz = z * std::cos(rotationZ) + x * std::sin(rotationZ);
                
                // Map to cube coordinates
                int px = static_cast<int>((rx + CUBE_SIZE/2) * CUBE_SIZE / (CUBE_SIZE * 1.5f));
                int py = static_cast<int>((ry + CUBE_SIZE/2) * CUBE_SIZE / (CUBE_SIZE * 1.5f));
                int pz = static_cast<int>((rz + CUBE_DEPTH/2) * CUBE_DEPTH / (CUBE_DEPTH * 1.5f));
                
                if (px >= 0 && px < CUBE_SIZE && 
                    py >= 0 && py < CUBE_SIZE && 
                    pz >= 0 && pz < CUBE_DEPTH) {
                    
                    // Create color based on position and time
                    Color color(
                        static_cast<uint8_t>((px * 255) / CUBE_SIZE),
                        static_cast<uint8_t>((py * 255) / CUBE_SIZE),
                        static_cast<uint8_t>((pz * 255) / CUBE_DEPTH)
                    );
                    
                    cube.setLED(Position(px, py, pz), color);
                }
            }
        }
    }
}

void CubeRotationAnimation::reset() {
    rotationX = 0.0;
    rotationY = 0.0;
    rotationZ = 0.0;
    currentTime = 0.0;
}

// GameOfLifeAnimation implementation
GameOfLifeAnimation::GameOfLifeAnimation() {
    // Initialize grids for 64x384 panel
    currentGrid.resize(64, std::vector<bool>(384, false));
    nextGrid.resize(64, std::vector<bool>(384, false));
}

void GameOfLifeAnimation::init() {
    initializeRandom();
    updateTimer = 0.0;
}

void GameOfLifeAnimation::update(double deltaTime) {
    currentTime += deltaTime * animationSpeed;
    updateTimer += deltaTime * animationSpeed;
    
    // Update Game of Life every interval
    if (updateTimer >= updateInterval) {
        updateTimer = 0.0;
        updateGameOfLife();
    }
}

void GameOfLifeAnimation::render(LEDCube& cube) {
    cube.clear();
    
    // Render the 2D grid to the cube
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 384; ++y) {
            if (currentGrid[x][y]) {
                // Map 2D coordinates to cube position
                // Each face is 64x64, so we need to map the 384 height to 6 faces
                int face = y / 64;
                int faceY = y % 64;
                
                Position pos(x, faceY, face);
                cube.setLED(pos, Color::Green()); // Living cells are green
            }
        }
    }
}

void GameOfLifeAnimation::reset() {
    initializeRandom();
    updateTimer = 0.0;
    currentTime = 0.0;
}

void GameOfLifeAnimation::initializeRandom() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    
    // Initialize with random pattern (about 30% alive)
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 384; ++y) {
            currentGrid[x][y] = (dist(gen) < 0.3f);
        }
    }
}

void GameOfLifeAnimation::updateGameOfLife() {
    // Compute next generation
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 384; ++y) {
            int neighbors = countNeighbors(x, y);
            bool current = currentGrid[x][y];
            
            // Conway's Game of Life rules
            bool next = false;
            if (current) {
                // Survive with 2 or 3 neighbors
                next = (neighbors == 2 || neighbors == 3);
            } else {
                // Born with exactly 3 neighbors
                next = (neighbors == 3);
            }
            
            nextGrid[x][y] = next;
        }
    }
    
    // Swap grids
    currentGrid.swap(nextGrid);
}

int GameOfLifeAnimation::countNeighbors(int x, int y) {
    int count = 0;
    
    // Check all 8 neighbors with wrapping
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue; // Skip self
            
            int nx = (x + dx + 64) % 64;  // Wrap X
            int ny = (y + dy + 384) % 384; // Wrap Y
            
            if (getCell(nx, ny)) {
                count++;
            }
        }
    }
    
    return count;
}

bool GameOfLifeAnimation::getCell(int x, int y) {
    if (x >= 0 && x < 64 && y >= 0 && y < 384) {
        return currentGrid[x][y];
    }
    return false;
}

void GameOfLifeAnimation::setCell(int x, int y, bool alive) {
    if (x >= 0 && x < 64 && y >= 0 && y < 384) {
        currentGrid[x][y] = alive;
    }
}

} // namespace LEDCube 