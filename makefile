
all: client.cpp server.cpp Board.h
	g++ Board.h -o Board
	g++ -std=c++14 -o server server.cpp -lpthread -I.
	g++ -std=c++14 -o client client.cpp -lpthread -I.

#g++ -o main main.cpp -I.