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

const char* DST_IP = "127.0.0.1";
const short DST_PORT = 789;
int main()
{
	initWSAS();

	SOCKET hServSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//整个过程中，代码中不需要知道客户端的ip和端口，这些都在运行时在内核中获取

	SOCKADDR_IN hServAddr;//将要发送的目的地址
	hServAddr.sin_addr.S_un.S_addr = inet_addr(DST_IP);
	hServAddr.sin_family = AF_INET;
	hServAddr.sin_port = htons(DST_PORT);
	int hServAddrLen = sizeof(hServAddr);

	connect(hServSocket, (sockaddr*)& hServAddr, sizeof(hServAddr));//可不使用连接,那就必须使用sendto每次发送时构造。

	string str;
	char  recv_buf[BUFFER_SIZE] = { 0 };
	int recv_len = 0;
	while (1)
	{
		cout << "请输入字符(Q to quit)：" << endl;
		cin >> str;
		str.append("\n");
		if (!str.compare("q\n") || !str.compare("Q\n"))
		{
			break;
		}

		/*	fgets(recv_buf, sizeof(recv_buf), stdin);
			if (!strcmp(recv_buf, "q\n") || !strcmp(recv_buf, "Q\n"))
			{
				break;
			}
	*/

	//int send_len = sendto(hClntSocket, str.c_str(), str.length(), 0, (sockaddr*)& hServAddr, sizeof(hServAddr));
		int send_len = send(hServSocket, str.c_str(), str.length(), 0);

		//cout << send_Len << endl;
		SOCKADDR_IN hFromAddr;
		int hFromAddrLen = sizeof(hFromAddr);
		recv_len = recvfrom(hServSocket, recv_buf, send_len, 0, (sockaddr*)& hFromAddr, &hFromAddrLen);//这是由于发送和接收的sokcet是同一个可以这么写，否则需要分开。
		recv_buf[recv_len] = '\0';
		cout << "得到回响：" << recv_buf << endl;
	}
	closesocket(hServSocket);
	WSACleanup();

}
