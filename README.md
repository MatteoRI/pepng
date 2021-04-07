# PEPNG Template

## Overview

This is the official template for [PEPNG](https://github.com/pepng-CU/pepng).

## Getting Started

You will need the minimal C++ build utils (git, cmake, make, ...). If you intend using WebGL, you will also need [emsdk](https://github.com/emscripten-core/emsdk) (this repo uses the [official Docker container](https://hub.docker.com/r/emscripten/emsdk)).

### OpenGL

To build in OpenGL, you can run the `opengl.sh` script or simply use:

```
cmake -B ./build -S .
cmake --build ./build
```

You will find an executable in the `bin` folder named `main`.

### WebGL

To build in WebGL, you can run the `webgl.sh` script. The script assumes that you are using the Docker image. You can also use the raw commands:

```
emcmake cmake -B ./build -S .
cmake --build ./build
```

You will find the necessary files in the `bin` folder. The compiler includes the files in the `models`, `textures`, and `shaders` folders by default. You will need to host the folder to run the build. You can use the following Python module (or equivalent) in the `bin` folder.

```
python -m http.server
```

If you use the previous command, you should find the build at http://localhost:8000/main.html.

## Common Issues

This section outlines common issues regarding this template.

### Git Submodules

Git submodules often don't pull as expected. You can run the following command manually:

```
git submodule update --init --recursive
```

If the issue persists, it is possible that the repo doesn't contain a `.git` folder. You can remedy this by cloning the repo. If you cannot have access to the target repo, you can manually run the following in the `thirdparty` folder:

```
git clone https://github.com/pepng-CU/pepng
```

It is possible that you will again need to download the git submodules, therefore refer to the start of this section.