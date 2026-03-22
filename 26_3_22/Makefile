RECV=recv
SEND=send
CC=g++
RECV_SRC=Receiver.cc
SEND_SRC=Sender.cc

.PHONY:all clean

all:$(RECV) $(SEND)

$(RECV):$(RECV_SRC)
	$(CC) -o $@ $^

$(SEND):$(SEND_SRC)
	$(CC) -o $@ $^

clean:
	rm $(RECV) $(SEND)