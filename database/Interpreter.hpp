#pragma once
#include "datatype.hpp"
using namespace std;
class Interpreter
{
private:
    /* data */
    string sql_statement_;//传入的SQL
    vector<string> sql_vector_;//将string解析为向量
    uint8 sql_type_;//SQL类型,便于解析
    void GenSQL();//利用正则表达式将SQL解析为sql_vector_
    vector<string> SplitSQL(const string &statement,const string &delim);
public:
    Interpreter(const string dirPath);
    ~Interpreter();
    void ExecuteSQL(const string& statement);//实现SQL解析

};

