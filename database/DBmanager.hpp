#pragma once
#include "datatype.hpp"
#include "table.hpp"
#include <string>
#include <map>
using namespace std;

//DBmanager类用于管理数据库,包括新建数据库,删除数据库,打开数据库,关闭数据库
//数据库类会用来管理表,包括新建表,删除表
//同时,增删改查的操作也会涉及,这里的成员函数负责调用对应的表的成员函数
class DBmanager
{
private:
    string db_name_;//数据库名
    string path_;//数据库路径
    map<string,unique_ptr<Table>> tb_list_;//表列表,键为表名,值为表
    vector<string> tb_name_list_;//表名列表
public:
    DBmanager();//默认构造函数
    DBmanager(const string& path);//设置数据库所在根目录
    void UseDatabase(const string& db_name);//使用数据库
    void CreateDatabase(const string& db_name);//新建数据库
    void DropDatabase(const string& db_name);//删除数据库
    void CloseDatabase();//关闭数据库
    void ShowTables();//展示数据库中的表
    void CreateTable(const string& tb_name,vector<TableColAttribute> &col_info);//新建表
    void DropTable(const string& tb_name);//删除表
    void CreateIndex(const string& tb_name,const string& col_name,const string& index_name);//新建索引
    void DropIndex(const string& index_name);//删除索引

};