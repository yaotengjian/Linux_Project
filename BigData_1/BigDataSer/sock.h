#ifndef _SOCK_H
#define _SOCK_H

#include"./BigInt/BigInt.h"
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>


#define SERVER_IP    "127.0.0.1"
#define SERVER_PORT  5050
#define LISTEN_QUEUE 5

#define BUFFER_SIZE 32

typedef enum {ADD,SUB,MUL,DIV,QUIT}OPER_ENUM;
typedef struct BigIntOper
{
    OPER_ENUM command;
    BigInt *bt1;
    BigInt *bt2;
}BigIntOper;

void SendData(int sockConn, BigInt *pbt);
void RecvData(int sockConn, BigInt &bt);
void* BigIntServer(void *arg);

#endif
