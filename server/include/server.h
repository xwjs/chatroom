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

#include <glog/logging.h>

/// @brief create socket listening in port
/// @param port 
/// @return socket_fd
int get_binded_socket(int port);

/// @brief create epoll , set ser_socket non block , and bind epoll ser_sock
/// @param serverSocket 
/// @return epoll_fd
int get_epoll(int serverSocket);

/// @brief set fd non block
/// @param fd 
/// @return 
void set_nonblock(int fd);

/// @brief get client ip from active socket
/// @param fd 
/// @return ip
char * get_client_ip(int fd); 

/// @brief server accpet client
/// @param ser_fd 
/// @return client_fd
int accept_client(int ser_fd);

/// @brief set clifd non block , put client into epoll to listen
/// @param clifd 
/// @param epoll_fd 
void set_client_epoll(int clifd,int epoll_fd);

void cope_one_client(int clifd);

#endif
