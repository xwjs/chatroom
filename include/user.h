#ifndef _USER_H
#define _USER_H

#include <set>
#include <list>
#include <vector>
#include <string>

class User
{

public:
    User(const char * uid);

    bool add_group(const char * gid);
    bool delete_group(const char * gid);
    std::list<const char*> list_group()const ;

    bool send_text(const char * gid,const char * text="");
    bool send_text(const char * gid,const std::string & text="");
    bool send_binary(const char * gid,const std::vector<char> & binary=std::vector<char>());

private:
    char * uid;
    std::set<const char*> groups;
};

#endif

