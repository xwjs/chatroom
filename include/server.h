#ifndef _SERVER_H_
#define _SERVER_H_

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <vector>
#include "log.h"

int get_binded_socket(int port=5000);

int get_epoll(int serverSocket);

int set_nonblock(int fd);

char * get_client_ip(int fd); 



#endif
