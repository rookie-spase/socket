#include"Socket.h"

TcpSocket::TcpSocket(const int& port)
	:client_fd(-1),listend(socket(AF_INET, SOCK_STREAM, 0))
{
		memset(buffer, 0, sizeof(buffer));


		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr.s_addr = htonl(INADDR_ANY);

		memset(&client_addr,0,sizeof(client_addr));
}

void TcpSocket::Accept()
{
		int socklen = sizeof(client_addr);
		client_fd = accept(listend, (struct sockaddr*)&client_addr, (socklen_t*)&socklen);
		std::cout << "client " << inet_ntoa(client_addr.sin_addr) << " Linked." << std::endl;
	
}

int TcpSocket::Send(const char* str)
{
		memset(buffer, 0, sizeof(buffer));
		strcpy(buffer, str);
		return send(client_fd, buffer, strlen(buffer),0);
}

TcpClient::TcpClient(const int& port, const char* ip)
	:fd(socket(AF_INET, SOCK_STREAM, 0))
{
	struct hostent* h = gethostbyname(ip);

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	memcpy(&server.sin_addr, h->h_addr, h->h_length); // 将ip地址放到服务器类
}