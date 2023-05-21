#pragma once

#include "datatype.hpp"
#include "table.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
using namespace std;

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