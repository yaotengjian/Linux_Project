#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <pthread.h>

#define MAX 1024
#define HOME_PAGE "index.html"

void usage(const char *proc)
{
	printf("Usage:\n\t%s port\n", proc);
}

int startup(int port)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket");
		exit(2);
	}

	int opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in local;
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(port);

	if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0){
		perror("bind");
		exit(3);
	}

	if(listen(sock, 5) < 0){
		perror("listen");
		exit(4);
	}

	return sock;
}

int get_line(int sock, char line[], int num)
{
	char c = 'A';
	int i = 0;
	while( i < num-1 && c != '\n'){
		ssize_t s = recv(sock, &c, 1, 0);
		if(s > 0){ // \r,\r\n, \n
			if(c == '\r'){
				recv(sock, &c, 1, MSG_PEEK);
				if(c == '\n'){
					recv(sock, &c, 1, 0);
				}
				else{
					c = '\n';
				}
			}
			// \r, \r\n -> \n
			line[i++] = c;
		}
		else{
			break;
		}
	}
	line[i] = '\0';
	return i;
}
	
void clear_header(int sock)
{
	char line[MAX];
	do{
		get_line(sock, line, MAX);
	}while(strcmp(line, "\n"));
}

int echo_www(int sock, char *path, int size)
{
	clear_header(sock);

	int fd = open(path, O_RDONLY);
	if(fd < 0){
		return 404;
	}

	
	char line[MAX];
	sprintf(line,"HTTP/1.0 200 OK\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "Content-Type:text/html;charset=ISO-8859-1\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line ,"\r\n");
	send(sock, line, strlen(line), 0);

	sendfile(sock, fd, NULL, size);
	return 200;
}

void show_404(int sock)
{
	char line[MAX];
	sprintf(line, "HTTP/1.0 404 Not Found\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "Content-Type:text/html;charset=ISO-8859-1\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line,"\r\n");
	send(sock, line, strlen(line), 0);

	char *page_404="wwwroot/404.html";
	struct stat st;
	stat(page_404, &st);
	int fd = open(page_404, O_RDONLY);
	sendfile(sock, fd, NULL, st.st_size);
	close(fd);
}

void echo_error(int sock, int code)
{
	clear_header(sock);
	switch(code){
		case 403:
			break;
		case 404:
			show_404(sock);
			break;
		case 500:
			break;
		case 501:
			break;
		case 503:
			break;
		default:
			break;
	}
}

int exe_cgi(int sock, char path[], char method[], char *query_string)
{
	char line[MAX];
	char method_env[MAX];
	char query_string_env[MAX];
	char content_length_env[MAX/8];

	int content_length = -1;
	if(strcasecmp(method, "GET") == 0){
		clear_header(sock);
	}else{//POST
	    do{
	    	get_line(sock, line, MAX);
			printf("---: %s", line);
			if(strncmp(line, "Content-Length: ", 16) == 0){
				content_length = atoi(line+16);
			}
	    }while(strcmp(line, "\n"));
		if(content_length == -1){
			return 404;
		}
	}

	int input[2];
	int output[2];

	pipe(input);
	pipe(output);

	//GET[path, query_string], POST[path, content_length]
	pid_t id = fork();
	if(id < 0){
		return 404;
	}
	else if(id == 0){
		//child
		//method, GET[query_string], POST[content_length]

		close(input[1]);
		close(output[0]);

		dup2(input[0], 0);
		dup2(output[1], 1);

		sprintf(method_env,"METHOD=%s", method);
		putenv(method_env);
		if(strcasecmp(method, "GET") == 0){
			sprintf(query_string_env, "QUERY_STRING=%s", query_string);
			putenv(query_string_env);
		}else{ //post
			sprintf(content_length_env, "CONTENT_LENGTH=%d",\
					content_length);
			putenv(content_length_env);
		}

		execl(path, path, NULL);
		exit(1);
	}
	else{
		close(input[0]);
		close(output[1]);

		char c;
		if(strcasecmp(method, "POST") == 0){
			int i = 0;
			for(; i < content_length; i++){
				recv(sock, &c, 1, 0);
				write(input[1], &c, 1);
			}
		}
	    sprintf(line,"HTTP/1.0 200 OK\r\n");
	    send(sock, line, strlen(line), 0);
	    sprintf(line, "Content-Type:text/html;charset=ISO-8859-1\r\n");
	    send(sock, line, strlen(line), 0);
	    sprintf(line ,"\r\n");
	    send(sock, line, strlen(line), 0);

		while(read(output[0], &c, 1) > 0){
			send(sock, &c, 1, 0);
		}

		waitpid(id, NULL, 0);

		close(input[1]);
		close(output[0]);
	}
	return 200;
}

void *handler_request(void *arg)
{
	int sock = (int)arg;
	char line[MAX];
	char method[MAX/32];
	char url[MAX];
	char path[MAX];
	int errCode = 200;
	int cgi = 0;
	char *query_string = NULL;

#ifdef Debug
	do{
		get_line(sock, line, MAX);
		printf("%s", line);
	}while(strcmp(line, "\n"));
#else
	get_line(sock, line, sizeof(line));

	///GET / HTTP/1.1
	int i = 0;
	int j = 0;
	while(i < sizeof(method)-1 && j < sizeof(line) && !isspace(line[j])){
		method[i] = line[j];
		i++, j++;
	}
	method[i] = '\0';

	if(strcasecmp(method, "GET") == 0){
	}
	else if(strcasecmp(method, "POST") == 0){
		cgi = 1;
	}else{
		errCode = 404;
		goto end;
	}

	while(j < sizeof(line) && isspace(line[j])){
		j++;
	}

	i=0;
	while(i < sizeof(url)-1 && j < sizeof(line) && !isspace(line[j])){
		url[i] = line[j];
		i++, j++;
	}
	url[i] = '\0';

	// method, cgi, url[/a/b/c.php?a=100&b=200]
	if(strcasecmp(method, "GET") == 0){
		query_string = url;
		while(*query_string){
			if(*query_string == '?'){
				*query_string = '\0';
				query_string++;
				cgi = 1;
				break;
			}
			query_string++;
		}
	}
	// url -> / 
	sprintf(path, "wwwroot%s", url);
	if(path[strlen(path)-1] == '/'){
		strcat(path, HOME_PAGE);
	}

	printf("method: %s, path: %s\n", method, path);
	struct stat st;
	if(stat(path, &st) < 0){
		errCode = 404;
		goto end;
	}else{//exist
		if(S_ISDIR(st.st_mode)){
			strcat(path, HOME_PAGE);
		}else if((st.st_mode & S_IXUSR) || \
				(st.st_mode & S_IXGRP) || \
				(st.st_mode) & S_IXOTH){
			cgi = 1;
		}else{
		}

		//method, cgi, path, query_string
		if(cgi){
			errCode = exe_cgi(sock, path, method, query_string);
		}else{
			//path
			errCode = echo_www(sock, path, st.st_size);
		}
	}
	
#endif
end:
	if(errCode != 200){
		echo_error(sock, errCode);
	}
	close(sock);
}

// ./httpd 8080
int main(int argc, char *argv[])
{
	if(argc != 2){
		usage(argv[0]);
		return 1;
	}
	int listen_sock = startup(atoi(argv[1]));

	signal(SIGPIPE, SIG_IGN);
	//daemon(0, 0);
	for(;;){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int new_sock = accept(listen_sock,\
				(struct sockaddr*)&client, &len);
		if(new_sock < 0){
			perror("accept");
			continue;
		}

		pthread_t id;
		pthread_create(&id, NULL, handler_request, (void *)new_sock);
		pthread_detach(id);
	}
}

