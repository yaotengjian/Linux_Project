BigMain:BigMain.o BigInt.o
	g++ -o BigMain BigMain.o BigInt.o -m32

BigMain.o:BigMain.cpp BigInt.h
	g++ -o BigMain.o -c BigMain.cpp -g -m32
BigInt.o:BigInt.h BigInt.cpp SeqList.h
	g++ -o BigInt.o -c BigInt.cpp -g -m32
clean:
	rm *.o BigMain

