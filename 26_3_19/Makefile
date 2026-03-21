BIN = propool
CC = g++
FLAGS = -c -Wall -std=c++11
LINK = -o
SRC=$(wildcard *.cpp)
OBJ=$(SRC:.cpp=.o)

$(BIN):$(OBJ)
	$(CC) $(LINK) $@ $^
%.o:%.cpp
	$(CC) $(FLAGS) $<

.PHONY:clean
clean:
	rm -f $(BIN) $(OBJ)

.PHONY:test
	echo $(SRC)
	echo $(OBJ)