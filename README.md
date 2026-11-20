# OpenGL Labs

Small graphics programs built while learning C++, OpenGL and GLSL.

## Projects

| Project | Description | Controls |
| --- | --- | --- |
| [25.10 — Task 01](labs/25-10/task-01/) | Scene switching and a movable house | Space: switch scenes · W/A/S/D: move the house · Esc: exit |
| [07.11 — Task 01](labs/07-11/task-01/) | Indexed rendering of the object from `co-to-jest.h` with random vertex colors | Esc: exit |
| [07.11 — Task 02](labs/07-11/task-02/) | Scene switching and matrix-based house rotation around the X, Y and Z axes | Space: switch scenes · W/S: rotate around X · D/A: rotate around Y · Q/E: rotate around Z · Esc: exit |
| [07.11 — Task 03](labs/07-11/task-03/) | Indexed rendering of object A and two copies of object B with independent transformations | A/D: rotate object A around Z · W/S: scale one copy of object B · Esc: exit |
| [07.11 — Task 04](labs/07-11/task-04/) | Four viewports displaying the same object with different shader transformations | Esc: exit |
| [21.11 — Task 01](labs/21-11/task-01/) | 3D scene with OBJ models, colored meshes, repeated grass and two rotating rocks | W/S: rotate around X · A/D: rotate around Y · left mouse drag: rotate · =/− or scroll: move scene closer/farther · Esc: exit |

## Requirements

- C++20 compiler
- CMake 3.20+
- GLFW 3
- GLM (used for transformations in 21.11 — Task 01)
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

Run the executable with the project directory as the working directory so it can find the GLSL files. Task 21.11 also loads its OBJ models from the working directory.
