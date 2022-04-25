CC = g++

NAME = Engine
SRC = lib/glad/src/glad.c $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) $(wildcard src/**/**/**/**/*.cpp) 
RES = res
BUILD = build
BIN = bin

LIBS_STATIC = -static -lglfw3 -opengl32 -lgdi32 -lassimp -lminizip -lz -static-libgcc -static-libstdc++ -lpthread
LIBS_DYN = -lglfw3.dll -lassimp.dll

CFLAGS = -std=c++20 -Wall -Wextra -g3 -Wno-narrowing
INCLUDES = -Ilib -Ilib/glad/include -Isrc -Isrc/Engine
CFLAGS += $(INCLUDES)

PCH = lib/libs.hpp

debug:
	$(CC) $(SRC) $(CFLAGS) $(LIBS_DYN) -o $(BIN)/$(NAME)

build: copy_res
	$(CC) $(SRC) $(CFLAGS) $(LIBS_STATIC) -o $(BUILD)/$(NAME)

pch:
	$(CC) -c $(PCH) $(CFLAGS)

copy_res: clean_res
	cp -r $(RES) $(BUILD)/$(RES)

clean_res:
	rm -r -f $(BUILD)/$(RES)
