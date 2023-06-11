#pragma once
#include "datatype.hpp"
#include "SQLstatement.hpp"
#include "db_api.hpp"
#include <vector>
#include <string>
#include <memory>
using namespace std;
class Interpreter
{
private:
    string sql_statement_;//传入的SQL
    vector<string> sql_vector_;//将string解析为向量
    uint8 sql_type_;//SQL类型,便于解析
    void GenSQL();//利用正则表达式将SQL解析为sql_vector_
    vector<string> SplitSQL(const string &statement,const string &delim);
    void GetSQLType();//获取SQL的类型
    void PraseSQL();//对接API接口并调用
    unique_ptr<DB_API> api;//智能指针
public:
    Interpreter();//测试用
    ~Interpreter();
    vector<string> get_gen_SQL(string &statement);//测试GenSQL用
    Interpreter(const string dirPath);
    void ExecuteSQL(const string& statement);//实现SQL解析
    void Quit();//退出数据库
};

