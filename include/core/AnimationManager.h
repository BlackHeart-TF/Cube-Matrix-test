#pragma once

#include "Animation.h"
#include "LEDCube.h"
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

namespace LEDCube {

class AnimationManager {
public:
    AnimationManager();
    ~AnimationManager();
    
    // Animation management
    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(const std::string& name);
    void clearAnimations();
    
    // Animation control
    void playAnimation(const std::string& name);
    void stopAnimation();
    void pauseAnimation();
    void resumeAnimation();
    
    // Animation state
    bool isPlaying() const { return currentAnimation != nullptr; }
    std::string getCurrentAnimationName() const;
    std::shared_ptr<Animation> getCurrentAnimation() const { return currentAnimation; }
    
    // Update and render
    void update(double deltaTime);
    void render(LEDCube& cube);
    
    // Animation list
    std::vector<std::string> getAnimationNames() const;
    std::shared_ptr<Animation> getAnimation(const std::string& name) const;
    
    // Animation control
    void resetCurrentAnimation();
    
    // Built-in animations
    void createBuiltInAnimations();

private:
    std::unordered_map<std::string, std::shared_ptr<Animation>> animations;
    std::shared_ptr<Animation> currentAnimation;
    bool isPaused = false;
    
    // Built-in animation creators
    void createRainAnimation();
    void createWaveAnimation();
    void createCubeRotationAnimation();
    void createTestPatternAnimation();
    void createGameOfLifeAnimation();
};

} // namespace LEDCube 