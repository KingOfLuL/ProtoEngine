# ProtoEngine

A prototype of an engine written in c++ and OpenGL I´m working on, based on the learnopengl.com tutorial. Currently only for Windows.

## Running

In the build directory there is an executable and the "res" directory. As long as this executable and the "res" directory are in the same folder, you can execute the program.

## Building

It´s compiled using make (mingw32-make)

For Compiling yourself you need the following libraries installed:

glm, GLFW, boost, stb, Assimp

The options for compiling:

### debug_lib

Compiles the engine into the library and compiles the application into an executable. The library is linked dynamically. Is meant for testing the engine itself.

### debug_app

Compiles to an executable without (re)creating the engine-library. Is meant to test the application without changing the inner workings of the engine.

### build

Creates the entire build: Compiles both the engine into the library and the files for the application to the executable. Also copies the "res" directory into the "build" directory. The library is linked statically.

### []_run

The three previous options have versions with the suffix "_run". Simply runs the respective executable.