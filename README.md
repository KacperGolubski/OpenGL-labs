# OpenGL Labs

Small graphics programs built while learning C++, OpenGL and GLSL.

## Projects

| Project | Description | Controls |
| --- | --- | --- |
| [25.10 — Task 01](labs/25-10/task-01/) | Scene switching and a movable house | Space: switch scenes · W/A/S/D: move the house · Esc: exit |
| [07.11 — Task 01](labs/07-11/task-01/) | Array-based and indexed rendering | Space: switch rendering modes · Esc: exit |
| [07.11 — Task 02](labs/07-11/task-02/) | Scene switching and matrix-based house rotation around the X, Y and Z axes | Space: switch scenes · W/S: rotate around X · D/A: rotate around Y · Q/E: rotate around Z · Esc: exit |

## Requirements

- C++20 compiler
- CMake 3.20+
- GLFW 3
- OpenGL 3.3 Core Profile

GLAD is included in each project.

## Build and run

Each project has its own `CMakeLists.txt`. For example, from the repository root:

```bash
cd labs/07-11/task-01
cmake -S . -B build
cmake --build build
./build/OpenGL_Learning
```

Run the executable with the project directory as the working directory so it can find the GLSL files.
