CC = g++

SRC = $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp) $(wildcard src/**/**/**/**/*.cpp)
SRC_OBJ = $(SRC:.cpp=.o)
RES = res
BUILD = build
BIN = bin
NAME = Engine

LIB_ENGINE = lib/ProtoEngine
LIB_ENGINE_DYN = libProtoEngine.dll
LIB_ENGINE_STATIC = libProtoEngine.a

LIB_DYN_OUT = $(LIB_ENGINE)/$(LIB_ENGINE_DYN)
LIB_STATIC_OUT = $(LIB_ENGINE)/$(LIB_ENGINE_STATIC)

LIBS_STATIC = -Llib/glad/bin -lglfw3 -opengl32 -lgdi32 -lassimp -lminizip -lz -static-libgcc -static-libstdc++ -lpthread -l:libGLAD.a
LIBS_DYN = -lglfw3 -lassimp -Llib/glad/bin -l:libGLAD.dll

CFLAGS = -std=c++20 -Wall -Wextra -g3 -Wno-narrowing
INCLUDES = -Ilib -Ilib/glad/include -Isrc -Isrc/ProtoEngine
CFLAGS += $(INCLUDES)

PCH = lib/libs.hpp

lib_dyn: $(SRC_OBJ)
	$(CC) -shared $^ $(CFLAGS) $(LIBS_DYN) -o $(LIB_DYN_OUT); \
	make clean; \
	cp $(LIB_DYN_OUT) $(BIN);

lib_static: $(SRC_OBJ)
	ar rcs $(LIB_STATIC_OUT) $^; \
	make clean;

%.o: %.cpp
	$(CC) -c -fPIC $^ $(CFLAGS) -o $@

clean:
	rm -rf $(SRC_OBJ)


debug: lib_dyn
	$(CC) src/main.cpp $(CFLAGS) -o $(BIN)/$(NAME) -l:$(LIB_ENGINE_DYN) $(LIBS_DYN)

debug_run: debug
	./bin/Engine


build: lib_static
	rm -r -f $(BUILD)/$(RES); \
	cp -r $(RES) $(BUILD)/$(RES); \
	$(CC) src/main.cpp $(CFLAGS) -static -L$(LIB_ENGINE) -l:$(LIB_ENGINE_STATIC) $(LIBS_STATIC) -o $(BUILD)/$(NAME);

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

.PHONY: build debug debug_run build_run lib_dyn clean