//定义程序所需要的所有异常类
#pragma once
#include <exception>
#include <string>


//在Interpreter中使用的报错
class SQLTypeError: public std::exception
{
public:
    SQLTypeError(const std::string& msg):msg_("SQLTypeError:"+msg){}
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
    SQLSyntaxError(const std::string& msg):msg_("SQLSyntaxError:"+msg){}
    const char* what() const noexcept override{
        return msg_.c_str();
    }
private:
    std::string msg_;
};
//在DBmanager中使用的报错
class DBManagerError: public std::exception
{
public:
    DBManagerError(const std::string& msg):msg_("DBManagerError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};


//在Table中使用的报错
class TableOpenError: public std::exception
{
public:
    TableOpenError(const std::string& msg):msg_("TableOpenError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};

class TableDataError: public std::exception
{
public:
    TableDataError(const std::string& msg):msg_("TableDataError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};

class TableIndexError: public std::exception
{
public:
    TableIndexError(const std::string& msg):msg_("TableIndexError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};

class TableIndexExist: public std::exception
{
public:
    TableIndexExist(const std::string& msg):msg_("TableIndexExist:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }   
private:
    std::string msg_;
}; 

class TableCreateError: public std::exception
{
public:
    TableCreateError(const std::string& msg):msg_("TableCreateError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};
class TableDeleteError: public std::exception
{
public:
    TableDeleteError(const std::string& msg):msg_("TableDeleteError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};
//ColValue相关的错误
class ColValueError: public std::exception
{
public:
    ColValueError(const std::string& msg):msg_("ColValueError:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};
//BPTree相关的错误
class BPTreeException: public std::exception
{
public:
    BPTreeException(const std::string& msg):msg_("BPTreeException:"+msg){}
    const char* what() const noexcept override{
        return (msg_).c_str();
    }
private:
    std::string msg_;
};