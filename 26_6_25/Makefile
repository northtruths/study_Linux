.PHONY: clean run

httpserver: HttpServerMain.cpp Log.cc
	g++ $^ -o $@ -std=c++17 -lpthread

clean: 
	rm httpserver