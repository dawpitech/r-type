# R-Type - Developer Documentation

## Overview

R-Type is a side-scrolling shooter game inspired by the classic arcade experience. The project is built with a client-server architecture enabling networked multiplayer, a behavior scripting system, and advanced entity management.

## Table of Contents

- [Architecture](#architecture)
- [Development Environment Setup](#development-environment-setup)
- [Building the Project](#building-the-project)
- [Network Architecture](#network-architecture)
- [Scripting System](#scripting-system)
- [Game Systems](#game-systems)
- [Contributing](#contributing)
- [Additional Documentation](#additional-documentation)

## Architecture

### Project Structure

The project follows a modular architecture separating client and server:

- **Client** (`r-type`): Graphical interface, input handling, rendering
- **Server** (`r-type_server`): Room management, network synchronization, shared game logic

### Technology Stack

- **Build System**: CMake
- **Language**: C++
- **Supported Platforms**: Windows, Linux, macOS

## Development Environment Setup

### Prerequisites

#### Linux

- CMake 3.x or higher
- Compatible C++ compiler (GCC/Clang recommended)
- Git
- (Optional) [Just](https://github.com/casey/just) for build automation

#### Windows

- [CMake for Windows](https://cmake.org/download/)
- [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/) with C++ toolchain
- Git

### Cloning the Repository

```bash
git clone https://github.com/dawpitech/r-type.git
cd r-type
```

## Building the Project

### Development Build

#### Linux with Just (Recommended)

```bash
just
```

#### Linux Manual (CMake)

```bash
# Configuration
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build -j$(nproc)
```

#### Windows (CMake + Visual Studio)

```bash
# Configuration
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build --config Release -j%NUMBER_OF_PROCESSORS%
```

### Debug Build

For a debug build with symbols, replace `Release` with `Debug` in the CMake commands:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
```

### Build Outputs

After building, you'll find the following executables:

- `r-type` or `r-type.exe`: Game client
- `r-type_server` or `r-type_server.exe`: Game server

## Network Architecture

### Server

The server manages multiple game rooms simultaneously.

**Launch Command**:
```bash
./r-type_server -p PORT -r NBROOMS
```

**Parameters**:
- `-p PORT`: Server listening port
- `-r NBROOMS`: Number of game rooms to create

**Example**:
```bash
./r-type_server -p 8080 -r 4
```

### Client

**Connection Command**:
```bash
./r-type -i IP -p PORT
```

**Parameters**:
- `-i IP`: Server IP address
- `-p PORT`: Server port

**Example**:
```bash
./r-type -i 192.168.1.100 -p 8080
```

### Network Protocol

The detailed network protocol is documented in `rtype-network-protocol.md`. Refer to this file for:
- Packet formats
- Connection sequences
- Game state synchronization
- Event handling

## Scripting System

The project includes an "easy behavior scripting" system for defining entity behaviors (enemies, power-ups, projectiles).

### Implementation Overview

*Note: Specific implementation details are not provided in the base README. Consult the source code to understand the scripting API.*

**To be documented**:
- Script format and syntax
- Available API for scripts
- Built-in behavior examples
- How to add new behaviors

### Core Features

1. **Networked Coop Mode**: Multiple players can play together
2. **Power-ups System**: Ship upgrade mechanics
3. **Enemy Waves**: Enemy generation and management
4. **Boss Fights**: Boss battles at the end of levels
5. **Graphics & Sound**: Retro-inspired with modern updates

### Controls

- **Movement**: Arrow keys or WASD
- **Shoot**: Space or Ctrl

### Game Flow

1. Player connects to server
2. Joins or creates a room
3. Game starts with wave progression
4. Collect power-ups to upgrade ship
5. Defeat bosses to advance stages
6. Survive and achieve high scores

## Contributing

### Contribution Workflow

1. Check the GitHub profile for upcoming tasks
2. Review the `CODEOWNERS` file to know module maintainers
3. Create a feature branch from `main`
4. Implement your changes
5. Submit a Pull Request

### Code Ownership

The `CODEOWNERS` file defines maintainers for each project module.

**Important**: Update `CODEOWNERS` when modifying code ownership or responsibilities.

### Coding Standards

*To be documented*:
- Code style guidelines
- Naming conventions
- Comment requirements
- Commit message format

### Bug Reports and Suggestions

Use [GitHub Issues](https://github.com/dawpitech/r-type/issues) to:
- Report bugs
- Propose improvements
- Request new features

**Issue Template**:
```markdown
### Description
[Clear description of the issue]

### Steps to Reproduce
1. Step one
2. Step two
3. ...

### Expected Behavior
[What should happen]

### Actual Behavior
[What actually happens]

### Environment
- OS: [Windows/Linux/macOS]
- Build: [Release/Debug]
- Version: [commit hash or release version]
```

## Additional Documentation

### Available Documents

1. **Comparative Study**: `comparative_study_r-type.pdf`
   - Technical choices analysis
   - Comparison with other implementations
   
2. **Network Protocol**: `rtype-network-protocol.md`
   - Complete protocol specification
   - Message formats
   - Communication sequences

## Dependencies

*Note: Third-party dependencies are not listed in the base README.*

**To be documented**:
- Graphics libraries used
- Networking libraries
- Audio libraries
- Dependency manager (Conan, vcpkg, etc.)

### Potential Dependencies

Based on typical game development:
- Graphics: SFML, SDL, or similar
- Networking: Asio, Boost.Asio, or custom
- Audio: OpenAL, FMOD, or similar

## Testing

*Note: Testing information is not present in the base README.*

**To be documented**:
- Testing framework used
- How to run tests
- Code coverage
- Integration tests

### Running Tests

```bash
# Placeholder - actual commands to be documented
cmake --build build --target test
```

## Performance Considerations

*To be documented*:
- Optimization strategies
- Profiling tools
- Known limitations
- Performance benchmarks

## Debugging

### Debug Build

Build with debug symbols:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

### Common Issues

*To be documented*:
- Connection issues
- Build errors
- Runtime crashes
- Platform-specific problems

## Release Process

*To be documented*:
- Version numbering
- Release checklist
- Packaging instructions
- Distribution process

## Architecture Deep Dive

*To be documented*:
- Class diagrams
- Design patterns used (ECS, State, etc.)
- Memory management strategies
- Threading model

## API Reference

*To be documented*:
- Core API documentation
- Scripting API
- Network API
- Extension points

## Contact and Support

- **Issues**: [GitHub Issues](https://github.com/dawpitech/r-type/issues)
- **Repository**: [https://github.com/dawpitech/r-type](https://github.com/dawpitech/r-type)
- **Releases**: [GitHub Releases](https://github.com/dawpitech/r-type/releases)

## Developer Notes

### Areas Requiring Documentation

This documentation needs to be enriched with:

1. **Detailed Technical Architecture**
   - Component interaction diagrams
   - Entity-Component-System details
   - State management

2. **Scripting API Documentation**
   - Complete function reference
   - Script examples
   - Best practices

3. **Dependencies and Build**
   - Exhaustive dependency list
   - Required versions
   - Development tool configuration

4. **Testing and Quality**
   - Test strategy
   - Static analysis tools
   - CI/CD pipeline

5. **Performance**
   - Optimization techniques
   - Profiling results
   - Known bottlenecks

---

**Last Updated**: 2025-11-02  
**Maintainers**: See [CODEOWNERS](CODEOWNERS)  
**Version**: Development documentation based on base README
