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


debug_lib: lib_dyn
	$(CC) src/main.cpp $(CFLAGS) -o $(BIN)/$(NAME) -Lbin -l:$(LIB_ENGINE_DYN) $(LIBS_DYN)

debug_app:
	$(CC) src/main.cpp $(CFLAGS) -o $(BIN)/$(NAME) -Lbin -l:$(LIB_ENGINE_DYN) $(LIBS_DYN)

debug_lib_run: debug_lib
	./bin/Engine

debug_app_run: debug_app
	./bin/Engine

build: lib_static
	make update_res; \
	$(CC) src/main.cpp $(CFLAGS) -static -L$(LIB_ENGINE) -l:$(LIB_ENGINE_STATIC) $(LIBS_STATIC) -o $(BUILD)/$(NAME);

build_run: build
	./build/Engine

pch:
	$(CC) -c $(PCH) $(CFLAGS)

update_res:
	rm -r -f $(BUILD)/$(RES); \
	cp -r $(RES) $(BUILD)/$(RES); \

.PHONY: build debug debug_run build_run lib_dyn clean