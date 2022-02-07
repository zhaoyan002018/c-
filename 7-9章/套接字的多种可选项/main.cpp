//#include<socketapi.h>
#include<iostream>
#include<Windows.h>
using namespace std;
void ErrorHandling(const char* message)
{
	cerr << message << endl;
	exit(-1);
}
int main1()
{
	return 0;
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	SOCKET tcp_sock, udp_sock;
	tcp_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	udp_sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);


	//int sock_type = 0, opt_len = sizeof(sock_type);
	//int state;
	//state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (char*)& sock_type, &opt_len);
	//if (state == SOCKET_ERROR)
	//	ErrorHandling("getsockopt() error");
	//cout << "socket type one:" << sock_type << endl;

	//state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (char*)& sock_type, &opt_len);
	//if (state == SOCKET_ERROR)
	//	ErrorHandling("getsockopt() error");
	//cout << "socket type two:" << sock_type << endl;

	int send_buf, rcv_buf, state, opt_len = sizeof(send_buf);

	send_buf = 1024;
	state = setsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (char*)& send_buf, opt_len);
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error");
	rcv_buf = 1024;
	state = setsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (char*)& rcv_buf, opt_len);
	if (state == SOCKET_ERROR)
		ErrorHandling("setsockopt() error");


	state = getsockopt(tcp_sock, SOL_SOCKET, SO_SNDBUF, (char*)& send_buf, &opt_len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");
	cout << "socket send buffer size:" << send_buf << endl;

	state = getsockopt(tcp_sock, SOL_SOCKET, SO_RCVBUF, (char*)& rcv_buf, &opt_len);
	if (state == SOCKET_ERROR)
		ErrorHandling("getsockopt() error");
	cout << "socket recv buffer size:" << rcv_buf << endl;

	closesocket(tcp_sock);
	closesocket(udp_sock);
	WSACleanup();

}
