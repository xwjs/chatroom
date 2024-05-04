#include "../include/server.h"


int get_binded_socket(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        return -1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == -1) {
        close(serverSocket);
        return -1;
    }

    return serverSocket;
}

int set_nonblock(int fd)
{
    int old = fcntl(fd,F_GETFL);
    int new_ = old | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_);
    return old;
}

int get_epoll(int serverSocket) {
    int epollFd = epoll_create1(0);
    if (epollFd == -1) 
    {
        return -1;
    }

    epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        close(serverSocket);
        close(epollFd);
        return -1;
    }

    return epollFd;
}


