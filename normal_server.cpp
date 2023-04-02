#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int main() {
    int sockfd; // declare socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create socket
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5555); // set port number


    if (bind(sockfd,(struct sockaddr*)&server,sizeof(struct sockaddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(sockfd,50) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }
    
    char res[1024] = {0};
    
    while(true){
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int clientfd = accept(sockfd, (struct sockaddr*)&client, &client_len);

        memset(res, 0, sizeof(res)); // initialize res to 0

        int valread = recv(clientfd, res, 1024, 0);
        if (valread <= 0) {
            break;
	
    close(clientfd); // close client socket
        }
        std::cout << res << std::endl;

        strcat(res, "ok"); // concatenate "ok" to res

        send(clientfd, res, strlen(res), 0); // send res to client
        std::cout << res << std::endl;

        
    }
    close(sockfd); // close server socket
}

