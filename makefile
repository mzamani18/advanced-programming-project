
main: client.cpp server.cpp main.cpp
	g++ -o main client.cpp main.cpp -I.
	g++ -std=c++14 -o server server.cpp -lpthread -I.

