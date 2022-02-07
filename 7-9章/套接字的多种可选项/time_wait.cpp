#include<iostream>
#include<Windows.h>
using namespace std;
void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}


	SOCKET tcp_sock;
	tcp_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);



	int option, opt_len;
	opt_len = sizeof(option);
	getsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, (char*)& option, &opt_len);
	cout << "SO_REUSEADDR选项默认值为：" << option << endl;
	option = TRUE;

	setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, (char*)& option, opt_len);//解决time_wait问题


	getsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY, (char*)& option, &opt_len);
	cout << "TCP_NODELAY选项默认值为：" << option << endl;
	option = TRUE;

	setsockopt(tcp_sock, SOL_SOCKET, TCP_NODELAY, (char*)& option, opt_len);//关闭nagle算法，尽可能快的发送数据





	SOCKADDR_IN serv_adr, clnt_adr;

	memset(&serv_adr, 0, sizeof(SOCKADDR_IN));
	serv_adr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(789);

	if (bind(tcp_sock, (sockaddr*)& serv_adr, sizeof(serv_adr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	if (listen(tcp_sock, 5) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}

	SOCKET tcp_sock_client;
	int clnt_adr_len = sizeof(clnt_adr);
	tcp_sock_client = accept(tcp_sock, (sockaddr*)& clnt_adr, &clnt_adr_len);

	int str_len;
	char buffer[1024] = { 0 };
	//while ((str_len = recv(tcp_sock_client, buffer, sizeof(buffer) - 1, 0)) != 0)
	//{
	//	send(tcp_sock_client, buffer, str_len, 0);
	//	buffer[str_len] = '\0';
	//	cout << buffer << endl;
	//}

	str_len = recv(tcp_sock_client, buffer, sizeof(buffer) - 1, 0);

	send(tcp_sock_client, buffer, str_len, 0);
	buffer[str_len] = '\0';
	cout << buffer << endl;


	closesocket(tcp_sock);
	closesocket(tcp_sock_client);
	WSACleanup();

}
