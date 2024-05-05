#ifndef _CHATROOM_H_
#define _CHATROOM_H_

#include <cstddef>
#include <unordered_map>
#include <list>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <functional>
#include <sstream>
#include "message.h"
#include "log.h"



namespace xx
{
    class Group;

    class User
    {
        public:
            User( const char * uid);
            ~User();

            bool add_group(Group *);
            void delete_group(Group *);

            const char * get_uid()const ;
            std::list<Group*> list_group()const ;
            
            bool send_message(Group * g,Message * m)const ;

        private:
            const char * uid;

            // storage active groups
            std::unordered_map<const char * ,Group * > groups;
    };

    class Group
    {
        public:
            Group(const char * gid,size_t max);
            ~Group();

            const char * get_gid()const;

            bool add_user(User * );
            void delete_user(User *);
            void broadcast(Group * g,Message * m);

            std::list<User*>  list_user()const;
        private:
            const char * gid;
            size_t max;
            std::unordered_map<const char * ,User * > users;
    };

    class Chatroom
    {
        public:
            static Chatroom & get_chatroom(size_t group_size = 5);
            static void rm_chatroom();

            User * create_user();
            Group * create_group(size_t group_size=10);

            void add_user(User * u,Group * g);
            void remove_user(User * u,Group *g);

            void delete_user(User * u,Group * g);
            void delete_group(Group * g);

            std::list<Group*> list_group(User * u)const ;

            std::list<User*>  list_user(Group * g)const;
        private:
            Chatroom();
            ~Chatroom();

            Chatroom(const Chatroom &)=delete;
            Chatroom & operator=(const Chatroom &)=delete;

            static Chatroom * chatroom;
            static std::mutex mutex_;

            static size_t group_size;
            static std::set<Group*> groups;
            static std::set<User*>  users;
    };
};

#endif
