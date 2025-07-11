#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace LEDCube {

class Shader {
public:
    Shader();
    ~Shader();
    
    // Shader management
    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromStrings(const std::string& vertexSource, const std::string& fragmentSource);
    void use();
    void destroy();
    
    // Uniform setters
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);
    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);
    void setMat3(const std::string& name, const glm::mat3& value);
    void setMat4(const std::string& name, const glm::mat4& value);
    
    // Utility
    bool isValid() const { return programID != 0; }
    GLuint getProgramID() const { return programID; }

private:
    GLuint programID;
    
    // Helper methods
    bool compileShader(GLuint& shaderID, GLenum type, const std::string& source);
    bool linkProgram();
    void checkCompileErrors(GLuint shader, const std::string& type);
    GLint getUniformLocation(const std::string& name);
};

} // namespace LEDCube 