.PHONY:clean
CXXFLAGS = -std=c++17
SRCS = Log.cc Pool_test.cc
OBJS = $(SRCS:.cc=.o)


ptest: $(OBJS) 
	g++ $^ -o $@ -lpthread

%.o: %.cc
	g++ -c $(CXXFLAGS) $< -o $@ -lpthread

test: tread_test.cc Log.cc
	g++ $(CXXFLAGS) $^ -o $@ 
	
clean:
	rm -r test ptest $(OBJS) log