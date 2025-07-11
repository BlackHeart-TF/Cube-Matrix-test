#include "opengl/CubeRenderer.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace LEDCube {

// Simple vertex and fragment shaders for textured cube
static const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uViewProj;
uniform mat4 uModel;

out vec2 TexCoord;

void main() {
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

static const char* fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;

void main() {
    FragColor = texture(uTexture, TexCoord);
}
)";

CubeRenderer::CubeRenderer()
    : cubeVAO(0), cubeVBO(0), cubeEBO(0), cubeShader(0), initialized(false), cubeScale(1.0f) {
    // Initialize texture IDs for 6 faces
    for (int i = 0; i < 6; ++i) {
        faceTextures[i] = 0;
    }
}

CubeRenderer::~CubeRenderer() {
    shutdown();
}

bool CubeRenderer::initialize() {
    if (initialized) return true;
    
    // Compile shaders
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertexShaderSource, nullptr);
    glCompileShader(vert);
    
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragmentShaderSource, nullptr);
    glCompileShader(frag);
    
    cubeShader = glCreateProgram();
    glAttachShader(cubeShader, vert);
    glAttachShader(cubeShader, frag);
    glLinkProgram(cubeShader);
    
    glDeleteShader(vert);
    glDeleteShader(frag);
    
    // Create cube geometry (6 faces, each with 2 triangles)
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        
        // Back face
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        
        // Right face
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        
        // Bottom face
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f
    };
    
    unsigned int indices[] = {
        0,  1,  2,  2,  3,  0,   // Front
        4,  5,  6,  6,  7,  4,   // Back
        8,  9,  10, 10, 11, 8,   // Left
        12, 13, 14, 14, 15, 12,  // Right
        16, 17, 18, 18, 19, 16,  // Top
        20, 21, 22, 22, 23, 20   // Bottom
    };
    
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Create textures for each face
    for (int i = 0; i < 6; ++i) {
        glGenTextures(1, &faceTextures[i]);
        glBindTexture(GL_TEXTURE_2D, faceTextures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        
        // Initialize with test pattern
        std::vector<uint8_t> testData(64 * 64 * 3, 0);
        for (int j = 0; j < 64 * 64; ++j) {
            testData[j * 3 + 0] = (i * 40) % 255;  // R
            testData[j * 3 + 1] = (i * 60) % 255;  // G
            testData[j * 3 + 2] = (i * 80) % 255;  // B
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, testData.data());
    }
    
    initialized = true;
    return true;
}

void CubeRenderer::shutdown() {
    if (!initialized) return;
    
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);
    glDeleteProgram(cubeShader);
    
    for (int i = 0; i < 6; ++i) {
        glDeleteTextures(1, &faceTextures[i]);
    }
    
    initialized = false;
}

void CubeRenderer::renderCube(const LEDCube& cube, const glm::mat4& viewProj, const glm::mat4& model) {
    if (!initialized) return;
    
    // Update textures from cube data
    updateTextures(cube);
    
    glUseProgram(cubeShader);
    glBindVertexArray(cubeVAO);
    
    // Set view-projection matrix
    glUniformMatrix4fv(glGetUniformLocation(cubeShader, "uViewProj"), 1, GL_FALSE, glm::value_ptr(viewProj));
    
    // Draw each face with its texture
    for (int face = 0; face < 6; ++face) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, faceTextures[face]);
        glUniform1i(glGetUniformLocation(cubeShader, "uTexture"), 0);
        
        // Set model matrix for this face (now passed in as parameter)
        glUniformMatrix4fv(glGetUniformLocation(cubeShader, "uModel"), 1, GL_FALSE, glm::value_ptr(model));
        
        // Draw this face (4 vertices per face, 6 faces total)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(face * 6 * sizeof(unsigned int)));
    }
    
    glBindVertexArray(0);
}

void CubeRenderer::updateTextures(const LEDCube& cube) {
    // Map the 2D panel data to 6 cube faces
    // Each face is 64x64, so we extract 64x64 sections from the panel
    
    for (int face = 0; face < 6; ++face) {
        std::vector<uint8_t> faceData(64 * 64 * 3);
        
        // Extract this face's data from the panel
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                // Map panel coordinates to cube face
                int panelX = x + face * 64;  // Each face is 64 pixels wide
                int panelY = y;
                
                // Get color from cube data
                Position pos(x, y, face);
                Color color = cube.getLED(pos);
                
                int idx = (y * 64 + x) * 3;
                faceData[idx + 0] = color.r;
                faceData[idx + 1] = color.g;
                faceData[idx + 2] = color.b;
            }
        }
        
        // Update texture
        glBindTexture(GL_TEXTURE_2D, faceTextures[face]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, faceData.data());
    }
}

void CubeRenderer::setCubeScale(float scale) { cubeScale = scale; }

} // namespace LEDCube 