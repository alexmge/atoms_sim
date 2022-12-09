CC = gcc
LDFLAGS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lstdc++

all: main

main: remain.cpp
	$(CC) -o main remain.cpp $(LDFLAGS)

clean:
	rm -f main
