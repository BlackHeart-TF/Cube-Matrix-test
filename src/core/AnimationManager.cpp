#include "core/AnimationManager.h"
#include <iostream>

namespace LEDCube {

AnimationManager::AnimationManager() {
    createBuiltInAnimations();
}

AnimationManager::~AnimationManager() {
}

void AnimationManager::addAnimation(std::shared_ptr<Animation> animation) {
    if (animation) {
        animations[animation->getName()] = animation;
    }
}

void AnimationManager::removeAnimation(const std::string& name) {
    animations.erase(name);
    
    // If we're currently playing this animation, stop it
    if (currentAnimation && currentAnimation->getName() == name) {
        stopAnimation();
    }
}

void AnimationManager::clearAnimations() {
    animations.clear();
    stopAnimation();
}

void AnimationManager::playAnimation(const std::string& name) {
    auto it = animations.find(name);
    if (it != animations.end()) {
        currentAnimation = it->second;
        currentAnimation->reset();
        currentAnimation->init();
        isPaused = false;
    } else {
        std::cerr << "Animation '" << name << "' not found!" << std::endl;
    }
}

void AnimationManager::stopAnimation() {
    currentAnimation.reset();
    isPaused = false;
}

void AnimationManager::pauseAnimation() {
    isPaused = true;
}

void AnimationManager::resumeAnimation() {
    isPaused = false;
}

std::string AnimationManager::getCurrentAnimationName() const {
    if (currentAnimation) {
        return currentAnimation->getName();
    }
    return "";
}

void AnimationManager::update(double deltaTime) {
    if (currentAnimation && !isPaused) {
        currentAnimation->update(deltaTime);
        
        // Check if animation finished and should loop
        if (currentAnimation->isFinished() && !currentAnimation->getLooping()) {
            stopAnimation();
        }
    }
}

void AnimationManager::render(LEDCube& cube) {
    if (currentAnimation && !isPaused) {
        currentAnimation->render(cube);
    }
}

std::vector<std::string> AnimationManager::getAnimationNames() const {
    std::vector<std::string> names;
    names.reserve(animations.size());
    
    for (const auto& pair : animations) {
        names.push_back(pair.first);
    }
    
    return names;
}

std::shared_ptr<Animation> AnimationManager::getAnimation(const std::string& name) const {
    auto it = animations.find(name);
    if (it != animations.end()) {
        return it->second;
    }
    return nullptr;
}

void AnimationManager::resetCurrentAnimation() {
    if (currentAnimation) {
        currentAnimation->reset();
        currentAnimation->init();
    }
}

void AnimationManager::createBuiltInAnimations() {
    // Create built-in animations
    createRainAnimation();
    createWaveAnimation();
    createCubeRotationAnimation();
    createTestPatternAnimation();
    createGameOfLifeAnimation();
}

void AnimationManager::createRainAnimation() {
    auto animation = std::make_shared<RainAnimation>();
    addAnimation(animation);
}

void AnimationManager::createWaveAnimation() {
    auto animation = std::make_shared<WaveAnimation>();
    addAnimation(animation);
}

void AnimationManager::createCubeRotationAnimation() {
    auto animation = std::make_shared<CubeRotationAnimation>();
    addAnimation(animation);
}

void AnimationManager::createTestPatternAnimation() {
    // Create a test pattern animation using FunctionAnimation
    auto testPattern = [](LEDCube& cube, double time) {
        cube.clear();
        
        // Create a moving test pattern
        int patternX = static_cast<int>(time * 10) % CUBE_SIZE;
        int patternY = static_cast<int>(time * 8) % CUBE_SIZE;
        int patternZ = static_cast<int>(time * 6) % CUBE_DEPTH;
        
        // Draw a cross pattern
        for (int i = 0; i < CUBE_SIZE; ++i) {
            cube.setLED(Position(patternX, i, patternZ), Color::Red());
            cube.setLED(Position(i, patternY, patternZ), Color::Green());
        }
        
        // Add some random dots
        for (int i = 0; i < 10; ++i) {
            int x = (patternX + i * 7) % CUBE_SIZE;
            int y = (patternY + i * 5) % CUBE_SIZE;
            int z = (patternZ + i * 3) % CUBE_DEPTH;
            
            cube.setLED(Position(x, y, z), Color::Blue());
        }
    };
    
    auto animation = std::make_shared<FunctionAnimation>("Test Pattern", testPattern, 0.0);
    addAnimation(animation);
}

void AnimationManager::createGameOfLifeAnimation() {
    auto animation = std::make_shared<GameOfLifeAnimation>();
    addAnimation(animation);
}

} // namespace LEDCube 