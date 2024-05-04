#include "../include/message.h"

static bool null_str(const std::string & s)
{
    return s == "";
}

static bool null_str(const char * s)
{
    return (strcmp(s,"") == 0) || (s == nullptr);
}

Message::Message(const std::string  & text):type(Type::Text),data(text)
{
    if(null_str(text)) type = Type::Null;
}
Message::Message(const char* text) : type(Type::Text), data(text) 
{
    if(null_str(text)) type = Type::Null;
}

Message::Message(const std::vector<char>& binary) : type(Type::Binary), data(binary) 
{
    if(!binary.size()) type = Type::Null;
}

Message::Type Message::get_type() const { return type; }

size_t Message::get_size() const 
{ 
    if(type == Type::Null) return 0;

    return (type == Type::Text) ? std::get<std::string>(data).size() : std::get<std::vector<char>>(data).size();
}

void Message::set_text(const std::string  & text)
{
    type = (null_str(text))? Type::Null : Type::Text; 
    data = std::string(text); 
}

void Message::set_text(const char* text) 
{
    type = (null_str(text))? Type::Null : Type::Text; 
    data = std::string(text); 
}

void Message::set_binary(const std::vector<char>& binary) 
{ 
    type = (binary.size() == 0) ? Type::Null:Type::Binary; 
    data = binary; 
}

std::string Message::get_text() const 
{
    return (type == Type::Text) ? std::get<std::string>(data) : ""; 
}

std::vector<char> Message::get_binary() const 
{ 
    return (type == Type::Binary) ? std::get<std::vector<char>>(data) : std::vector<char>(); 
}
