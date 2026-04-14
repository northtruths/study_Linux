.PHONY: clean

signal: Signal.cc
	g++ $^ -o $@

sa: Sigaction.cc
	g++ $^ -o $@

sm: Sigprocmask.cc
	g++ $^ -o $@

clean:
	rm signal sa sm