.PHONY: all remove

all:tcp_server tcp_client

tcp_server: TcpServerMain.cpp Log.cc
	g++ $^ -o $@ -std=c++17 -lpthread -ljsoncpp

tcp_client: TcpClientMain.cpp Log.cc -ljsoncpp
	g++ $^ -o $@ -std=c++17

remove:
	rm -rf tcp_server tcp_client