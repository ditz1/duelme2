win:
	g++ -std=c++11 main.cpp -o game -I. -L../client/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

mac:
	clang -std=c++11 main.cpp -o game \
	-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL \
	-I. -L../client/lib/ -lraylib