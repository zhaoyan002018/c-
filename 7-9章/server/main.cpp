//#include<Windows.h>
#include<fstream>
#include<iostream>
#include<WinSock2.h>
using namespace std;

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

short HOST_PORT = 789;
#define BUFF_SIZE 1024

int main()
{
	initWSAS();

	SOCKET hServSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN hServAddr;
	hServAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	hServAddr.sin_port = htons(HOST_PORT);
	hServAddr.sin_family = AF_INET;


	if (bind(hServSocket, (sockaddr*)& hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error!");
	}

	if (listen(hServSocket, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error!");
	}

	SOCKET hClntSocket;
	SOCKADDR_IN hClntAddr;
	int hClntAddrLen = sizeof(hClntAddr);
	if ((hClntSocket = accept(hServSocket, (sockaddr*)& hClntAddr, &hClntAddrLen)) == SOCKET_ERROR)
	{
		ErrorHandling("accept() error!");
	}

	char buffer[BUFF_SIZE] = { 0 };
	int recv_len = 0, recv_cnt;

	recv_len = recv(hClntSocket, buffer, BUFF_SIZE - 1, 0);
	buffer[recv_len] = '\0';

	ifstream inFile;
	inFile.open(buffer, ios::in | ios::binary);
	if (!inFile)
	{
		cout << "服务器下不存在该文件" << endl;
		closesocket(hClntSocket);
	}
	else {
		memset(buffer, 0, BUFF_SIZE);
		while (!inFile.eof())
		{
			inFile.read(buffer, BUFF_SIZE);
			int file_cnt = inFile.gcount();
			send(hClntSocket, buffer, file_cnt, 0);
			if (inFile.rdstate() != 0)
			{
				break;
			}
		}
		shutdown(hClntSocket, SD_SEND);
		recv_len = recv(hClntSocket, buffer, BUFF_SIZE - 1, 0);
		buffer[recv_len] = '\0';
		cout << "收到返回：" << buffer << endl;
		closesocket(hClntSocket);
	}

	closesocket(hServSocket);
	WSACleanup();
}
