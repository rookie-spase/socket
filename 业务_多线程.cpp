#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> // add signal.h header
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h> // add pthread.h header
using namespace std;

string getresfromxml(string res, string tag) {
    string start_tag = "<" + tag + ">";
    string end_tag = "</" + tag + ">";
    auto start = res.find(start_tag);
    if (start == string::npos) {
        return "";
    }
    start += start_tag.length();
    auto end = res.rfind(end_tag); // use rfind to reverse search for end_tag
    if (end == string::npos) {
        return "";
    }
    return res.substr(start, end - start);
};

void yw_account(char* recv,char* send){
    string account_str = getresfromxml(recv, "account");
    string pwd_str = getresfromxml(recv, "pwd");
    if (account_str == "root" && pwd_str == "123456") {
        strcpy(send, "login success");
    } else {
        strcpy(send, "login fail");
    }
}

void* main_(void* arg) {
  pthread_detach(pthread_self());  // 分离线程。
  pthread_setcanceltype(PTHREAD_CANCEL_DISABLE,NULL);  // 设置取消方式为立即取消。
    
    int clientfd = *(int*)arg; // cast arg to int pointer and dereference it to get clientfd
    char res[1024] = { 0 };
    memset(res, 0, sizeof(res)); // initialize res to 0

    int valread = recv(clientfd, res, 1024, 0);
    if (valread <= 0) {
        close(clientfd); // close client socket
        pthread_exit(0); // exit thread
    }

    string code_str = getresfromxml(res, "code");
    int codes = atoi(code_str.c_str()); // convert code_str to int
    char ses[1024]={0};
    switch(codes){
        case 1:
            yw_account(res,ses);break;
        case 2:
            //yw_hello(res,ses);break;
            break;
        default:
            break;
    }

    std::cout << "recv:" << res << std::endl;
    
    int send_result = send(clientfd, ses, strlen(ses), 0); // send ses to client
    if (send_result < 0) { // check if send fails
        std::cerr << "Error sending message to client" << std::endl;
        close(clientfd); // close client socket
        pthread_exit(0); // exit thread
    }

    std::cout << "send:" << ses << std::endl;

    close(clientfd); // close client socket
    pthread_exit(0); // exit thread
}

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


    if (bind(sockfd, (struct sockaddr*)&server, sizeof(struct sockaddr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        return 1; // return 1 if bind fails
    }

    if (listen(sockfd, 50) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }
    pthread_t threads[50]; // declare array of threads
    int thread_count = 0; // initialize thread count to 0

    while (true) {
        struct sockaddr_in client;
        socklen_t client_len = sizeof(client);
        int clientfd = accept(sockfd, (struct sockaddr*)&client, &client_len);

        if (clientfd < 0) { // check if accept fails
            std::cerr << "Error accepting client connection" << std::endl;
            continue; // continue to next iteration of loop
        }

        if (thread_count >= 50) { // check if maximum number of threads reached
            std::cerr << "Maximum number of threads reached" << std::endl;
            close(clientfd); // close client socket
            continue; // continue to next iteration of loop
        }

        int* arg = new int; // allocate memory for arg
        *arg = clientfd; // set arg to clientfd

        pthread_create(&threads[thread_count], NULL, main_, (void*)arg); // create thread
        thread_count++; // increment thread count
    };

    close(sockfd); // close server socket
    pthread_exit(NULL); // exit main thread
};
