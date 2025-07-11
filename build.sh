#!/bin/bash

# LED Cube Matrix Build Script
# This script builds either GPIO mode (for Raspberry Pi) or OpenGL mode (for desktop)

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're on a Raspberry Pi
is_raspberry_pi() {
    if [ -f /proc/cpuinfo ]; then
        grep -q "Raspberry Pi" /proc/cpuinfo
        return $?
    fi
    return 1
}

# Install dependencies for OpenGL mode
install_opengl_deps() {
    print_status "Installing OpenGL dependencies..."
    
    if command -v pacman &> /dev/null; then
        # Arch Linux
        sudo pacman -S --needed cmake glfw-x11 glm
    elif command -v apt-get &> /dev/null; then
        # Debian/Ubuntu
        sudo apt-get update
        sudo apt-get install -y cmake libglfw3-dev libglm-dev
    elif command -v dnf &> /dev/null; then
        # Fedora
        sudo dnf install -y cmake glfw-devel glm-devel
    else
        print_warning "Could not detect package manager. Please install cmake, glfw3, and glm manually."
    fi
}

# Install dependencies for GPIO mode
install_gpio_deps() {
    print_status "Installing GPIO dependencies..."
    
    if command -v pacman &> /dev/null; then
        # Arch Linux
        sudo pacman -S --needed cmake wiringpi
    elif command -v apt-get &> /dev/null; then
        # Debian/Ubuntu
        sudo apt-get update
        sudo apt-get install -y cmake wiringpi
    elif command -v dnf &> /dev/null; then
        # Fedora
        sudo dnf install -y cmake wiringpi
    else
        print_warning "Could not detect package manager. Please install cmake and wiringpi manually."
    fi
}

# Build function
build() {
    local mode=$1
    local build_dir="build_${mode}"
    
    print_status "Building in ${mode} mode..."
    
    # Create build directory
    mkdir -p "$build_dir"
    cd "$build_dir"
    
    # Configure with CMake
    if [ "$mode" = "gpio" ]; then
        cmake .. -DBUILD_GPIO_MODE=ON -DBUILD_OPENGL_MODE=OFF
    else
        cmake .. -DBUILD_GPIO_MODE=OFF -DBUILD_OPENGL_MODE=ON
    fi
    
    # Build
    make -j$(nproc)
    
    print_success "Build completed successfully!"
    print_status "Executable created: ${build_dir}/LEDCubeMatrix"
    
    cd ..
}

# Main script
main() {
    print_status "LED Cube Matrix Build Script"
    print_status "============================"
    
    # Check if we're on a Raspberry Pi
    if is_raspberry_pi; then
        print_status "Detected Raspberry Pi - GPIO mode recommended"
        MODE="gpio"
    else
        print_status "Detected desktop system - OpenGL mode recommended"
        MODE="opengl"
    fi
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            --gpio)
                MODE="gpio"
                shift
                ;;
            --opengl)
                MODE="opengl"
                shift
                ;;
            --install-deps)
                if [ "$MODE" = "gpio" ]; then
                    install_gpio_deps
                else
                    install_opengl_deps
                fi
                exit 0
                ;;
            --help|-h)
                echo "Usage: $0 [OPTIONS]"
                echo ""
                echo "Options:"
                echo "  --gpio          Build in GPIO mode (for Raspberry Pi)"
                echo "  --opengl        Build in OpenGL mode (for desktop)"
                echo "  --install-deps  Install dependencies for current mode"
                echo "  --help, -h      Show this help message"
                echo ""
                echo "If no mode is specified, the script will auto-detect:"
                echo "  - GPIO mode on Raspberry Pi"
                echo "  - OpenGL mode on desktop systems"
                exit 0
                ;;
            *)
                print_error "Unknown option: $1"
                echo "Use --help for usage information"
                exit 1
                ;;
        esac
    done
    
    # Check if cmake is available
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is not installed. Please install it first."
        exit 1
    fi
    
    # Build the selected mode
    if [ "$MODE" = "gpio" ]; then
        print_status "Building GPIO mode for Raspberry Pi..."
        build "gpio"
    else
        print_status "Building OpenGL mode for desktop..."
        build "opengl"
    fi
    
    print_success "Build completed!"
    print_status "To run: ./build_${MODE}/LEDCubeMatrix"
}

# Run main function with all arguments
main "$@" 