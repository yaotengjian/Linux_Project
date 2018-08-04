CURR_PATH=$(shell pwd)
#SRC=$(shell ls *.cpp | sed 's/.cpp/.o/g')
SRC=$(shell ls *.cpp)
BIN=chat_server
CC=g++
LDFLAGS=#

$(BIN):$(SRC)
	$(CC) -o $@ $^
.PHONY:clean
clean:
	rm -f $(BIN)
