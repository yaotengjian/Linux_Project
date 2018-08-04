#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "data_pool.h"

#define MAX 1024

class udp_server{
	public:
		udp_server(const int &_port);
		void addUser(struct sockaddr_in &c);
		void delUser(struct sockaddr_in &c);
		int initServer();
		void broadcast();
		void sendData(const std::string &outString, \
				const struct sockaddr_in &client);
		void recvData(std::string &inString);
		~udp_server();
	private:
		int sock;
		int port;
		std::map<int, struct sockaddr_in> online;
		data_pool pool;
};

#endif












