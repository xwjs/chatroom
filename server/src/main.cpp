#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include <glog/logging.h>

#include "../include/threadpool.h"
#include "../include/server.h"
#include "../include/chatroom.h"

xx::Chatroom * chatroom = & xx::Chatroom::get_chatroom();

namespace po = boost::program_options;

int main(int argc,char * argv[])
{
    FLAGS_alsologtostderr = 1;
    google::InitGoogleLogging(argv[0]);

    // TODO 设置调试级别的参数，区分debug和release
    po::options_description desc("Usage: ./chatroom -t 4 -p 5000 -g 10 -u 100");

    auto threads_size = [](int size)
    {
        if (size < 2) 
        {
            throw po::validation_error(po::validation_error::invalid_option_value, "thread size must be greater than 1");
        }
    };

    auto users_size = [](int size)
    {
        if(size < 1)
        {
            throw po::validation_error(po::validation_error::invalid_option_value, "user size must be greater than 0");
        }
    };

    auto group_size = [](int size)
    {
        if(size < 1)
        {
            throw po::validation_error(po::validation_error::invalid_option_value, "group size must be greater than 0");
        }
    };

    auto port_size = [](int size)
    {
        if(size < 1)
        {
            throw po::validation_error(po::validation_error::invalid_option_value, "port value error");
        }
    };

    desc.add_options()
        ("help,h", "Produce help message")
        ("users,u",po::value<int>()->default_value(100)->notifier(users_size),"max alive user size")
        ("groups,g",po::value<int>()->default_value(10)->notifier(group_size),"max alive groups size")
        ("port,p", po::value<int>()->default_value(5000)->notifier(port_size), "port")
        ("threads,t", po::value<int>()->default_value(4)->notifier(threads_size), "threads size");
    po::variables_map vm;

    try{
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    }catch (const po::error& e) {
        LOG(ERROR) << "\n" << e.what();
        return 1;
    }

    const int PORT = vm["port"].as<int>();
    const int THREADS = vm["threads"].as<int>();
    const int MAX_GROUPS = vm["groups"].as<int>();
    const int MAX_USERS = vm["users"].as<int>();

    // TODO to promote performance , these should be dynamic
    const int MAX_EVENTS = 20;
    const int TIME_OUT = 50;
    struct epoll_event events[MAX_EVENTS];

    LOG(INFO) << "welcome to ChatRoom Copyright (C) 2024 xwj";
    LOG(INFO) << "This program is licensed under the MIT License."; // TODO chagne license

    auto ser_sock = get_binded_socket(PORT);
    auto epoll_fd = get_epoll(ser_sock);

    ThreadPool poll(THREADS);

    LOG(INFO) << "max alive groups:" << MAX_GROUPS << " , " << "max alive users:" << MAX_USERS;
    LOG(INFO) << "working with " << THREADS << " threads, 1 master " << THREADS -1 << "workers"; 
    LOG(INFO) << "listening in port:" << PORT;

    while(true)
    {
        // cope 20 fd , timeout 50ms
        int fd_size = epoll_wait(epoll_fd, events, MAX_EVENTS, TIME_OUT);

        for(int i = 0; i < fd_size;i++)
        {
            int fd = events[i].data.fd;

            // user connect server
            if(fd == ser_sock)
            {
                int cli = accept_client(ser_sock);

                int user_size = chatroom->user_size();

                if(user_size > MAX_USERS) // more than MAX_USER, user connect fail
                {
                    LOG(ERROR) << "chatroom cant take more than " << MAX_USERS << " clients , drop this client:" << get_client_ip(cli);
                    close(cli);
                }
                else // user connnect success
                {
                    LOG(INFO) << "new connect from:"<< get_client_ip(cli) << " client count:" << user_size; 
                    
                    // set cli_sock not block, put int epoll to listen
                    set_client_epoll(cli,epoll_fd);
                }
            }

            // create User , create Group and so on
            else
            {
                pool.enqueue([](int clifd) { 
                    cope_one_client(clifd); 
                }, fd);
            }
        }
    }

    google::ShutdownGoogleLogging();
    return 0;
}
