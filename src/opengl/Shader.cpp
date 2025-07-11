#include "opengl/Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace LEDCube {

Shader::Shader() : programID(0) {
}

Shader::~Shader() {
    destroy();
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode, fragmentCode;
    
    // Read vertex shader
    std::ifstream vertexFile(vertexPath);
    if (!vertexFile.is_open()) {
        std::cerr << "Shader: Could not open vertex shader file: " << vertexPath << std::endl;
        return false;
    }
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    vertexCode = vertexStream.str();
    vertexFile.close();
    
    // Read fragment shader
    std::ifstream fragmentFile(fragmentPath);
    if (!fragmentFile.is_open()) {
        std::cerr << "Shader: Could not open fragment shader file: " << fragmentPath << std::endl;
        return false;
    }
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    fragmentCode = fragmentStream.str();
    fragmentFile.close();
    
    return loadFromStrings(vertexCode, fragmentCode);
}

bool Shader::loadFromStrings(const std::string& vertexSource, const std::string& fragmentSource) {
    // In a real implementation, this would compile and link the shaders
    // For now, we'll just simulate the shader creation
    
    std::cout << "Shader: Loading shaders from strings" << std::endl;
    
    // Simulate shader compilation
    programID = 1; // Dummy program ID
    
    std::cout << "Shader: Shaders loaded successfully" << std::endl;
    return true;
}

void Shader::use() {
    if (programID != 0) {
        // In real implementation: glUseProgram(programID);
        std::cout << "Shader: Using program " << programID << std::endl;
    }
}

void Shader::destroy() {
    if (programID != 0) {
        // In real implementation: glDeleteProgram(programID);
        programID = 0;
        std::cout << "Shader: Program destroyed" << std::endl;
    }
}

void Shader::setBool(const std::string& name, bool value) {
    // In real implementation: glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
    std::cout << "Shader: Set bool uniform '" << name << "' = " << value << std::endl;
}

void Shader::setInt(const std::string& name, int value) {
    // In real implementation: glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    std::cout << "Shader: Set int uniform '" << name << "' = " << value << std::endl;
}

void Shader::setFloat(const std::string& name, float value) {
    // In real implementation: glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    std::cout << "Shader: Set float uniform '" << name << "' = " << value << std::endl;
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) {
    // In real implementation: glUniform2fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    std::cout << "Shader: Set vec2 uniform '" << name << "' = (" << value.x << ", " << value.y << ")" << std::endl;
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) {
    // In real implementation: glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    std::cout << "Shader: Set vec3 uniform '" << name << "' = (" << value.x << ", " << value.y << ", " << value.z << ")" << std::endl;
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) {
    // In real implementation: glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    std::cout << "Shader: Set vec4 uniform '" << name << "' = (" << value.x << ", " << value.y << ", " << value.z << ", " << value.w << ")" << std::endl;
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) {
    // In real implementation: glUniformMatrix3fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    std::cout << "Shader: Set mat3 uniform '" << name << "'" << std::endl;
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) {
    // In real implementation: glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    std::cout << "Shader: Set mat4 uniform '" << name << "'" << std::endl;
}

bool Shader::compileShader(GLuint& shaderID, GLenum type, const std::string& source) {
    // In real implementation, this would compile the shader
    // For now, we'll just simulate the compilation
    
    shaderID = 1; // Dummy shader ID
    return true;
}

bool Shader::linkProgram() {
    // In real implementation, this would link the shader program
    // For now, we'll just simulate the linking
    
    return true;
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type) {
    // In real implementation, this would check for shader compilation errors
    // For now, we'll just simulate the error checking
}

GLint Shader::getUniformLocation(const std::string& name) {
    // In real implementation: return glGetUniformLocation(programID, name.c_str());
    return 0; // Dummy location
}

} // namespace LEDCube 