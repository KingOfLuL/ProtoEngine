CC = g++

SRC = lib/glad/src/glad.cpp $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) $(wildcard src/**/**/**/**/*.cpp)
SRC_OBJ = $(SRC:.cpp=.o)
RES = res
BUILD = build
BIN = bin
NAME = App

LIB_ENGINE = lib/ProtoEngine
LIB_ENGINE_DYN = ProtoEngine.dll
LIB_ENGINE_STATIC = ProtoEngine.lib

LIB_DYN_OUT = $(LIB_ENGINE)/$(LIB_ENGINE_DYN)
LIB_STATIC_OUT = $(LIB_ENGINE)/$(LIB_ENGINE_STATIC)

LIBS_STATIC = -lglfw3 -opengl32 -lgdi32 -lassimp -lminizip -lz -static-libgcc -static-libstdc++ -lpthread
LIBS_DYN = -lglfw3 -lassimp

CFLAGS = -std=c++20 -Wall -Wextra -g3 -Wno-narrowing
INCLUDES = -Ilib -Ilib/glad/include -Isrc -Isrc/ProtoEngine
CFLAGS += $(INCLUDES)

PCH = lib/libs.hpp

libs: $(SRC_OBJ)
	$(CC) -shared $^ $(CFLAGS) $(LIBS_DYN) -o $(LIB_DYN_OUT); \
	ar rcs $(LIB_STATIC_OUT) $^; \
	cp $(LIB_DYN_OUT) $(BIN);
	rm -rf $(SRC_OBJ)

%.o: %.cpp
	$(CC) -c -fPIC $^ $(CFLAGS) -o $@

debug:
	$(CC) src/main.cpp $(CFLAGS) -o $(BIN)/$(NAME) -Lbin -l:$(LIB_ENGINE_DYN) $(LIBS_DYN)

debug_run: debug
	./$(BIN)/$(NAME)

build:
	rm -r -f $(BUILD)/$(RES); \
	cp -r $(RES) $(BUILD)/$(RES); \
	$(CC) src/main.cpp $(CFLAGS) -static -L$(LIB_ENGINE) -l:$(LIB_ENGINE_STATIC) $(LIBS_STATIC) -o $(BUILD)/$(NAME);

build_run: build
	./$(BUILD)/$(NAME)

pch:
	$(CC) -c $(PCH) $(CFLAGS)

.PHONY: build debug debug_run build_run lib_dyn clean