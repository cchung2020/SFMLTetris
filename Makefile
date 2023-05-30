CC = g++
CFLAGS = -Wall -Werror -pedantic --std=c++20
SFML = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: Game TetrisPiece.o display.o main.o

Game: TetrisPiece.o display.o main.o
	$(CC) $(CFLAGS) -o Game TetrisPiece.o display.o main.o $(SFML)

TetrisPiece.o: TetrisPiece.cpp TetrisPiece.h
	$(CC) $(CFLAGS) -c TetrisPiece.cpp

display.o: display.cpp display.h
	$(CC) $(CFLAGS) -c display.cpp

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	rm *.o Game
