#include "udp_server.h"
#include "data.h"

udp_server::udp_server(const int &_port):sock(-1), port(_port), pool(256)
{
}
int udp_server::initServer()
{
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock < 0){
		//print_log
		std::cerr << "socket create error" << std::endl;
		return -1;
	}

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		std::cerr << "bind error" << std::endl;
		return -2;
	}

	return 0;
}
void udp_server::sendData(const std::string &outString, const struct sockaddr_in &client)
{
	sendto(sock, outString.c_str(), outString.size(), 0,\
			(struct sockaddr*)&client, sizeof(client));
}

void udp_server::broadcast()
{
	std::string sendString;
	pool.getData(sendString);
	std::map<int,struct sockaddr_in>::iterator iter = online.begin();
	for( ; iter != online.end(); iter++ ){
		sendData(sendString, iter->second);
	}
}

void udp_server::addUser(struct sockaddr_in &c)
{
	online.insert(std::pair<int, struct sockaddr_in>(c.sin_addr.s_addr, c));
}

void udp_server::delUser(struct sockaddr_in &c)
{
	online.erase(c.sin_addr.s_addr);
}

void udp_server::recvData(std::string &inString)
{
	data d;
	char buf[MAX];
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, \
			(struct sockaddr*)&client, &len);
	std::cout << "AAAAAAAAA" << std::endl;
	if(s > 0){
		std::cout << "aaaaaaaaa" << std::endl;
		buf[s] = 0;
		inString = buf;
		d.deserialize(inString);
		std::cout <<"cmd:"<< d.cmd <<std::endl;
		if(d.cmd == "QUIT"){
			std::cout << "quit" <<std::endl;
			delUser(client);
		}else{
			addUser(client);
		}
		pool.putData(inString);
	}
}

udp_server::~udp_server()
{
	if(sock >= 0){
		close(sock);
		sock = -1;
	}
}















