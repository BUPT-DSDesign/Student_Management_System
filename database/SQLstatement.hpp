#pragma once
#include "datatype.hpp"
#include "table.hpp"
#include <string>
#include <vector>
#include <map>
using namespace std;
//创建数据库需要传入的参数
//创建数据库
class SQLBase{
public:
    virtual void PraseSQLVector(vector<string> &sql_vector) = 0;
};
class SQLCreateDatabase: public SQLBase
{
public:
    SQLCreateDatabase(vector<string> &sql_vector);
    string get_db_name();
    ~SQLCreateDatabase(){}
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string db_name_;//数据库的名字
};
//创建表需要的参数
class SQLCreateTable: public SQLBase
{
public:
    SQLCreateTable(vector<string> &sql_vector);
    string get_tb_name();
    vector<TableColAttribute> get_attr();
    void set_type2uint();
    ~SQLCreateTable(){}
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name_;//表的名字
    vector<TableColAttribute> attrs_;//表的属性(即每一列的属性)
    map<string_view,uint8> type2uint;//从数据类型到uint标记的映射
};

//创建主键需要的参数
class SQLCreateIndex: public SQLBase
{
public:
    SQLCreateIndex(vector<string> &sql_vector);
    string get_tb_name();
    string get_col_name();
    string get_index_name();
    ~SQLCreateIndex(){}
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name_;//表的名字
    string col_name_;//列的名字
    string index_name_;//索引名
};

//删除数据库需要的参数
class SQLDropDatabase: public SQLBase
{
public:
    SQLDropDatabase(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string db_name_;//数据库名字
};

//删除某张表索引所需要的参数
class SQLDropIndex: public SQLBase
{
public:
    SQLDropIndex(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string index_name_;//索引名
};

//删除表
class SQLDropTable: public SQLBase
{
public:
    SQLDropTable(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name_;//表名
};

//链接数据库
class SQLUse: public SQLBase
{
public:
    SQLUse(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string db_name_;//数据库名称
};

//SQL插入的对象
class SQLInsertValue: public SQLBase
{
public:
    SQLInsertValue(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    uint8 data_type_;//数据类型
    string val_;//值
};

//增加
class SQLInsert: public SQLBase
{
public:
    SQLInsert(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name;//表名
    vector<SQLInsertValue> values_;//待新增的数据
};

//Where约束
class SQLWhere: public SQLBase
{
public:
    SQLWhere(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
	uint8 op_type_;//Where约束的运算符类型
	string key_;//对应键值
	string value_;//约束值
};

//删除操作
class SQLDelete: public SQLBase
{
public:
    SQLDelete(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name_;//表名
    vector<SQLWhere> values_;//待删除的数据限定信息
};
class SQLUpdate: public SQLBase
{
public:
    SQLUpdate(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    
};

class SQLSelect: public SQLBase
{
public:
    SQLSelect(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:

};



