CXXFLAGS = -std=c++17


.PHONY:clean

test: test.cc Log.cc
	g++ $(CXXFLAGS) $^ -o $@ 

clean:
	rm -r test log