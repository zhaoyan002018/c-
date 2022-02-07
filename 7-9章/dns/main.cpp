#include<iostream>
#include<Windows.h>
//#include<WinSock2.h>
//#include <WS2tcpip.h>
using namespace std;
void ErrorHandling(const char* message)
{
	cerr << message;
	exit(1);
}

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	hostent* host;
	//host = gethostbyname("www.baidu.com");

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(addr));
	const char* ip = "103.235.46.39";
	addr.sin_addr.S_un.S_addr = inet_addr(ip);
	//inet_pton(AF_INET, ip, (void*)& addr.sin_addr.S_un.S_addr);
	host = gethostbyaddr((char*) & (addr.sin_addr), 4, AF_INET);
	//getnameinfo

	if (!host)
	{
		ErrorHandling("gethost... error");
	}

	cout << host->h_name << endl;
	int i = 0;
	for (i = 0; host->h_aliases[i]; i++)
		cout << "Aliases " << i << " :" << host->h_aliases[i] << '\t';
	cout << endl;

	cout << "Address types :" << ((host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6") << endl;

	for (i = 0; host->h_addr_list[i]; i++)//注意保存的是in_addr的地址
		cout << "ip addr " << i << " :" << inet_ntoa(*((in_addr*)host->h_addr_list[i])) << '\t';

}
