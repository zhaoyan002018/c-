#include<Windows.h>
#include"tcp_client.h"
#include<fstream>
int main1()
{
	return 1;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	SOCKET hServSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN hServAddr;
	hServAddr.sin_addr.S_un.S_addr = inet_addr(HOST_IP);
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_port = htons(HOST_PORT);

	if (connect(hServSocket, (sockaddr*)& hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}

	string filename;
	char buffer[BUFF_SIZE];
	cout << "请输入想要获得的文件路径:" << endl;
	cin >> filename;
	send(hServSocket, filename.c_str(), filename.length(), 0);

	ofstream outFile;
	outFile.open(filename.c_str(), ios::out | ios::binary | ios::ate);
	int recv_ctn = 0;
	//如何判断文件传输结束？-1是出错。需要实现给出文件长度，但同样也可以通过断开连接来结束，但这就无法确定是传输完成了，还是失败了。
	while ((recv_ctn = recv(hServSocket, buffer, BUFF_SIZE, 0)) != 0)
	{

		outFile.write(buffer, recv_ctn);
		memset(buffer, 0, 1024);

	}
	outFile.close();
	closesocket(hServSocket);
	cout << "传输结束" << endl;
	outFile.close();


}
