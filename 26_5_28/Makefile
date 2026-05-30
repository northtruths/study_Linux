.PHONY: all remove

all:server client

server: UdpServerMain.cpp Log.cc
	g++ $^ -o $@ -std=c++17

client: UdpClientMain.cpp Log.cc
	g++ $^ -o $@ -std=c++17


remove:
	rm -f server client
