OBJECT=./BigInt/BigInt.o

BigIntSer:$(OBJECT) sock.o
	g++ -o BigIntSer sock.o $(OBJECT) -lpthread
sock.o:sock.h sock.cpp
	g++ -o sock.o -c sock.cpp
clean:
	rm BigIntSer *.o
