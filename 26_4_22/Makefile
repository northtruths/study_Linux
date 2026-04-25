.PHONY:clean

test_tp: ThreadPool.cc
	g++ $^ -o $@ -lpthread

test: tread_test.cc
	g++ $^ -o $@ -lpthread

clean:
	rm test test_tp