
#ROOT=$(shell pwd) 
INCLUDE=-I../../lib/include
LIB=-L../../lib/lib
LIB_NAME=-lmysqlclient

.PHONY:all
all:net_cal insert_cgi select_cgi

net_cal:net_cal.c
	gcc -o $@ $^
insert_cgi:insert_cgi.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
select_cgi:select_cgi.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)

.PHONY:clean
clean:
	rm -f net_cal insert_cgi select_cgi
