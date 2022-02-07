
#include<Windows.h>
#include"tcp_client.h"
const short HOST_PORT = 789;
const char* HOST_IP = "127.0.0.1";

int main()
{
	//return 1;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	SOCKET hSevrSocket;
	//创建远程套接字，本地套接字由connect在内核自动生成，ip为本地host，port随机生成
	hSevrSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN hServtAddr;
	//inet_pton(AF_INET, HOST_IP, (void*)& hServtAddr.sin_addr.S_un.S_addr);

	hServtAddr.sin_addr.S_un.S_addr = inet_addr(HOST_IP);
	hServtAddr.sin_family = AF_INET;
	hServtAddr.sin_port = htons(HOST_PORT);

	//连接远程客户端
	if (connect(hSevrSocket, (SOCKADDR*)& hServtAddr, sizeof(hServtAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("connect() error。");
	}



	string str;
	int str_len;
	char recStr[BUFF_SIZE] = { 0 };
	while (1)
	{
		str.clear();
		cout << "请输入要发送的字符(Q to quit)" << endl;;
		cin >> str;
		str.append("\n");

		if (!str.compare("Q\n") || !str.compare("q\n"))
			break;

		str_len = send(hSevrSocket, str.c_str(), str.length(), 0);

		//不能期待一次recv将所有发送出去的字符读回来，因为tcp流传输是没有数据边界的，需要有机制保证我们要读写的字符串长度，和二进制读写类似。
			//方案一，固定传输数据的长度
		int recv_len = 0, recv_cnt;
		while (recv_len < str_len)
		{
			recv_cnt = recv(hSevrSocket, recStr, BUFF_SIZE - 1, 0);
			if (recv_cnt == 0) {
				break;
			}
			if (recv_cnt == -1)
			{
				ErrorHandling("recv() error。");
			}
			recv_len += recv_cnt;
		}

		recStr[str_len] = '\0';
		cout << "返回的字符：" << recStr << endl;
	}


	//方案二，人为制定数据边界，即数据协议，定义应用层协议
	//先发送整型数据，再发送字符型运算符
	/*int number;
	char op;
	char buffer[BUFF_SIZE] = { 0 };
	long result;
	for (int i = 0; i < 3; i++)
	{

		cout << "请输入数据:" << endl;
		cin >> number;
		send(hSevrSocket, (const char*)& number, sizeof(number), 0);
	}
	cout << "请输入运算符:" << endl;
	cin >> op;
	send(hSevrSocket, &op, sizeof(op), 0);

	int recv_len = 0, recv_cnt;
	while (recv_len < sizeof(result))
	{
		recv_cnt = recv(hSevrSocket, buffer, BUFF_SIZE, 0);
		if (recv_cnt < 0)
		{
			ErrorHandling("recv() error。");
		}
		recv_len += recv_cnt;
	}
	result = (long)buffer[0];

	cout << "返回的结果为" << result << endl;*/

	cout << "结束数据传输" << endl;
	closesocket(hSevrSocket);
	WSACleanup();

}

void ErrorHandling(const char* message)
{
	cerr << message << endl;
	exit(-1);
}
