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
#include "MyException.hpp"
using namespace std;
Interpreter::Interpreter():sql_type_(SQL_ERROR)
{
    //测试SQL引擎用的构造方式
}
Interpreter::~Interpreter()
{
}
Interpreter::Interpreter(const string dirPath):sql_type_(SQL_ERROR)//未指定,初始值为-1
{
    filesystem::path database(dirPath);//访问目录
    if(!filesystem::exists(database)){//如果没有此目录
        //创建文件夹
        filesystem::create_directory(database);
    }
    //实例化智能指针api
    #ifndef TEST_SQL
    api = make_unique<DB_API>(DB_API(dirPath));
    #endif
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
	reg = "(\"|\'|\\`)\\s*|\\s*(\"|\'|\\`)";
    sql_statement_ = regex_replace(sql_statement_, reg, " $1$2 ");
    
	//将string转换为vector
	sql_vector_ = SplitSQL(sql_statement_, " ");
    
    //throw SQLTypeError("测试");
}

vector<string> Interpreter::get_gen_SQL(string &statement){
    sql_statement_ = statement;
    GenSQL();
    return sql_vector_;
}
void Interpreter::ExecuteSQL(const string& statement)
{
    //将statement传递给成员变量
    sql_statement_ = statement;
    //将SQL语句转换为小写
    //transform(sql_statement_.begin(), sql_statement_.end(), sql_statement_.begin(), (int (*)(int))tolower);
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
        throw SQLTypeError("SQL Generate Error");
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
            throw SQLTypeError("SQL format error:SQL type not found");
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
                throw SQLTypeError("SQL format error:SQL type not found");
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
                throw SQLTypeError("SQL format error:SQL type not found");
            }
        }else if(op_first == "show"){
            if(op_second == "databases"){
                sql_type_ = SQL_SHOW_DATABASES;
            }
            else if(op_second == "tables"){
                sql_type_ = SQL_SHOW_TABLES;
            }else{
                throw SQLTypeError("SQL format error:SQL type not found");
            }
        }else{
            throw SQLTypeError("SQL format error:SQL type not found");
        }
    }
}

void Interpreter::PraseSQL(){
    switch(sql_type_){
        case SQL_QUIT://退出数据库
        {
            Quit();
            break;
        }
        case SQL_SHOW_DATABASES://展示数据库
        {
        #ifndef TEST_SQL
            api->ShowDatabases();
        #endif
            break;
        }
        case SQL_SHOW_TABLES://展示表
        {
        #ifndef TEST_SQL
            api->ShowTables();
        #endif
            break;
        }
        case SQL_CREATE_DATABASE://创建数据库
        {
            unique_ptr<SQLCreateDatabase> st = make_unique<SQLCreateDatabase>(SQLCreateDatabase(sql_vector_));
        #ifndef TEST_SQL
            api->CreateDatabase(*st);
        #endif
            break;
        }
        case SQL_CREATE_INDEX://新建索引
        {
            unique_ptr<SQLCreateIndex> st = make_unique<SQLCreateIndex>(SQLCreateIndex(sql_vector_));
        #ifndef TEST_SQL
            api->CreateIndex(*st);
        #endif
            break;
        }
        case SQL_CREATE_TABLE://新建表
        {
            unique_ptr<SQLCreateTable> st = make_unique<SQLCreateTable>(SQLCreateTable(sql_vector_));
        #ifndef TEST_SQL
            api->CreateTable(*st);
        #endif
            break;
        }
        case SQL_DROP_TABLE://删除表
        {
            unique_ptr<SQLDropTable> st = make_unique<SQLDropTable>(SQLDropTable(sql_vector_));
        #ifndef TEST_SQL
            api->DropTable(*st);
        #endif
            break;
        }
        case SQL_DROP_INDEX://删除索引
        {
            unique_ptr<SQLDropIndex> st = make_unique<SQLDropIndex>(SQLDropIndex(sql_vector_));
        #ifndef TEST_SQL
            api->DropIndex(*st);
        #endif
            break;
        }
        case SQL_USE://使用数据库
        {
            unique_ptr<SQLUse> st=make_unique<SQLUse>(SQLUse(sql_vector_));
        #ifndef TEST_SQL
            api->Use(*st);
        #endif
            break;
        }
        case SQL_INSERT://插入
        {
            unique_ptr<SQLInsert> st=make_unique<SQLInsert>(SQLInsert(sql_vector_));
        #ifndef TEST_SQL
            api->Insert(*st);
        #endif
            break;
        }
        case SQL_DELETE:
        {
            unique_ptr<SQLDelete> st=make_unique<SQLDelete>(SQLDelete(sql_vector_));
        #ifndef TEST_SQL
            api->Delete(*st);
        #endif
            break;
        }
        case SQL_UPDATE:
        {
            unique_ptr<SQLUpdate> st=make_unique<SQLUpdate>(SQLUpdate(sql_vector_));
        #ifndef TEST_SQL
            api->Update(*st);
        #endif
            break;
        }
        case SQL_SELECT:
        {
            unique_ptr<SQLSelect> st=make_unique<SQLSelect>(SQLSelect(sql_vector_));
        #ifndef TEST_SQL
            api->Select(*st);
        #endif
            break;
        }
        case SQL_ALTER:case SQL_ERROR:default:
            break;
    }
    
}

void Interpreter::Quit(){
#ifndef TEST_SQL
    api->Quit();
#endif
}