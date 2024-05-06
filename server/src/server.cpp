#include "../include/server.h"
#include "server.h"

int get_binded_socket(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) { LOG(FATAL) << "create server socket error , check system if out of resource";}

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) 
    {
        close(serverSocket);
        LOG(FATAL) << "bind socket error";
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        close(serverSocket);
        LOG(FATAL) << "listen socket error";
    }

    return serverSocket;
}

void set_nonblock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


int get_epoll(int serverSocket) {
    set_nonblock(serverSocket);
    int epollFd = epoll_create1(0);
    if (epollFd == -1) 
    {
        LOG(FATAL) << "create epoll error , check system if out of resource";
    }

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) 
    {
        close(serverSocket);
        close(epollFd);
        LOG(FATAL) << "bind socket to epoll error";
    }

    return epollFd;
}

char * get_client_ip(int clientSocketFD) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    if (getpeername(clientSocketFD, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) 
    {
        return "";    
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    return clientIP;
}

int accept_client(int ser_fd)
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int cli = accept(ser_fd, (sockaddr*)&clientAddr, &clientAddrLen);
    return cli;
}

void set_client_epoll(int clifd, int epoll_fd)
{
    set_nonblock(clifd);
    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clifd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clifd, &event);
}

void cope_one_client(int clifd);