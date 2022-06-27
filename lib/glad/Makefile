CC = g++

BIN = bin
SRC = src/glad.c
OBJ = $(BIN)/glad.o 
OUT_STATIC = $(BIN)/libGLAD.a
OUT_SHARED = $(BIN)/libGLAD.dll

static:
	$(CC) -c $(SRC) -o $(OBJ); \
	ar rcs $(OUT_STATIC) $(OBJ); \
	rm $(OBJ);

shared:
	$(CC) -c -fPIC $(SRC) -o $(OBJ); \
	$(CC) -shared $(OBJ) -o $(OUT_SHARED); \
	rm $(OBJ);