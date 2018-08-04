#include <pthread.h>
#include "udp_server.h"

static void usage(const std::string proc)
{
	std::cout << "Usage: " << proc << " [port]" << std::endl;
}

void *udp_recv(void *arg)
{
	udp_server *sp = (udp_server*)arg;
	std::string str;
	for (;;){
		sp->recvData(str);
		std::cout << str << std::endl;
	}
}

void *udp_broadcast(void *arg)
{
	udp_server *sp = (udp_server*)arg;
	for(;;){
		sp->broadcast();
	}
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		usage(argv[0]);
		return 1;
	}
	
	udp_server server(atoi(argv[1]));
	if(server.initServer() < 0){
		return 2;
	}

	pthread_t p, c;
	pthread_create(&p, NULL, udp_recv, (void *)&server);
	pthread_create(&c, NULL, udp_broadcast, (void *)&server);

	pthread_join(p, NULL);
	pthread_join(c, NULL);

//	std::string str;
//	for( ; ; ){
//		server.recvData(str);
//		std::cout << "client# " << str << std::endl;
//	//	server.sendData(str);
//	}
}










