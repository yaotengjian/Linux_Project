#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include "udp_client.h"
#include "data.h"
#include "window.h"

typedef struct{
	udp_client *cp;
	window *wp;
}chat_t;

chat_t chat;

struct{
	std::string nick_name;
	std::string school;
}client_info;


void usage(std::string proc)
{
	std::cerr << "Usage: " << proc << " [ip]" << " [port]" << std::endl;
}

void* runTitle(void *arg)
{
	chat_t *p = (chat_t *)arg;
	window *winp = p->wp;

	winp->drawTitle();
	winp->titleMove();
}

void* runOutput(void *arg)
{
	chat_t *p = (chat_t *)arg;
	udp_client *clientp = p->cp;
	window *winp = p->wp;

    winp->drawOutput();
    winp->drawFlist();

	int i = 1;
	int y,x;
	std::string f;
	for(; ;){
    	std::string recvString;
    	clientp->recvData(recvString);
    
    	data d;
    	d.deserialize(recvString);
    
    	std::string showString = d.nickName; //nickName-school# 
    	showString += "-";
    	showString += d.school;

		f = showString;
		if(d.cmd == "QUIT"){
			clientp->delUser(f);
			goto end;
		}else{
			clientp->addUser(f);
		}

    	showString += "# ";
    	showString += d.msg;
    
		getmaxyx(winp->output, y, x);
    	winp->putStringToWin(winp->output, i++, 3, showString);
		if(i >= y-1){
			i=1;
			sleep(1);
			winp->clearLines(winp->output, 1, y-1);
		}
end:
		//show friend
		winp->clearLines(winp->flist, 1, y);
		winp->drawFlist();
		int j = 1;
		std::vector<std::string>::iterator iter = clientp->online.begin();
		for(; iter != clientp->online.end(); iter++){
			winp->putStringToWin(winp->flist, j++, 2, *iter);
			if(j >= y-1){
				//
			}
		}
	}
}

void* runInput(void *arg)
{
	chat_t *p = (chat_t *)arg;
	udp_client *clientp = p->cp;
	window *winp = p->wp;
	std::string s;
	std::string sendString;

	winp->drawInput();
	for( ; ; ){
    	winp->inputGetMsg(s);

    	data d;
    	d.nickName = client_info.nick_name;
    	d.school = client_info.school;
    	d.msg = s;
    	d.cmd = "None";
    	d.serialize(sendString);

    	clientp->sendData(sendString);
	}
}

void sendQuitString(int sig)
{
	data d;
    d.nickName = client_info.nick_name;
    d.school = client_info.school;
    d.msg = "None";
    d.cmd = "QUIT";
    
	std::string sendString;
	d.serialize(sendString);
    (chat.cp)->sendData(sendString);
	delete chat.wp;
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 3){
		usage(argv[0]);
		return 1;
	}
	std::string ip = argv[1];
	int port = atoi(argv[2]);

	std::cout << "Please Enter Your Nick Name# ";
	std::cin >> client_info.nick_name;
	std::cout << "Please Enter Your School# ";
	std::cin >> client_info.school;


	udp_client client(ip, port);
	client.initClient();
	window *win = new window();

	chat = {&client, win};
	signal(SIGINT, sendQuitString);

	pthread_t title, output, input;
	pthread_create(&title, NULL, runTitle, (void *)&chat);
	pthread_create(&output, NULL, runOutput, (void *)&chat);
	pthread_create(&input, NULL, runInput, (void *)&chat);

	pthread_join(title, NULL);
	pthread_join(output, NULL);
	pthread_join(input, NULL);

//	data d;
//	for( ; ; ){
//		std::string sendString;
//		std::cout << "Please Enter nickName:";
//		std::cin >> d.nickName;
//
//		std::cout << "Please Enter school:";
//		std::cin >> d.school;
//		std::cout << "Please Enter msg:";
//		std::cin >> d.msg;
//		d.cmd = "None";
//		
//		d.serialize(sendString);
//
//		client.sendData(sendString);
//
//		std::string recvString;
//		client.recvData(recvString);
//		std::cout << "server# " << recvString << std::endl;
//	}
//
}


















