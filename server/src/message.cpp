#include "../include/message.h"

static bool null_str(const std::string & s)
{
    return s == "";
}

static bool null_str(const char * s)
{
    return (strcmp(s,"") == 0) || (s == nullptr);
}

xx::Message::Message(const std::string  & text):type(xx::Message::Type::Text),data(text)
{
    if(null_str(text)) type = xx::Message::Type::Null;
}
xx::Message::Message(const char* text) : type(xx::Message::Type::Text), data(text) 
{
    if(null_str(text)) type = xx::Message::Type::Null;
}

xx::Message::Message(const std::vector<char>& binary) : type(xx::Message::Type::Binary), data(binary) 
{
    if(!binary.size()) type = xx::Message::Type::Null;
}

xx::Message::Type xx::Message::get_type() const { return type; }

size_t xx::Message::get_size() const 
{ 
    if(type == xx::Message::Type::Null) return 0;

    return (type == xx::Message::Type::Text) ? std::get<std::string>(data).size() : std::get<std::vector<char>>(data).size();
}

void xx::Message::set_text(const std::string  & text)
{
    type = (null_str(text))? xx::Message::Type::Null : xx::Message::Type::Text; 
    data = std::string(text); 
}

void xx::Message::set_text(const char* text) 
{
    type = (null_str(text))? xx::Message::Type::Null : xx::Message::Type::Text; 
    data = std::string(text); 
}

void xx::Message::set_binary(const std::vector<char>& binary) 
{ 
    type = (binary.size() == 0) ? xx::Message::Type::Null : xx::Message::Type::Binary; 
    data = binary; 
}

std::string xx::Message::get_text() const 
{
    return (type == xx::Message::Type::Text) ? std::get<std::string>(data) : ""; 
}

std::vector<char> xx::Message::get_binary() const 
{ 
    return (type == xx::Message::Type::Binary) ? std::get<std::vector<char>>(data) : std::vector<char>(); 
}
