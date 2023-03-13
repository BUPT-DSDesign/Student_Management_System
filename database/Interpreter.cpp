#include <filesystem>
#include <string_view>
#include <regex>
#include <string>
#include <vector>
#include <ranges>
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
    //TODO:随后要将这个值传入给API
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
    //随后开始生成SQL_vector
    GenSQL();
    //TODO:还有后续的保存部分
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