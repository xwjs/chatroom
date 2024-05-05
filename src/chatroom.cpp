#include "../include/chatroom.h"
#include "chatroom.h"

xx::Chatroom * xx::Chatroom::chatroom = nullptr;
size_t xx::Chatroom::group_size;
std::set<xx::Group*> xx::Chatroom::groups;
std::set<xx::User*> xx::Chatroom::users;
std::mutex xx::Chatroom::mutex_;

// user
xx::User::User(const char * uid):uid(uid){}

xx::User::~User() {}

bool xx::User::add_group(Group *)
{
    return false;
}

void xx::User::delete_group(Group *)
{
}

std::list<xx::Group *> xx::User::list_group() const
{
    return std::list<xx::Group*>(groups.begin(),groups.end());
}

bool send_message(xx::Group * g,xx::Message * m)
{
    
}

// group
xx::Group::Group(const char * gid,size_t max) :gid(gid),max(max) {}

xx::Group::~Group(){}

bool xx::Group::add_user(User *)
{
    return false;
}

void xx::Group::delete_user(User *)
{
}

void xx::Group::broadcast(Group *g, Message *m)
{
}

std::list<xx::User*> xx::Group::list_user()const
{
    return std::list<User*>(users.begin(),users.end());
}

// chatroom
xx::Chatroom::Chatroom() {}

xx::Chatroom::~Chatroom() {}

xx::Chatroom & xx::Chatroom::get_chatroom(size_t g_size)
{
    if(chatroom == nullptr)
    {
        std::lock_guard<std::mutex> lock(mutex_);

        if(chatroom == nullptr)
        {
            chatroom = new Chatroom();
            LOG(INFO) << "create chatroom";
        }   
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        xx::Chatroom::group_size = g_size;
        LOG(INFO) << "change chatroom group size to " << xx::Chatroom::group_size;
    }

    return *chatroom;
}

void xx::Chatroom::rm_chatroom()
{
    std::lock_guard<std::mutex> lock(mutex_);
    LOG(INFO) << "destroy chatroom";
    for(auto i : groups)
    {
        delete i;
    }

    for(auto i : users)
    {
        delete i;
    }
}

static std::string hash() 
{
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    std::hash<long long> hash_fn;
    long long hash_value = hash_fn(timestamp);

    std::ostringstream oss;
    oss << hash_value;
    std::string hash_str = oss.str();
    
    if (hash_str.length() > 20) 
    {
        hash_str = hash_str.substr(0, 20);
    }
    
    return hash_str;
}

xx::User * xx::Chatroom::create_user()
{
    std::lock_guard<std::mutex> lock(mutex_);
    return new User(hash().c_str());
}

xx::Group * xx::Chatroom::create_group(size_t group_size)
{
    std::lock_guard<std::mutex> lock(mutex_);
    return new Group(hash().c_str(),group_size);
}

void xx::Chatroom::add_user(User *u, Group *g)
{
    if(u && g)
    {
        u->add_group(g);
        g->add_user(u);
    }
}

void xx::Chatroom::delete_user(User *u, Group *g)
{

}

void xx::Chatroom::delete_group(Group *g)
{

}

std::list<xx::Group *> xx::Chatroom::list_group() const
{
    return std::list<Group *>(groups.begin(),groups.end());
}

std::list<xx::Group *> xx::Chatroom::list_group(User *u) const
{
    return u->list_group();
}

std::list<xx::User *> xx::Chatroom::list_user() const
{
    return std::list<User*>(users.begin(),users.end());
}

std::list<xx::User *> xx::Chatroom::list_user(Group *g) const
{
    return g->list_user();
}

void xx::Chatroom::broadcast(Group *g, Message *m)
{
}
