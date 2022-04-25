# ProtoEngine

A prototype of an engine written in c++ and OpenGL I´m working on, based on the learnopengl.com tutorial

## Running

In the build directory there is an executable and the "res" directory. As long as this executable and the "res" directory are in the same folder, you can execute the program.

## Building

It´s compiled using make (mingw32-make)

For Compiling yourself you need the following libraries installed:

glm, GLFW, boost, stb, Assimp

The included Makefile has two options for compiling:

### debug

Creates an executable without statically linked libraries. Found in the "bin" directory.

### build

Copies the "res" directory into the "build" directory and creates an executable with static linking. Can be found in the "build" directory.
