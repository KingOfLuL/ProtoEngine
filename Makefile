CC = g++

NAME = Engine
SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) $(wildcard src/**/**/**/**/*.cpp) 
RES = res
BUILD = build
BIN = bin

LIBS_STATIC = -static -lglfw3 -opengl32 -lgdi32 -lassimp -lminizip -lz -static-libgcc -static-libstdc++ -lpthread -Llib/glad/bin -lGLAD.dll
LIBS_DYN = -lglfw3.dll -lassimp.dll -Llib/glad/bin -lGLAD

CFLAGS = -std=c++20 -Wall -Wextra -g3 -Wno-narrowing
INCLUDES = -Ilib -Ilib/glad/include -Isrc -Isrc/Engine
CFLAGS += $(INCLUDES)

PCH = lib/libs.hpp

debug:
	$(CC) $(SRC) $(CFLAGS) $(LIBS_DYN) -o $(BIN)/$(NAME)


build:
	rm -r -f $(BUILD)/$(RES);		\
	cp -r $(RES) $(BUILD)/$(RES);	\
	$(CC) $(SRC) $(CFLAGS) $(LIBS_STATIC) -o $(BUILD)/$(NAME);


pch:
	$(CC) -c $(PCH) $(CFLAGS)


glad_static:
	cd lib/glad; \
	make static;

glad_shared:
	cd lib/glad; \
	make shared;

.PHONY: build debug