# Qt/QML/C++ Custom Libraries
This repository contains a collection of **custom Qt/QML/C++ components and libraries** designed to extend and simplify application development.
Each module is implemented as a**self-contained subdirectory** with its own `CMakeLists.txt`, examples, and documentation.

This repository is structured for **modularity**: you can either use the entire repo as a project dependency or include only specific components as needed.
---

## Structure
```bash
root
├── CMakeLists.txt # Root CMake
├── Chart/ 	   # High-performance OpenGL chart
│   ├── CMakeLists.txt
│   ├── README.md
│   └── ...
├── ListModel/     # Extended QListModel with extra functionality
│   ├── CMakeLists.txt
│   ├── README.md
│   └── ...
├── Tools/ 	   # Others
│   ├── CMakeLists.txt
│   └── fps.h
└── ...
```
---

## Available Components
- **Chart**
A high-performance chart built with **C++/OpenGL/QML**, supporting multiple series types (`Scatter`, `Line`, `Area`), interactive features (Pan, Zoom, Selection and ...), and customizable visuals.
--> See [Chart/README.md](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/Chart/README.md)
- **CustomListModel**
An extended **QListModel** with additional features for handling structured data and QML integration.
--> See [ListModel/README.md](http://172.16.50.13/parsa/qcustommodels/-/blob/parsa-chart/ListModel/README.md)
- **Tools**
A base tools object like `FPS` counter and ...


(More components will be added over time.)

---

## Notes
- Each component has its own **README.md** with detailed documentation.
- The repo is designed to be used as a **submodule** or **standalone library**.
- Minimum requirements:
        - C++11
        - Qt 5.12+
        - CMake 3.14+
---
