#include"BigInt.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5050
#define BUFFER_SIZE 32

typedef enum {ADD,SUB,MUL,DIV,QUIT}OPER_ENUM;

typedef struct BigIntOper
{
    OPER_ENUM command;
    BigInt *bt1;
    BigInt *bt2;
}BigIntOper;

void SendData(int sock, BigIntOper *pBto)
{
    char sendbuf[BUFFER_SIZE];
    int comm = pBto->command;
    u_long data_len = pBto->bt1->size();
    size_t len = 0;
    size_t pos = 0;
    //send command
    send(sock,(char*)&comm,sizeof(int),0);
    //send data length
    send(sock,(char*)&data_len,sizeof(u_long),0);
    //send bt1
    while(data_len > 0)
    {
        len = data_len > BUFFER_SIZE ? BUFFER_SIZE : data_len;
        data_len -= len;
        //分片
        pBto->bt1->BigIntCopy(sendbuf,len,pos);
        send(sock,sendbuf,len,0);
        pos += len;
    }
    //cout<<"send bt1 over!"<<endl;
    //send bt2

    pos = 0;
    data_len = pBto->bt2->size();
    send(sock,(char*)&data_len,sizeof(u_long),0);
    while(data_len > 0)
    {
        len = data_len > BUFFER_SIZE ? BUFFER_SIZE : data_len;
        data_len -= len;
        pBto->bt2->BigIntCopy(sendbuf,len,pos);
        send(sock,sendbuf,len,0);
        pos += len;
    }
    //cout<<"send bt2 over!"<<endl;

}

void RecvData(int sockCli, BigInt &bt)
{
    char recvbuf[BUFFER_SIZE];
    u_long data_len;
    u_long length;
    memset(recvbuf,0,BUFFER_SIZE);
    recv(sockCli,(char*)&data_len,sizeof(u_long),0);
    if(data_len < BUFFER_SIZE)
        length = recv(sockCli,recvbuf,data_len,0);
    else
        length = recv(sockCli,recvbuf,BUFFER_SIZE,0);
    
    for(u_long i=0; i<length; ++i)
    {
        bt.push_back(recvbuf[i]);
    }
    while(length < data_len)
    {
        memset(recvbuf,0,BUFFER_SIZE);
        data_len -= length;
        if(data_len < BUFFER_SIZE)
            length = recv(sockCli,recvbuf,data_len,0);
        else
            length = recv(sockCli,recvbuf,BUFFER_SIZE,0);
        for(u_long i=0; i<length; ++i)
        {
            bt.push_back(recvbuf[i]);
        }
    }
}

int main(int argc, char *argv[])
{
    int sockCli;
    sockCli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addrSer;
    addrSer.sin_family = AF_INET;
    addrSer.sin_port = htons(SERVER_PORT);
    addrSer.sin_addr.s_addr = inet_addr(SERVER_IP);

    int res = connect(sockCli,(struct sockaddr*)&addrSer,sizeof(struct sockaddr));
    if(res != 0)
    {
        cout<<"Client Connect Server Failed!"<<endl;
        return -1;
    }
    else
    {
        cout<<"Client Connect Server Success!"<<endl;
    }

    int select = 1;
    BigInt bt1, bt2;
    u_long n;

    BigIntOper Bto;
    BigInt bt;
    while(select)
    {
        cout<<"**********************************************"<<endl;
        cout<<"* [1]随机加载两个大数    [2]定制两个大数     *"<<endl;
        cout<<"* [3]显示两个大数        [0]退出系统         *"<<endl;
        cout<<"* [4]bt = bt1+bt2        [5]bt = bt1-bt2     *"<<endl;
        cout<<"* [6]bt = bt1*bt2        [7]bt = bt1/bt2     *"<<endl;
        cout<<"* [8]bt = bt1%bt2                            *"<<endl;
        cout<<"**********************************************"<<endl;
        cout<<"请选择:>";
        cin>>select;
        switch(select)
        {
        case 1:
            cout<<"请输入要加载第一个大数的位数:>";
            cin>>n;
            bt1.LoadData(n);
            cout<<"请输入要加载第二个大数的位数:>";
            cin>>n;
            bt2.LoadData(n);
            continue;
            //break;
        case 2:
            break;
        case 3:
            cout<<"bt1 = ";
            bt1.PrintData();
            cout<<"bt2 = ";
            bt2.PrintData();
            continue;
            //break;
        case 4:
            Bto.command = ADD;
            Bto.bt1 = &bt1;
            Bto.bt2 = &bt2;
            SendData(sockCli, &Bto);
            RecvData(sockCli, bt);
            break;
        case 5:
            Bto.command = SUB;
            Bto.bt1 = &bt1;
            Bto.bt2 = &bt2;
            SendData(sockCli, &Bto);
            RecvData(sockCli, bt);
            break;
        case 6:
            Bto.command = MUL;
            Bto.bt1 = &bt1;
            Bto.bt2 = &bt2;
            SendData(sockCli, &Bto);
            RecvData(sockCli, bt);
            break;
        case 7:
            Bto.command = DIV;
            Bto.bt1 = &bt1;
            Bto.bt2 = &bt2;
            SendData(sockCli, &Bto);
            RecvData(sockCli, bt);
            break;
        default:
            break;
        }
        cout<<"result = ";
        bt.PrintData();
        bt.Clear();
    }

    close(sockCli);
    return 0;
}
