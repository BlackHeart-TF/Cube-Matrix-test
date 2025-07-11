#pragma once

#include "LEDCube.h"
#include <functional>
#include <string>
#include <memory>

namespace LEDCube {

// Animation interface
class Animation {
public:
    virtual ~Animation() = default;
    
    // Core animation methods
    virtual void init() = 0;
    virtual void update(double deltaTime) = 0;
    virtual void render(LEDCube& cube) = 0;
    virtual void reset() = 0;
    
    // Animation properties
    virtual std::string getName() const = 0;
    virtual bool isFinished() const = 0;
    virtual double getDuration() const = 0;
    
    // Animation state
    void setSpeed(double speed) { animationSpeed = speed; }
    double getSpeed() const { return animationSpeed; }
    
    void setLooping(bool loop) { isLooping = loop; }
    bool getLooping() const { return isLooping; }

protected:
    double animationSpeed = 1.0;
    bool isLooping = true;
    double currentTime = 0.0;
};

// Function-based animation (for quick prototyping)
class FunctionAnimation : public Animation {
public:
    using AnimationFunction = std::function<void(LEDCube&, double)>;
    
    FunctionAnimation(const std::string& name, 
                     AnimationFunction func, 
                     double duration = 0.0);
    
    void init() override {}
    void update(double deltaTime) override;
    void render(LEDCube& cube) override;
    void reset() override;
    
    std::string getName() const override { return name; }
    bool isFinished() const override;
    double getDuration() const override { return duration; }

private:
    std::string name;
    AnimationFunction animationFunc;
    double duration;
    bool finished = false;
};

// Built-in animations
class RainAnimation : public Animation {
public:
    RainAnimation();
    
    void init() override;
    void update(double deltaTime) override;
    void render(LEDCube& cube) override;
    void reset() override;
    
    std::string getName() const override { return "Rain"; }
    bool isFinished() const override { return false; }
    double getDuration() const override { return 0.0; }
    
    // Set gravity direction based on cube rotation
    void setGravityDirection(float pitch, float yaw);

private:
    struct RainDrop {
        Position pos;
        double speed;
        Color color;
    };
    
    std::vector<RainDrop> drops;
    double spawnTimer = 0.0;
    
    // Gravity direction (normalized vector)
    float gravityX = 0.0f;
    float gravityY = -1.0f;  // Default: falling down
    float gravityZ = 0.0f;
};

class WaveAnimation : public Animation {
public:
    WaveAnimation();
    
    void init() override;
    void update(double deltaTime) override;
    void render(LEDCube& cube) override;
    void reset() override;
    
    std::string getName() const override { return "Wave"; }
    bool isFinished() const override { return false; }
    double getDuration() const override { return 0.0; }

private:
    double waveTime = 0.0;
    Color waveColor = Color::Cyan();
};

class CubeRotationAnimation : public Animation {
public:
    CubeRotationAnimation();
    
    void init() override;
    void update(double deltaTime) override;
    void render(LEDCube& cube) override;
    void reset() override;
    
    std::string getName() const override { return "Cube Rotation"; }
    bool isFinished() const override { return false; }
    double getDuration() const override { return 0.0; }

private:
    double rotationX = 0.0;
    double rotationY = 0.0;
    double rotationZ = 0.0;
};

class GameOfLifeAnimation : public Animation {
public:
    GameOfLifeAnimation();
    
    void init() override;
    void update(double deltaTime) override;
    void render(LEDCube& cube) override;
    void reset() override;
    
    std::string getName() const override { return "Game of Life"; }
    bool isFinished() const override { return false; }
    double getDuration() const override { return 0.0; }

private:
    // 2D grid for the unfolded cube (64x384)
    std::vector<std::vector<bool>> currentGrid;
    std::vector<std::vector<bool>> nextGrid;
    
    // Game of Life parameters
    double updateTimer = 0.0;
    double updateInterval = 0.5; // Update every 0.5 seconds
    
    // Helper methods
    void initializeRandom();
    void updateGameOfLife();
    int countNeighbors(int x, int y);
    bool getCell(int x, int y);
    void setCell(int x, int y, bool alive);
};

} // namespace LEDCube 