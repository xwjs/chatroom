#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>

int get_binded_socket(int port);

int get_epoll(int serverSocket);

int set_nonblock(int fd);

#endif
