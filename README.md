# LED Cube Matrix

A C++ application for controlling an Adafruit 64x64x6 LED panel cube with support for both Raspberry Pi GPIO mode and desktop OpenGL preview mode.

## Features

- **Dual Mode Support**: Run in GPIO mode on Raspberry Pi or OpenGL preview mode on desktop
- **64x64x6 LED Matrix**: Full support for the Adafruit LED panel cube
- **Animation System**: Built-in animations and custom animation support
- **Real-time Rendering**: Smooth 60 FPS animation playback
- **Cross-platform**: Works on Linux systems (Arch, Debian, Ubuntu, Fedora)

## Built-in Animations

- **Rain**: Animated raindrops falling through the cube
- **Wave**: Colorful wave patterns that ripple through the matrix
- **Cube Rotation**: 3D rotating cube with color gradients
- **Test Pattern**: Moving test pattern for hardware verification

## Project Structure

```
cpp_cube_thing/
├── CMakeLists.txt          # Main build configuration
├── build.sh               # Build script with auto-detection
├── README.md              # This file
├── include/               # Header files
│   ├── core/             # Core LED cube functionality
│   ├── gpio/             # Raspberry Pi GPIO interface
│   └── opengl/           # OpenGL preview renderer
└── src/                  # Source files
    ├── core/             # Core implementations
    ├── gpio/             # GPIO implementations
    ├── opengl/           # OpenGL implementations
    ├── main_gpio.cpp     # GPIO mode entry point
    └── main_opengl.cpp   # OpenGL mode entry point
```

## Hardware Requirements

### GPIO Mode (Raspberry Pi)
- Raspberry Pi 3 or newer
- Adafruit 64x64x6 LED panel cube
- WiringPi library
- Proper GPIO connections (see GPIO pin definitions)

### OpenGL Mode (Desktop)
- Linux system with OpenGL support
- GLFW3 library
- GLM library
- X11 display server

## GPIO Pin Connections

The application uses the following GPIO pins for the LED matrix:

| Pin | GPIO | Function |
|-----|------|----------|
| 10  | GPIO 10 | Data (SPI0 MOSI) |
| 11  | GPIO 11 | Clock (SPI0 SCLK) |
| 8   | GPIO 8  | Latch |
| 9   | GPIO 9  | Output Enable |
| 12  | GPIO 12 | Address A |
| 13  | GPIO 13 | Address B |
| 14  | GPIO 14 | Address C |
| 15  | GPIO 15 | Address D |
| 16  | GPIO 16 | Reset |
| 17  | GPIO 17 | Blank |

## Building

### Quick Start

The build script automatically detects your system and builds the appropriate mode:

```bash
# Auto-detect and build
./build.sh

# Build specific mode
./build.sh --gpio      # Force GPIO mode
./build.sh --opengl    # Force OpenGL mode

# Install dependencies first
./build.sh --install-deps
```

### Manual Build

If you prefer to build manually:

```bash
# GPIO Mode
mkdir build_gpio
cd build_gpio
cmake .. -DBUILD_GPIO_MODE=ON -DBUILD_OPENGL_MODE=OFF
make

# OpenGL Mode
mkdir build_opengl
cd build_opengl
cmake .. -DBUILD_GPIO_MODE=OFF -DBUILD_OPENGL_MODE=ON
make
```

## Running

### GPIO Mode (Raspberry Pi)

```bash
# Run on Raspberry Pi with LED matrix
sudo ./build_gpio/LEDCubeMatrix
```

**Note**: GPIO mode requires root privileges for hardware access.

### OpenGL Mode (Desktop)

```bash
# Run desktop preview
./build_opengl/LEDCubeMatrix
```

## Controls

### OpenGL Mode Controls
- **1-4**: Switch between animations
- **Space**: Pause/Resume animation
- **R**: Reset current animation
- **Mouse**: Rotate camera view
- **Scroll**: Zoom in/out
- **Escape**: Exit application

### GPIO Mode Controls
- **Ctrl+C**: Exit application
- Animations cycle automatically every 10 seconds

## Creating Custom Animations

You can create custom animations by implementing the `Animation` interface:

```cpp
class MyCustomAnimation : public Animation {
public:
    void init() override {
        // Initialize animation state
    }
    
    void update(double deltaTime) override {
        // Update animation logic
    }
    
    void render(LEDCube& cube) override {
        // Render to LED cube
    }
    
    std::string getName() const override {
        return "My Custom Animation";
    }
    
    bool isFinished() const override {
        return false; // Loop forever
    }
    
    double getDuration() const override {
        return 0.0; // No fixed duration
    }
};
```

Or use the `FunctionAnimation` for quick prototyping:

```cpp
auto myAnimation = std::make_shared<FunctionAnimation>("Quick Animation", 
    [](LEDCube& cube, double time) {
        // Your animation code here
        cube.clear();
        // ... set LEDs based on time
    });
```

## Development Workflow

1. **Desktop Development**: Use OpenGL mode for rapid iteration
2. **Hardware Testing**: Deploy to Raspberry Pi with GPIO mode
3. **Animation Creation**: Develop animations using the common API
4. **Testing**: Verify animations work in both modes

## Troubleshooting

### Common Issues

**Build fails with missing dependencies:**
```bash
./build.sh --install-deps
```

**GPIO mode requires root:**
```bash
sudo ./build_gpio/LEDCubeMatrix
```

**OpenGL mode fails to start:**
- Ensure X11 is running
- Check that GLFW and OpenGL libraries are installed
- Verify display permissions

**LED matrix not responding:**
- Check GPIO pin connections
- Verify wiringPi is installed
- Ensure proper power supply to LED matrix

### Debug Information

The application provides verbose output about:
- Animation loading and switching
- Hardware initialization
- Performance metrics
- Error conditions

## Performance

- **Target FPS**: 60 FPS for smooth animations
- **Memory Usage**: ~24KB for LED buffer (64x64x6 RGB)
- **CPU Usage**: Minimal in GPIO mode, moderate in OpenGL mode
- **Latency**: <16ms for real-time responsiveness

## License

This project is open source. Feel free to modify and distribute according to your needs.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add your custom animations or improvements
4. Test in both GPIO and OpenGL modes
5. Submit a pull request

## Future Enhancements

- [ ] Web interface for remote control
- [ ] Audio-reactive animations
- [ ] Network synchronization for multiple cubes
- [ ] More sophisticated 3D effects
- [ ] Animation timeline editor
- [ ] MIDI input support 