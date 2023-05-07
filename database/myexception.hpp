//定义程序所需要的所有异常类
#pragma once
#include <exception>
#include <string>


//在Interpreter中使用的报错
class SQLTypeError: public std::exception
{
public:
    SQLTypeError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return msg_.c_str();
    }
private:
    std::string msg_;
};

//在SQLstatement中使用的报错
class SQLSyntaxError: public std::exception
{
public:
    SQLSyntaxError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return msg_.c_str();
    }
private:
    std::string msg_;
};
