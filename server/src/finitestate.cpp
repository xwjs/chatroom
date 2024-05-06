#include "../include/finitestate.h"

// TODO
void cope_message(const string &s)
{
    auto js = json::parse(s);

    const char * uid  =  js["uid"];
    const char * gid  =  js["gid"];
    const char * type =  js["type"];
    const char * data =  js["data"];
    const char * op   =  js["opearte"];
}