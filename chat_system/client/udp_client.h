#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

class udp_client{
	public:
		udp_client(const std::string &ip,const int &_port);
		int initClient();
		void recvData(std::string &inString);
		void sendData(const std::string &outString);
		void addUser(const std::string &f);
		void delUser(const std::string &f);
		~udp_client();
		std::vector<std::string> online;
	private:
		int sock;
		struct sockaddr_in server;
};

#endif

