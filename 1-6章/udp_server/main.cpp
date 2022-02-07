#include<Windows.h>
#include<iostream>
using namespace std;

#define BUFFER_SIZE 1024
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
//udp包传输时除了需要包含目的socket信息的外还有自己的，因为他们没有建立连接有可以保存的信息。
int main()
{
	initWSAS();

	SOCKET hServSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//虽然没有类似通过tcp客户端connect建立连接时绑定地址，但也是在sendto过程中分配了套接字地址
	SOCKADDR_IN hServAddr;
	short port = 789;
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	hServAddr.sin_port = htons(port);
	if (bind(hServSocket, (sockaddr*)& hServAddr, sizeof(hServAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error!");
	}
	char buffer[BUFFER_SIZE] = { 0 };
	SOCKADDR_IN hClntAddr;

	int recv_len = 0;
	while (1)//什么时候收到0长度的信息？
	{

		int hClntAddr_len = sizeof(hClntAddr);
		recv_len = recvfrom(hServSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)& hClntAddr, &hClntAddr_len);
		//不需要再创建client的套接字，只需服务器端一个套接字完成收发即可
		cout << "收到消息长度：" << recv_len << endl;
		sendto(hServSocket, buffer, recv_len, 0, (sockaddr*)& hClntAddr, hClntAddr_len);//自己的端口+ip内核会生成，但包需要知道目的信息

	}
	closesocket(hServSocket);
	WSACleanup();
	cout << "结束信息传输" << endl;

}
