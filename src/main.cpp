#include <set>
#include <iostream>
#include <cstring>
#include "../include/server.h"
#include "../include/message.h" // Include Message header
#include "../include/user.h"
#define MAX_CLIENT 5
#define MAX_EVENTS 5
#define MAX_BUFFER_SIZE 255

int main()
{
    auto ser_sock = get_binded_socket(5003);

    set_nonblock(ser_sock);

    auto epoll_fd = get_epoll(ser_sock);

    std::set<int> cli_sock;

    while(1)
    {
        struct epoll_event events[MAX_EVENTS];

        int fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for(int i = 0; i < fds; i++)
        {
            int fd = events[i].data.fd;

            if(fd == ser_sock)
            {
                sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int cli = accept(ser_sock, (sockaddr*)&clientAddr, &clientAddrLen);

                if(cli_sock.size() >= MAX_CLIENT)
                {
                    // Reach max clients
                    std::cout << "Max clients reached. Connection rejected." << std::endl;
                    close(cli);
                    continue;
                }

                cli_sock.insert(cli);
                set_nonblock(cli);

                epoll_event event;
                event.events = EPOLLIN;
                event.data.fd = cli;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli, &event);

                std::cout << "New client connected. Client count: " << cli_sock.size() << std::endl;
            }
            else
            {
                char buf[MAX_BUFFER_SIZE];
                int re = read(fd, buf, sizeof(buf));

                if(re > 0)
                {
                    // Wrap received message into Message object
                    Message receivedMessage(buf);

                    // Broadcast message to all clients
                    for(auto const& cli : cli_sock)
                    {
                        if(cli != fd) // Don't echo back to sender
                        {
                            // Get binary representation of Message object
                            auto binaryData = receivedMessage.get_text();

                            // Write binary data to client
                            int written = write(cli, binaryData.data(), binaryData.size());
                            if(written < 0)
                            {
                                // Handle write error
                                std::cout << "Write error to client " << cli << std::endl;
                            }
                        }
                    }
                }
                else if(re == 0)
                {
                    // Client disconnected
                    std::cout << "Client " << fd << " disconnected." << std::endl;
                    close(fd);
                    cli_sock.erase(fd);
                }
                else
                {
                    // Handle read error
                    std::cout << "Read error from client " << fd << std::endl;
                }
            }
        }
    }
}
