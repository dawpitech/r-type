# R-Type

R-Type is a side-scrolling shooter game inspired by the classic arcade experience. You control a spaceship, fighting enemy waves and bosses, aiming to survive and advance through stages.

## Features

- Networked coop mode
- Easy behavior scripting
- Power-ups and ship upgrades
- Updated graphics and sound, with a retro feel

## Installation

### Windows

1. Download the latest release from [GitHub Releases](https://github.com/dawpitech/r-type/releases).
2. Extract the zip file.
3. Run
   ```powershell
   r-type.exe
   ```

### Linux

1. Download the latest release from [GitHub Releases](https://github.com/dawpitech/r-type/releases).
2. Extract the archive.
3. Open a terminal in the extracted folder.
4. Run:
   ```bash
   ./r-type
   ```

### macOS

macOS is not officially supported, but you can try building from source (see below).

## How to Play

- Use arrow keys or WASD to move.
- Shoot with Space or Ctrl.
- Avoid enemy fire and collect power-ups.
- Defeat bosses to progress.

## Multiplayer

You can run a server using the `r-type_server` executable. You need to provide a port to bind onto, for example:
```bash
./r-type_server 4242
```
Clients can then connect to your server using the game client.

## Support & Feedback

Please report issues or suggestions via [GitHub Issues](https://github.com/dawpitech/r-type/issues).

## License

See [LICENSE](LICENSE) for details.

## Building from Source

### Recommended: Using the Justfile

Note: this step only works on Linux, for Windows please use the instructions in "Manual Build on Windows".
If you have [`just`](https://github.com/casey/just) installed, simply run :

```bash
just release
```

### Manual Build on Linux (CMake)

If you do not wish to use `just`, you can build manually:

1. Clone the repository:
   ```bash
   git clone https://github.com/dawpitech/r-type.git
   cd r-type
   ```

2. Create the build directory and run CMake:
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   ```

3. Build the project:
   ```bash
   cmake --build build -j$(nproc)
   ```
   
### Manual Build on Windows (CMake & Visual Studio)

1. Download and install [CMake for Windows](https://cmake.org/download/)

2. Download [Visual Studio 2022 Community Edition](https://visualstudio.microsoft.com/fr/) and install the C++ toolchain from the installer.

3. Clone the repository:
   ```bash
   git clone https://github.com/dawpitech/r-type.git
   cd r-type
   ```

4. Create the build directory and run CMake:
   ```bash
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
   ```

5. Build the project:
   ```bash
   cmake --build build --config Release -j%NUMBER_OF_PROCESSORS%
   ```

## Dev guidelines

Contributions are welcomed. You can check the GitHub profile for an overview of what needs to be done for future updates.  
`CODEOWNERS` helps keep track of who is responsible for which part of the project—please update it when you make changes to ownership or responsibility.
