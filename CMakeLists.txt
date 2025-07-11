cmake_minimum_required(VERSION 3.16)
project(LEDCubeMatrix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options for different modes
option(BUILD_GPIO_MODE "Build for Raspberry Pi GPIO mode" OFF)
option(BUILD_OPENGL_MODE "Build for OpenGL preview mode" ON)

# Find required packages
find_package(PkgConfig REQUIRED)

if(BUILD_GPIO_MODE)
    # GPIO mode dependencies
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(WIRINGPI REQUIRED wiringPi)
    message(STATUS "Building in GPIO mode for Raspberry Pi")
elseif(BUILD_OPENGL_MODE)
    # OpenGL mode dependencies
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    find_package(glm REQUIRED)
    find_package(GLEW REQUIRED)
    
    message(STATUS "Building in OpenGL preview mode")
else()
    message(FATAL_ERROR "Must specify either BUILD_GPIO_MODE or BUILD_OPENGL_MODE")
endif()

# Common source files
set(COMMON_SOURCES
    src/core/LEDCube.cpp
    src/core/Animation.cpp
    src/core/AnimationManager.cpp
    src/core/MatrixBuffer.cpp
)

# Mode-specific source files
if(BUILD_GPIO_MODE)
    set(MODE_SOURCES
        src/gpio/GPIOController.cpp
        src/gpio/MatrixDriver.cpp
        src/main_gpio.cpp
    )
elseif(BUILD_OPENGL_MODE)
    set(MODE_SOURCES
        src/opengl/OpenGLRenderer.cpp
        src/opengl/Shader.cpp
        src/opengl/CubeRenderer.cpp
        src/main_opengl.cpp
    )
endif()

# Create executable
add_executable(${PROJECT_NAME} ${COMMON_SOURCES} ${MODE_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    src
)

# Link libraries based on mode
if(BUILD_GPIO_MODE)
    target_link_libraries(${PROJECT_NAME} ${WIRINGPI_LIBRARIES})
    target_compile_options(${PROJECT_NAME} PRIVATE ${WIRINGPI_CFLAGS_OTHER})
elseif(BUILD_OPENGL_MODE)
    target_link_libraries(${PROJECT_NAME} 
        OpenGL::GL 
        glfw 
        glm::glm
        GLEW::GLEW
    )
endif()

# Compiler definitions
if(BUILD_GPIO_MODE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GPIO_MODE)
elseif(BUILD_OPENGL_MODE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OPENGL_MODE)
endif() 