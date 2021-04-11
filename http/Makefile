

.PHONY:all
all:httpd cgi

httpd:httpd.c
	gcc -o $@ $^ -lpthread
cgi:
	cd wwwroot/cgi; make clean; make ; cd -;

.PHONY:output
output:all
	mkdir -p output/wwwroot/cgi
	cp httpd output
	cp start.sh output
	cp -rf lib output
	cp wwwroot/*.html output/wwwroot
	cp -rf wwwroot/imag output/wwwroot
	cp -f wwwroot/cgi/net_cal output/wwwroot/cgi
	cp -f wwwroot/cgi/insert_cgi output/wwwroot/cgi
	cp -f wwwroot/cgi/select_cgi output/wwwroot/cgi

.PHONY:clean
clean:
	rm -rf httpd output
	cd wwwroot/cgi; make clean; cd -;

