#include<Windows.h>
#include"tcp_server.h"
#include<iostream>
using namespace std;
short HOST_PORT = 789;
int main1()
{
	return 1;

	initWSAS();

	SOCKET hServSocket;
	SOCKADDR_IN servAddr;

	//创建socket
	hServSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//绑定socket地址
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(HOST_PORT);

	if (bind(hServSocket, (SOCKADDR*)& servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error!");
	}


	//监听请求
	if (listen(hServSocket, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error!");
	}

	cout << "开始监听请求：" << endl;
	SOCKET hClntSocket;
	SOCKADDR_IN clntAddr;
	//int szClntAddr = 0;
	int szClntAddr = sizeof(clntAddr);

	//阻塞接收请求
	char buffer[BUFF_SIZE];

	for (int i = 0; i < 4; i++)
	{
		hClntSocket = accept(hServSocket, (SOCKADDR*)& clntAddr, &szClntAddr);//szClntAddr的作用应该是要指定接收的客户端套接字的长度。虽然觉得既然制定了协议应该规定了长度，不需要再额外指出，但应该有别的用意。
		if (hClntSocket == -1)
		{
			ErrorHandling("accept() error!");
		}
		else
		{
			cout << "connected client " << i << endl;
		}
		int messageLen = 0;
		/*
		回声服务器部分
		while ((messageLen = recv(hClntSocket, buffer, BUFF_SIZE, 0)) != 0)
		{
			send(hClntSocket, buffer, messageLen, 0);
		}
		*/
		//处理客户端传来的4个字节长度的整形、1个字节长度的运算符
		int sub[3] = { 0 };
		char op;
		long result;

		//对于tcp流传输来说，不需要管的是write怎么写，而是read怎么读，需要有一个读取的截止条件。
		//固定参数长度为：3个int型数据+1个char型数据=13个字节大小
		int message_cnt = 0;
		while (messageLen < PROTOCOL_LEN)
		{
			message_cnt = recv(hClntSocket, buffer + messageLen, BUFF_SIZE, 0);//BUFF_SIZE肯定大于int型对应的字符流，尽管长度不确定
			//这里有个问题，如果多次recv，那么向buffer写入的时候，会不会自动向没有数据的位置偏移呢？还是会覆盖？答案是会覆盖，需要手动偏移。
			if (message_cnt == -1)
			{
				ErrorHandling("recv() error!");
			}
			messageLen += message_cnt;
		}
		for (int j = 0, k = 0; j < 3; k = k + sizeof(int))
		{
			sub[j++] = (int)buffer[k];
		}
		op = buffer[PROTOCOL_LEN - 1];
		result = col(sub, 3, op);
		send(hClntSocket, (const char*)& result, sizeof(result), 0);

		closesocket(hClntSocket);
	}

	closesocket(hServSocket);
	WSACleanup();
	return 0;

}


long col(int sub[], int len, char op)
{
	int i = 0;
	long result = 0;
	switch (op)
	{
	case '+':
		while (i < len)
			result += sub[i++];
		break;
	case '-':
		result = sub[i++];
		while (i < len)
			result -= sub[i++];
		break;
	case '*':
		result = sub[i++];
		while (i < len)
			result *= sub[i++];
		break;
	default:
		cout << "未识别的操作符:" << op << endl;
		break;
	}
	return result;
}
void ErrorHandling(const char* message)
{
	cerr << message << endl;
	exit(-1);
}
void initWSAS()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}
}
