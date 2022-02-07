#include<Windows.h>
#include"tcp_server.h"
#include<fstream>

#include<iostream>
using namespace std;
//完成网络文件传输
int main()
{
	//整个传输过程可以理解为：磁盘->内存->缓冲(内存)->内存->磁盘
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

	char buffer[BUFF_SIZE] = { 0 };
	int recv_len = 0, recv_cnt;
	while (1)
	{
		SOCKET hClntSocket;
		SOCKADDR_IN hClntAddr;
		int hClntAddrLen = sizeof(hClntAddr);
		if ((hClntSocket = accept(hServSocket, (sockaddr*)& hClntAddr, &hClntAddrLen)) == SOCKET_ERROR)
		{
			ErrorHandling("accept() error!");
		}
		cout << "connnected client" << endl;
		recv_len = recv(hClntSocket, buffer, BUFF_SIZE - 1, 0);
		buffer[recv_len] = '\0';
		cout << "name=" << buffer << endl;
		ifstream inFile;
		inFile.open(buffer, ios::in | ios::binary);
		if (!inFile)
		{
			cout << "服务器下不存在该文件" << endl;
			closesocket(hClntSocket);
			break;
		}

		memset(buffer, 0, BUFF_SIZE);
		int i = 0;
		while (1)
		{
			inFile.read(buffer, BUFF_SIZE);//如果没有填满buffer，同样也会返回null，此时可以通过getcount来判断是否有读取。

			send(hClntSocket, buffer, BUFF_SIZE, 0);

			memset(buffer, 0, BUFF_SIZE);

			if (inFile.eof() == 1)
			{
				cout << "文件读取完毕" << endl;

				break;
			}

			if (inFile.rdstate() != 0)
			{
				cout << "流不正常：" << inFile.rdstate() << endl;
				break;
			}
		}

		inFile.close();
		closesocket(hClntSocket);
		cout << "单方向关闭客户端套接字" << endl;
	}

	closesocket(hServSocket);
	WSACleanup();

}
