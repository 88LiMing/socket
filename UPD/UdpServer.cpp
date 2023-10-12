#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error in socket creation." << std::endl;
        return -1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error in socket binding." << std::endl;
        return -1;
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    socklen_t clientAddrLen = sizeof(clientAddr);
    ssize_t msgLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (msgLen < 0) {
        std::cerr << "Error in receiving message." << std::endl;
        return -1;
    }

    std::cout << "Received message from client: " << buffer << std::endl;

    close(sockfd);

    return 0;
}
