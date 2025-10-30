# Detect platform
UNAME_S := $(shell uname -s)

# Common settings
CC = g++ -Wall -std=c++17
CODE = ./*.cpp
IMGUI = ./imgui/*.cpp

# Platform-specific settings
ifeq ($(UNAME_S),Darwin)    # macOS
    INC = -I/opt/homebrew/include -I/usr/local/include
    LIB = -L/opt/homebrew/lib -L/usr/local/lib
    FLAG = -lSDL2 -lSDL2_image -framework OpenGL -lGLEW
    PROG = opengl
    CLEAN_CMD = rm -f ./opengl ./opengl_debug
else ifeq ($(UNAME_S),Linux)    # Linux
    INC =
    LIB =
    FLAG = -lSDL2 -lSDL2_image -lGL -lGLEW
    PROG = opengl
    CLEAN_CMD = rm -f ./opengl ./opengl_debug
else    # Windows (MinGW)
    INC = -I"C:\msys64\mingw64\include" -I"C:\msys64\usr\local\include"
    LIB = -L"C:\msys64\mingw64\lib" -I"C:\msys64\usr\local\lib"
    FLAG = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lglew32 -lopengl32
    PROG = opengl.exe
    CLEAN_CMD = rm ./*.exe
endif

build:
	$(CC) $(CODE) $(IMGUI) $(INC) $(LIB) $(FLAG) -o $(PROG)

debug:
	$(CC) -g3 $(CODE) $(IMGUI) $(INC) $(LIB) $(FLAG) -o $(PROG)_debug

clean:
	$(CLEAN_CMD)

.PHONY: build debug clean
