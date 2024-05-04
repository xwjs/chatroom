#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <vector>
#include <variant>
#include <string>
#include <cstring>

class Message
{
public:
    enum class Type
    {
        Text, Binary,Null
    };
    
    Message(const std::string & text);
    Message(const char* text);
    Message(const std::vector<char>& binary);

    Type get_type() const;
    size_t get_size() const;

    void set_text(const std::string & text);
    void set_text(const char* text);
    void set_binary(const std::vector<char>& binary);

    std::string get_text() const;
    std::vector<char> get_binary() const;

private:
    Type type;
    std::variant<std::string, std::vector<char>> data;

    template<typename T>
    T get() const 
    {
        return std::get<T>(data);
    }
};

#endif
