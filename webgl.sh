#!/bin/sh

sudo docker run --rm -v "$(pwd)":/src -u $(id -u):$(id -g) emscripten/emsdk emcmake cmake -B ./build -S .
sudo docker run --rm -v "$(pwd)":/src -u $(id -u):$(id -g) emscripten/emsdk cmake --build ./build