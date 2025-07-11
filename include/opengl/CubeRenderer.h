#pragma once

#include "../core/LEDCube.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace LEDCube {

class CubeRenderer {
public:
    CubeRenderer();
    ~CubeRenderer();
    bool initialize();
    void shutdown();
    void renderCube(const LEDCube& cube, const glm::mat4& viewProj, const glm::mat4& model);
    void setCubeScale(float scale);
private:
    void updateTextures(const LEDCube& cube);
    
    GLuint cubeVAO, cubeVBO, cubeEBO, cubeShader;
    GLuint faceTextures[6];  // One texture per cube face
    bool initialized;
    float cubeScale;
};

} // namespace LEDCube 