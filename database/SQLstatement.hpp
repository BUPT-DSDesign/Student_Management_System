#pragma once
#include "datatype.hpp"
#include <string>
#include <vector>
using namespace std;
//创建数据库需要传入的参数
//创建数据库
class SQLCreateDatabase
{
public:
    SQLCreateDatabase(string name);
    string get_db_name();
    ~SQLCreateDatabase();
private:
    string db_name_;//数据库的名字
};
//每一列的属性
class ColAttribute
{
public:
    ColAttribute();
    ~ColAttribute();
private:
    uint8 data_type_;//数据类型
    bool is_primary_;//是否为主键
    uint16 length_;//该数据类型长度
    string col_name_;//列名     
};

//创建表需要的参数
class SQLCreateTable
{

private:
    string tb_name_;//表的名字
    vector<ColAttribute> attrs_;//表的属性(即每一列的属性)
};

//创建主键需要的参数
class SQLCreateIndex
{
    
private:
    string tb_name_;//表的名字
    string col_name_;//列的名字
    string index_name_;//索引名
};

//删除数据库需要的参数
class SQLDropDatabase
{
private:
    string db_name_;//数据库名字
};

//删除某张表索引所需要的参数
class SQLDropIndex
{
private:
    string index_name_;//索引名
};

//删除表
class SQLDropTable{
private:
    string tb_name_;//表名
};

//链接数据库
class SQLUse
{
private:
    string db_name_;//数据库名称
};

//SQL插入的对象
class SQLInsertValue
{
private:
    uint8 data_type_;//数据类型
    string val_;//值
};

//增加
class SQLInsert
{
private:
    string tb_name;//表名
    vector<SQLInsertValue> values_;//待新增的数据
};

//Where约束
class SQLWhere
{
private:
	uint8 op_type_;//Where约束的运算符类型
	string key_;//对应键值
	string value_;//约束值
};

//删除操作
class SQLDelete
{
private:
    string tb_name_;//表名
    vector<SQLWhere> values_;//待删除的数据限定信息
};
class SQLUpdate
{
    
};

class SQLSelect
{
    /* data */
};



