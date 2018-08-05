#include"sock.h"
#include<sys/types.h>

void SendData(int sockConn, BigInt *pbt)
{
    u_long data_len = pbt->size();
    send(sockConn,(char*)&data_len,sizeof(u_long),0);
    
    char sendbuf[BUFFER_SIZE];
    
    size_t len = 0;
    size_t pos = 0;
    while(data_len > 0)
    {
        len = data_len > BUFFER_SIZE ? BUFFER_SIZE : data_len;
        data_len -= len;
        //分片
        pbt->BigIntCopy(sendbuf,len,pos);
        send(sockConn,sendbuf,len,0);
        pos += len;
    }
}

void RecvData(int sockConn, BigInt &bt)
{
    bt.Clear();
    char recvbuf[BUFFER_SIZE];
    u_long data_len;
    u_long length;
    memset(recvbuf,0,BUFFER_SIZE);
    recv(sockConn,(char*)&data_len,sizeof(u_long),0);
    //cout<<"data_len = "<<data_len<<endl;
    if(data_len < BUFFER_SIZE)
        length = recv(sockConn,recvbuf,data_len,0);
    else
        length = recv(sockConn,recvbuf,BUFFER_SIZE,0);

    for(u_long i=0; i<length; ++i)
    {
        bt.push_back(recvbuf[i]);
    }
    while(length < data_len)
    {
        memset(recvbuf,0,BUFFER_SIZE);
        data_len -= length;
        if(data_len < BUFFER_SIZE)
            length = recv(sockConn,recvbuf,data_len,0);
        else
            length = recv(sockConn,recvbuf,BUFFER_SIZE,0);
        for(u_long i=0; i<length; ++i)
        {
            bt.push_back(recvbuf[i]);
        }
    }
}

void* BigIntServer(void *arg)
{
    int sockConn = *(int*)arg;
    int comm;
    /////////////////////////////////////////////////////////////////////
    BigInt bt;
    BigInt bt1;
    BigInt bt2;
    /////////////////////////////////////////////////////////////////////
    while(1)
    {
        recv(sockConn,(char*)&comm,sizeof(int),0);
        cout<<"command"<<comm<<endl;
        RecvData(sockConn, bt1);
        cout<<"bt1 = ";
        bt1.PrintData();
        RecvData(sockConn, bt2);
        cout<<"bt2 = ";
        bt2.PrintData();
        switch(comm)
        {
        case ADD:
            BigInt::Add(bt,bt1,bt2);
            break;
        case SUB:
            BigInt::Sub(bt,bt1,bt2);
            break;
        case MUL:
            BigInt::Mul(bt,bt1,bt2);
            break;
        case DIV:
            BigInt::Div(bt,bt1,bt2);
            break;
        //case ....:
        //  break;
        default:
            break;
        }

        SendData(sockConn, &bt);
    }
    close(sockConn);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int sockSer;
    sockSer = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSer,addrCli;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(SERVER_PORT);
    addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);
    bind(sockSer,(struct sockaddr*)&addrSer, sizeof(struct sockaddr));
    listen(sockSer,LISTEN_QUEUE);

    int len = sizeof(struct sockaddr);
    int sockConn;


    while(1)
    {
        sockConn = accept(sockSer,(struct sockaddr*)&addrCli, (socklen_t*)(&len));
        if(sockConn < 0)
        {
            cout<<"Server Accept Client Connect Fail!"<<endl;
            continue;
        }
        else
        {
            cout<<"Server Accept Client Connect Success!"<<endl;
        }

        pthread_t tid;
        int res = pthread_create(&tid, NULL, BigIntServer, (void*)&sockConn);
        if(res != 0)
        {
            cout<<"Server Create Client Thread Failed!"<<endl;
        }
        else
        {
            cout<<"Server Create Client Thread Success!"<<endl;
        }
    }
    close(sockSer);
    return 0;
}
