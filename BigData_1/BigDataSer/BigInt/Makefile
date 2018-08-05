BigMain:BigMain.o BigInt.o
	g++ -o BigMain BigMain.o BigInt.o

BigMain.o:BigMain.cpp BigInt.h
	g++ -o BigMain.o -c BigMain.cpp -g
BigInt.o:BigInt.h BigInt.cpp SeqList.h
	g++ -o BigInt.o -c BigInt.cpp -g
clean:
	rm *.o BigMain

