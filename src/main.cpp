#include "../include/message.h"
#include "../include/server.h"
#include "../include/chatroom.h"
#include <glog/logging.h>

#define MAX_CLIENT 5
#define MAX_EVENTS 5
#define MAX_BUFFER_SIZE 255

int main(int argc,char * argv[])
{
    FLAGS_alsologtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    LOG(INFO) << "welcom to chatroom designed by xwj";
    
    auto ser_sock = get_binded_socket();
    set_nonblock(ser_sock);

    auto epoll_fd = get_epoll(ser_sock);

    xx::Chatroom * chatroom = & xx::Chatroom::get_chatroom();
    std::set<int> cli_sock;

    while(1)
    {
        struct epoll_event events[MAX_EVENTS];

        LOG(INFO) << "epoll waiting";
        int fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for(int i = 0; i < fds; i++)
        {
            int fd = events[i].data.fd;

            // cope new connect
            if(fd == ser_sock)
            {
                sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int cli = accept(ser_sock, (sockaddr*)&clientAddr, &clientAddrLen);

                if(cli_sock.size() >= MAX_CLIENT)
                {
                    LOG(ERROR) << "chatroom cant take more than " << MAX_CLIENT << " clients , drop this appectance";
                    close(cli);
                }
                else
                {
                    LOG(INFO) << "new connect:" << "client_fd[" << cli << "] ,ip:"<< get_client_ip(cli) << " client count:" << cli_sock.size(); 
                    cli_sock.insert(cli);
                    set_nonblock(cli);

                    epoll_event event;
                    event.events = EPOLLIN;
                    event.data.fd = cli;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli, &event);
                }
            }
            else // cope message
            {

            }
        }
    }


    google::ShutdownGoogleLogging();
    return 0;
}
