CC = gcc
LDFLAGS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lstdc++ -g

all: main

main: src/remain.cpp src/quadtree.cpp
	$(CC) -o main src/remain.cpp src/quadtree.cpp $(LDFLAGS)

clean:
	rm -f main
