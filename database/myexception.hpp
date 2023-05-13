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
        return ("SQLTypeError:"+msg_).c_str();
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
        return ("SQLSyntaxError:"+msg_).c_str();
    }
private:
    std::string msg_;
};

//在table中使用的报错
class TableOpenError: public std::exception
{
public:
    TableOpenError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return ("TableOpenError:"+msg_).c_str();
    }
private:
    std::string msg_;
};

class TableDataError: public std::exception
{
public:
    TableDataError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return ("TableDataError:"+msg_).c_str();
    }
private:
    std::string msg_;
};

class TableIndexError: public std::exception
{
public:
    TableIndexError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return ("TableIndexError:"+msg_).c_str();
    }
private:
    std::string msg_;
};

class TableIndexExist: public std::exception
{
public:
    TableIndexExist(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return ("TableIndexExist:"+msg_).c_str();
    }   
private:
    std::string msg_;
}; 

class TableCreateError: public std::exception
{
public:
    TableCreateError(const std::string& msg):msg_(msg){}
    const char* what() const noexcept override{
        return ("TableCreateError:"+msg_).c_str();
    }
private:
    std::string msg_;
};