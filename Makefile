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

GLAD_SRC = lib/glad/src
GLAD_BIN = lib/glad/bin

debug:
	$(CC) $(SRC) $(CFLAGS) $(LIBS_DYN) -o $(BIN)/$(NAME)



build: copy_res
	$(CC) $(SRC) $(CFLAGS) $(LIBS_STATIC) -o $(BUILD)/$(NAME)

copy_res: clean_res
	cp -r $(RES) $(BUILD)/$(RES)

clean_res:
	rm -r -f $(BUILD)/$(RES)



pch:
	$(CC) -c $(PCH) $(CFLAGS)



glad_static: glad_create_static
	rm $(GLAD_BIN)/glad.o

glad_create_static: glad_compile_static
	ar rcs $(GLAD_BIN)/libGLAD.dll.a $(GLAD_BIN)/glad.o

glad_compile_static:
	$(CC) -c $(GLAD_SRC)/glad.c -o $(GLAD_BIN)/glad.o


glad_shared: glad_create_shared
	rm $(GLAD_BIN)/glad.o

glad_create_shared: glad_compile_shared
	$(CC) -shared $(GLAD_BIN)/glad.o -o $(GLAD_BIN)/libGLAD.dll

glad_compile_shared:
	$(CC) -c -fPIC $(GLAD_SRC)/glad.c  -o $(GLAD_BIN)/glad.o