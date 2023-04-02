#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sockfd; // declare socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
	std::cout << getpid() << std::endl;
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5555); // set port number

    if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    char message[] = "hello";
    send(sockfd, message, strlen(message), 0); // send message to server

    char res[1024] = {0};
    int valread = recv(sockfd, res, 1024, 0);
    if (valread <= 0) {
        std::cerr << "Error receiving response from server" << std::endl;
        return 1;
    }
    std::cout << res << std::endl;

    close(sockfd); // close socket
}

