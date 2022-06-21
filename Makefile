CC = g++

NAME = Engine
SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) $(wildcard src/**/**/**/**/*.cpp)
RES = res
BUILD = build
BIN = bin

LIBS_STATIC = -Llib/glad/bin -static -lglfw3 -opengl32 -lgdi32 -lassimp -lminizip -lz -static-libgcc -static-libstdc++ -lpthread -lGLAD.dll
LIBS_DYN = -lglfw3 -lassimp -Llib/glad/bin -lGLAD

CFLAGS = -std=c++20 -Wall -Wextra -g3 -Wno-narrowing
INCLUDES = -Ilib -Ilib/glad/include -Isrc -Isrc/Engine
CFLAGS += $(INCLUDES)

PCH = lib/libs.hpp

debug:
	$(CC) $(SRC) $(CFLAGS) $(LIBS_DYN) -o $(BIN)/$(NAME)

debug_run: debug
	./bin/Engine


build:
	rm -r -f $(BUILD)/$(RES); \
	cp -r $(RES) $(BUILD)/$(RES); \
	$(CC) $(SRC) $(CFLAGS) $(LIBS_STATIC) -o $(BUILD)/$(NAME);

build_run: build
	./build/Engine


pch:
	$(CC) -c $(PCH) $(CFLAGS)


glad_static:
	cd lib/glad; \
	make static;

glad_shared:
	cd lib/glad; \
	make shared;

.PHONY: build debug debug_run build_run