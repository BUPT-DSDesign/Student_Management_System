#pragma once

#include "datatype.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_map>
using namespace std;
/*
** 一条稍复杂的Where语句示例如下
** WHERE (col1 = 1 AND col2 = 2) OR (col3 = 3 AND col4 = 4) OR (col5 = 5 AND col6 = 6)
** 其中,我们可以将 col1 = 1看成一个子结构,子结构由AND/OR运算符连接起来,形成一个从句
** 从句之间也可以用AND和OR连接起来,形成一个复杂的Where语句
*/

//WhereTerm,用来存储一个子结构
//子结构的形式为:列名 运算符 立即数
class WhereTerm{
public:
    WhereTerm(uint16 col_id,uint8 eOperator,any eValue);
private:
    enum class op{
        OP_EQ = 0,//=
        OP_LT = 1,//<
        OP_LE = 2,//<=
        OP_GT = 3,//>
        OP_GE = 4,//>=
        OP_NE = 5,//<>
    };
    uint16 col_id_;//子结构的列id
    op eOperator_;//子结构的运算符,包含=,>,<,>=,<=,<>
    any eValue_;//子结构的立即数
};

//WhereClause,用来存储一个从句
//从句的形式为:子结构1 AND/OR 子结构2 AND/OR 子结构3 AND/OR ...
class WhereClause{
public:
    WhereClause();
    void AddTerm(WhereTerm &term);
private:
    vector<WhereTerm> terms_;//从句中的子结构
    uint8 eOperator_;//连接各个子结构的运算符,包含AND/OR
    unique_ptr<WhereClause> outerClause_;//外层从句,如(col1 = 1 AND col2 = 2) OR (col3 = 3 AND col4 = 4),其中(col1 = 1 AND col2 = 2)和(col3 = 3 AND col4 = 4)的外层从句为整个句子
};
//Where约束
class SQLWhere
{
public:
    SQLWhere();
    void PraseSQLVector(vector<string> &sql_vector);
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