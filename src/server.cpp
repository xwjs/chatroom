#include "../include/server.h"


int get_binded_socket(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) { LOG(FATAL) << "create server socket error";}

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    LOG(INFO) << "set tcp(v4)socket in port " << port << " accept from any ip";

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) 
    {
        close(serverSocket);
        LOG(FATAL) << "bind socket error";
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        close(serverSocket);
        LOG(FATAL) << "listen socket error";
    }

    LOG(INFO) << "create socket success , socket_fd[" << serverSocket << "]";
    return serverSocket;
}

int set_nonblock(int fd)
{
    LOG(INFO) << "set nonblock fd[" << fd <<"]";
    int old = fcntl(fd,F_GETFL);
    int new_ = old | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_);
    return old;
}

int get_epoll(int serverSocket) {
    int epollFd = epoll_create1(0);
    if (epollFd == -1) 
    {
        LOG(FATAL) << "create epoll error";
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

    LOG(INFO) << "bind socket to epoll success ,epoll_fd[" << epollFd << "]";
    return epollFd;
}

char * get_client_ip(int clientSocketFD) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    if (getpeername(clientSocketFD, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) 
    {
        LOG(ERROR) << "get client ip error ,client_fd[" << clientSocketFD << "]";
        return "";    
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    return clientIP;
}

