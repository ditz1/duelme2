win:
	g++ -std=c++11 main.cpp -o game -I. -L../client/lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

linux:
	g++ -std=c++11 *.cpp -o game -I. -L../client/lib/ -lrayliblinux -lGL -lm -lpthread -ldl -lrt -lX11

mac:
	clang -std=c++11 *.cpp -o game \
	-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL \
	-I./ -L../client/lib/ -lraylibmac -lstdc++