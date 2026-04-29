.PHONY:clean
CXXFLAGS = -std=c++17
SRCS = Log.cc SigTP_test.cc
OBJS = $(SRCS:.cc=.o)

sigtest: $(OBJS) 
	g++ $^ -o $@ -lpthread

%.o: %.cc
	g++ -c $(CXXFLAGS) $< -o $@ -lpthread

	
clean:
	rm -r sigtest $(OBJS) log