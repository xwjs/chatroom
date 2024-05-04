#ifndef _GROUP_H_
#define _GROUP_H_

#include <set>
#include <list>

class Group
{
    public:
        Group(const char * gid,int max = 10);

        int delete_user(const char * uid);
        int add_user(const char * uid);
        std::list<const char*> list_users()const;

        void broadcast()const;
    private:
        int max;
        char * gid;
        std::set<const char*> users;
};

#endif
