#include <iostream>
#include <filesystem>
#include <string_view>
#include <regex>
#include <string>
#include <vector>
#include <ranges>
#include <algorithm>
#include "Interpreter.hpp"
#include "datatype.hpp"
using namespace std;

Interpreter::Interpreter(const string dirPath):sql_type_(-1)//未指定,初始值为-1
{
    filesystem::path database(dirPath);//访问目录
    if(!filesystem::exists(database)){//如果没有此目录
        //创建文件夹
        filesystem::create_directory(database);
    }
    //实例化智能指针api
    api = make_unique<DB_API>(new DB_API(dirPath));
}

void Interpreter::GenSQL(){
    //移除换行和tab
    regex reg("[\r\n\t]");
    sql_statement_ = regex_replace(sql_statement_,reg," ");
    //移除;和;之后的字符
    reg = ";.*$";
    sql_statement_ = regex_replace(sql_statement_, reg, "");
    //移除开头和结尾的空格
    reg = "(^ +)|( +$)";
	sql_statement_ = regex_replace(sql_statement_, reg, "");
	//移除重复的空格
	reg = " +";
	sql_statement_ = regex_replace(sql_statement_, reg, " ");
	// 在 ( ) , = <> < > 前后加空格,并去掉多增加的空格
	reg = " ?(\\(|\\)|,|=|(<>)|<|>) ?";
	sql_statement_ = regex_replace(sql_statement_, reg, " $1 ");
	reg = "< *>";
	sql_statement_ = regex_replace(sql_statement_, reg, "<>");
	reg = "< *=";
	sql_statement_ = regex_replace(sql_statement_, reg, "<=");
	reg = "> *=";
	sql_statement_ = regex_replace(sql_statement_, reg, ">=");
	
	//将string转换为vector
	sql_vector_ = SplitSQL(sql_statement_, " ");
}

void Interpreter::ExecuteSQL(const string& statement)
{
    //将statement传递给成员变量
    sql_statement_ = statement;
    try
    {
        //随后开始生成SQL_vector
        GenSQL();
        //获取SQL的类型
        GetSQLType();
        //执行
        PraseSQL();
    }
    catch(const string& e)
    {
        sql_type_ = SQL_ERROR;
        std::cerr << e << '\n';
    }
    
    
}

vector<string> Interpreter::SplitSQL(const string &statement,const string &delim)
{
    vector<string> tokens;
    //跳过开始的分隔符
    auto lastPos = statement.find_first_not_of(delim,0);
    //找第一个非分隔符
    auto pos = statement.find_first_of(delim,lastPos);
    while(string::npos != pos || string::npos != lastPos){
        //找到一段,之后加入返回值向量
        tokens.push_back(statement.substr(lastPos,pos - lastPos));
        //找下一个分隔符
        lastPos = statement.find_first_not_of(delim,pos);
        //找下一个非分隔符
        pos = statement.find_first_of(delim,lastPos);
    }
    return tokens;
}

void Interpreter::GetSQLType(){
    //若sql_vector无数值,则返回消息
    if(sql_vector_.size()==0){
        throw "Error:SQL generate failed\n";
        return;
    }
    //将命令关键字命令变为小写
    transform(sql_vector_[0].begin(),sql_vector_[0].end(),sql_vector_[0].begin(),(int (*)(int))tolower);
    const string& op_first = sql_vector_[0];
    //先解析一元运算符
    if(op_first == "select")
        sql_type_ = SQL_SELECT;
    else if (op_first == "insert")
        sql_type_ = SQL_INSERT;
    else if (op_first == "delete")
        sql_type_ = SQL_DELETE;
    else if (op_first == "use")
        sql_type_ = SQL_USE;
    else if (op_first == "quit")
        sql_type_ = SQL_QUIT;
    else{
        //后解析二元运算符
        if(sql_vector_.size()<2){
            throw "Error:SQL format error\n";
        }
        transform(sql_vector_[1].begin(),sql_vector_[1].end(),sql_vector_[1].begin(),(int (*)(int))tolower);
        const string& op_second = sql_vector_[1];
        if (op_first == "create"){
            if(op_second == "table"){
                sql_type_ = SQL_CREATE_TABLE;
            }
            else if(op_second == "index"){
                sql_type_ = SQL_CREATE_INDEX;
            }else if(op_second == "database"){
                sql_type_ = SQL_CREATE_DATABASE;
            }else{
                throw "Error:SQL format error\n";
            }
        }else if(op_first == "drop"){
            if(op_second == "table"){
                sql_type_ = SQL_DROP_TABLE;
            }
            else if(op_second == "index"){
                sql_type_ = SQL_DROP_INDEX;
            }else if(op_second == "database"){
                sql_type_ = SQL_DROP_DATABASE;
            }else{
                throw "Error:SQL format error\n";
            }
        }else{
            throw "Error:SQL format error\n";
        }
    }
}

void Interpreter::PraseSQL(){
    switch(sql_type_){
        case SQL_CREATE_DATABASE:
        {
            unique_ptr<SQLCreateDatabase> st(new SQLCreateDatabase(sql_vector_));
            api->CreateDatabase(*st);
            break;
        }
        case SQL_CREATE_INDEX:
        {
            unique_ptr<SQLCreateIndex> st(new SQLCreateIndex(sql_vector_));
        }
        case SQL_ALTER:case SQL_ERROR:
            break;
    }
}