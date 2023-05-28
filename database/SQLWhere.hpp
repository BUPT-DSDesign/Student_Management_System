#pragma once

#include "datatype.hpp"

#include <string>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <memory>
#include <any>
using namespace std;
//下面代码为将Where语句解析到AST的代码
//表达式节点
class ExpressionNode{
public:
    virtual ~ExpressionNode() = default;
    virtual string toString() const = 0;
};
/*
** 一条稍复杂的Where语句示例如下
** WHERE (col1 = 1 AND col2 = 2) OR (col3 = 3 AND col4 = 4) OR (col5 = 5 AND col6 = 6)
** 其中,我们可以将 col1 = 1看成一个子结构,子结构由AND/OR运算符连接起来,形成一个从句
** 从句之间也可以用AND和OR连接起来,形成一个复杂的Where语句
*/
enum class TermOperator{
    EQUAL,
    NOT_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL
};
enum class ClauseOperator{
    NOP,
    AND,
    OR
};

//WhereTerm,用来存储一个子结构
//子结构的形式为:列名 运算符 立即数
class WhereTerm{
public:
    WhereTerm(const string& col_name,const string& eOperator,const string& value);
    string getColName() const;
    TermOperator getOperator() const;
    string getValue() const;
    bool isNegated() const;
    void setNegated(bool negated);
private:
    string col_name_;//子结构的列名
    TermOperator eOperator_;//子结构的运算符,包含=,>,<,>=,<=,<>
    string eValue_;//子结构的立即数
    bool negated_;//是否取反
};

//WhereClause,用来存储一个从句
//从句的形式为:子结构1 AND/OR 子结构2 AND/OR 子结构3 AND/OR ...
class WhereClause{
public:
    WhereClause();//默认构造函数
    WhereClause(vector<WhereTerm> term,const string& op,shared_ptr<WhereClause> outerClause = nullptr);
    shared_ptr<WhereClause> getOuterClause() const;
    ClauseOperator getOperator() const;
    vector<WhereTerm> getTerms() const;
    void setOuterClause(shared_ptr<WhereClause> outerClause);
    void setOperator(const string& op);
    void addTerm(const WhereTerm& term);
    void setNegated(bool negated);
private:
    bool negated_;//看是不是取反
    vector<WhereTerm> terms_;//从句中的子结构
    ClauseOperator op_;//连接各个子结构的运算符,包含AND/OR
    shared_ptr<WhereClause> outerClause_;//外层从句,如(col1 = 1 AND col2 = 2) OR (col3 = 3 AND col4 = 4),其中(col1 = 1 AND col2 = 2)和(col3 = 3 AND col4 = 4)的外层从句为整个句子
};
//Where约束
enum class QueryType{
    NOP,
    QUERY_EQ,//=
};
class SQLWhere
{
public:
    SQLWhere();
    string GetBestIndex(vector<string> &col_name_list,const string primary);
    void PraseSQLVector(vector<string>::iterator & it);
    shared_ptr<WhereClause> PraseWhereClause(vector<string>::iterator &it,shared_ptr<WhereClause> parent=nullptr);
    WhereTerm PraseWhereTerm(vector<string>::iterator &it);
    uint64 GetQueryKey(string index_name);
    uint64 GetQueryLeftKey(string index_name);
    uint64 GetQueryRightKey(string index_name);
    QueryType GetQueryType(string index_name);
    bool Filter(any value,string index_name);
    
private:
    //最顶层的表达式
    shared_ptr<WhereClause> rootClause_;
    //存储表达式中的列名
    set<string> col_name_set_;
};