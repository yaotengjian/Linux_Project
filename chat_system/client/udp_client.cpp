#include "udp_client.h"

udp_client::udp_client(const std::string &ip,const int &_port):sock(-1)
{
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip.c_str());
	server.sin_port = htons(_port);
}

void udp_client::addUser(const std::string &f)
{
	std::vector<std::string>::iterator iter = online.begin();
	
	for(; iter != online.end(); iter++){
		if( *iter == f ){
			return;
		}
	}
	online.push_back(f);
}

void udp_client::delUser(const std::string &f)
{
	std::vector<std::string>::iterator iter = online.begin();
	for(; iter != online.end(); ){
		if(*iter == f){
			iter = online.erase(iter);
			break;
		}else{
			iter++;
		}
	}
}

int udp_client::initClient()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		//print_log
		std::cerr << "socket create error" << std::endl;
		return -1;
	}
	return 0;
}
void udp_client::sendData(const std::string &outString)
{
	sendto(sock, outString.c_str(), outString.size(), 0,\
			(struct sockaddr*)&server, sizeof(server));
}

void udp_client::recvData(std::string &inString)
{
	char buf[1024];
	ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, NULL, NULL);
	if(s > 0){
		buf[s] = 0;
		inString = buf;
	}
}

udp_client::~udp_client()
{
	if(sock >= 0){
		close(sock);
		sock = -1;
	}
}















