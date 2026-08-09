# Simulation of a Hypothetical White Hole

C++ simulation and visualizer using GLFW for a theoretical white hole.

## Dependencies

* **C++17 Compiler** (GCC, Clang, or MSVC)
* **GLFW3**
* **OpenGL**

### Installation

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get install libglfw3-dev libgl1-mesa-dev
```

**macOS:**
```bash
brew install glfw
```

**Windows:**
* Install MinGW-w64 (for GCC) or Visual Studio (for MSVC).
* Download the Windows pre-compiled binaries from the [GLFW website](https://glfw.org).

---

## Build & Run

### Linux (GCC)
```bash
g++ -std=c++17 whitehole_glfw.cpp -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -o whitehole_sim
./whitehole_sim
```

### macOS (Clang)
```bash
clang++ -std=c++17 whitehole_glfw.cpp -lglfw -framework OpenGL -o whitehole_sim
./whitehole_sim
```

### Windows (MinGW / g++)
```bash
g++ -std=c++17 whitehole_glfw.cpp -lglfw3 -lopengl32 -lgdi32 -luser32 -lkernel32 -o whitehole_sim.exe
whitehole_sim.exe
```
*Note: If GLFW is not in your system path, append `-I<path_to_include>` and `-L<path_to_lib>`.*

### Windows (Visual Studio / MSVC CLI)
Open the *Developer Command Prompt for VS* and run:
```cmd
cl /std:c++17 whitehole_glfw.cpp /link glfw3.lib opengl32.lib gdi32.lib user32.lib shell32.lib /out:whitehole_sim.exe
whitehole_sim.exe
```
