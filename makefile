CC = g++ -Wall -std=c++17
CODE = ./*.cpp
IMGUI = ./imgui/*.cpp
INC = -I"C:\msys64\mingw64\include" -I"C:\msys64\usr\local\include"
LIB = -L"C:\msys64\mingw64\lib" -I"C:\msys64\usr\local\lib"
FLAG = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lglew32 -lglu32 -lopengl32
PROG = opengl.exe

build:
	$(CC) $(CODE) $(IMGUI) $(INC) $(LIB) $(FLAG) -o $(PROG)

debug:
	$(CC) -g3 $(CODE) $(IMGUI) $(INC) $(LIB) $(FLAG) -o opengl_debug.exe

clean:
	rm ./*.exe
