//#include<Windows.h>
#include<WinSock2.h>
#include<iostream>
#include<fstream>
#include <WS2tcpip.h>
using namespace std;
const short HOST_PORT = 789;
const char* HOST_IP = "127.0.0.1";
#define BUFF_SIZE 1024
void ErrorHandling(const char* message)
{
	cerr << message << endl;
	exit(-1);
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	SOCKET hServSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN hServAddr;
	//hServAddr.sin_addr.S_un.S_addr = inet_addr(HOST_IP);
	inet_pton(AF_INET, HOST_IP, (void*)& hServAddr.sin_addr.S_un.S_addr);
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_port = htons(HOST_PORT);

	if (connect(hServSocket, (sockaddr*)& hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error!");
	}

	string filename;
	cout << "请输入想要获得的文件路径:" << endl;
	cin >> filename;
	send(hServSocket, filename.c_str(), filename.length(), 0);

	ofstream outFile;
	outFile.open(filename.c_str(), ios::out | ios::binary | ios::ate);
	int recv_ctn = 0;

	char buffer[BUFF_SIZE] = { 0 };
	while ((recv_ctn = recv(hServSocket, buffer, BUFF_SIZE, 0)) != 0)
	{
		cout << "recv_ctn=" << recv_ctn << endl;
		outFile.write(buffer, recv_ctn);
	}

	string sayBye = "thank you~";
	send(hServSocket, sayBye.c_str(), sayBye.length(), 0);

	outFile.close();
	closesocket(hServSocket);
	cout << "传输结束" << endl;
}
