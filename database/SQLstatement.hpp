#pragma once
#include "datatype.hpp"
#include "table.hpp"
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
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
    vector<TableColAttribute>& get_attr();
    void set_type2uint();
    ~SQLCreateTable(){}
    void PraseSQLVector(vector<string> &sql_vector);
private:
    string tb_name_;//表的名字
    vector<TableColAttribute> attrs_;//表的属性(即每一列的属性)
    map<string_view,uint8> type2uint;//从数据类型到uint标记的映射
};

//创建索引需要的参数
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
    string get_db_name();
private:
    string db_name_;//数据库名字
};

//删除某张表索引所需要的参数
class SQLDropIndex: public SQLBase
{
public:
    SQLDropIndex(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_index_name();
private:
    string index_name_;//索引名
};

//删除表
class SQLDropTable: public SQLBase
{
public:
    SQLDropTable(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_tb_name();
private:
    string tb_name_;//表名
};

//链接数据库
class SQLUse: public SQLBase
{
public:
    SQLUse(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_db_name();
private:
    string db_name_;//数据库名称
};

//SQL插入的对象
/*
class SQLInsertValue: public SQLBase
{
public:
    SQLInsertValue(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
private:
    uint8 data_type_;//数据类型
    string val_;//值
};
*/
//增加
class SQLInsert: public SQLBase
{
public:
    SQLInsert(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_tb_name();
    vector<string>& get_col_name();
    vector<string>& get_values();
private:
    string tb_name_;//表名
    vector<string> col_name_;//对应的列名
    vector<string> values_;//需要新增的数据
    //vector<SQLInsertValue> values_;//待新增的数据
};

//Where约束



class SQLWhere
{
public:
    SQLWhere();
    void PraseSQLVector(vector<string> &sql_vector);
    bool CalculateWhere(vector<byte> &data,vector<TableColAttribute> &col_attr);//计算某条数据是否满足where条件
private:
    //WHERE运算符优先级表
    unordered_map<string,int> priority_ = {
        {"(",0},
        {"OR",1},
        {"AND",2},
        {"NOT",3},
        {"=",4},
        {"<>",4},
        {"<",4},
        {">",4},
        {"<=",4},
        {">=",4},
        {"LIKE",4},
        {"BETWEEN",4},
        {"IN",4},
        {"IS",4},
        {"EXISTS",4}
    };
    enum SQLWhereOperandType{
        IMMEDIATE = 0,//立即数
        OPERATOR = 1,//运算符
        COLUMN = 2,//列名
        SUB_QUERY = 3//子查询
    };
    enum ImmType{
        INT = 0,
        FLOAT = 1,
        STRING = 2,
        BOOL = 3,
        NULL_ = 4
    };
    //记录简单的运算符取反
    unordered_map<string,string> reverse_operator_ = {
        {"=",">="},
        {"<>","<"},
        {"<=",">"},
        {">=","<"},
        {"<",">="},
        {">","<="}
    };
    //Where算式的操作数
    struct SQLWhereOperand{
        uint8 type_;//操作数类型,0为立即数,1为运算符,2为列名,3为子查询
        string value_;//操作数的值,此处若type为0则是立即数,为1则是运算符,为2,3则是为列名
        uint8 val_type_;//范围的类型,0为整数,1为浮点数,2为字符串,3为布尔值,4为NULL
        any lrange,rrange;//如果为3,操作数的范围,用于B+树的范围查询
    };
    //某一列数据的限制范围,调用B+树的范围查询时使用
    map<string,pair<any,any>> col_range_;
    //逆波兰表达式操作数栈
    stack<SQLWhereOperand> operend_stack_;
    //逆波兰表达式
    vector<SQLWhereOperand> rpn_;

};

//删除操作
class SQLDelete: public SQLBase
{
public:
    SQLDelete(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_tb_name();
    SQLWhere& get_condition();
private:
    string tb_name_;//表名
    SQLWhere condition_;//查询条件
};
//SQL的更新操作
class SQLUpdate: public SQLBase
{
public:
    SQLUpdate(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_tb_name();
    vector<string>& get_col_name();
    vector<string>& get_values();
    SQLWhere& get_condition();
private:
    string tb_name_;//表名
    vector<string> col_name_;//对应的列名
    vector<string> values_;//需要更新的数据
    SQLWhere condition_;//查询条件
};

class SQLSelect: public SQLBase
{
public:
    SQLSelect(vector<string> &sql_vector);
    void PraseSQLVector(vector<string> &sql_vector);
    string get_tb_name();
    vector<string>& get_col_name();
    SQLWhere& get_condition();
private:
    string tb_name_;//表名
    vector<string> col_name_;//对应的列名
    SQLWhere condition_;//查询条件
};



