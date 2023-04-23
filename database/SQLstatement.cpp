#include "SQLstatement.hpp"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
/*******************SQLCreateDatabase**********************************/
// 创建数据库相关
SQLCreateDatabase::SQLCreateDatabase(vector<string> &sql_vector)
{

    PraseSQLVector(sql_vector);
}
string SQLCreateDatabase::get_db_name()
{
    return db_name_;
}
void SQLCreateDatabase::PraseSQLVector(vector<string> &sql_vector)
{
    // 常见SQL语法
    // CREATE DATABASE name
    if (sql_vector.size() < 3)
    {
        // TODO:处理错误
    }
    else
    {
        db_name_ = sql_vector[2];
    }
}
/*******************SQLCreateTable**********************************/
SQLCreateTable::SQLCreateTable(vector<string> &sql_vector)
{
    set_type2uint(); // 设置映射表
    PraseSQLVector(sql_vector);
}
void SQLCreateTable::set_type2uint()
{
    // 字符串转标记符
    type2uint["bigint"] = T_BIG_INT;
    type2uint["char"] = T_CHAR;
    type2uint["date"] = T_DATE;
    type2uint["datetime"] = T_DATE_TIME;
    type2uint["double"] = T_DOUBLE;
    type2uint["float"] = T_FLOAT;
    type2uint["int"] = T_INT;
    type2uint["smallint"] = T_SMALL_INT;
    type2uint["time"] = T_TIME;
    type2uint["timestamp"] = T_TIMESTAMP;
    type2uint["tinyint"] = T_TINY_INT;
    type2uint["varchar"] = T_VARCHAR;
    type2uint["year"] = T_YEAR;
}
string SQLCreateTable::get_tb_name()
{
    return tb_name_;
}
vector<TableColAttribute> SQLCreateTable::get_attr()
{
    return attrs_;
}
void SQLCreateTable::PraseSQLVector(vector<string> &sql_vector)
{
    /*
    语法
    CREATE TABLE Persons
    (
    Id_P int NOT NULL,
    LastName varchar(255) NOT NULL,
    FirstName varchar(255),
    Address varchar(255),
    City varchar(255),
    PRIMARY KEY (Id_P)
    );
    */
    uint16 pos = 2;
    if (sql_vector.size() < 3)
    {
        // TODO:Error
        return;
    }
    tb_name_ = sql_vector[pos];
    pos++;
    if (sql_vector[pos] != "(")
    {
        // TODO:Format Error
        return;
    }
    pos++;
    bool is_attr = true; // 判断当前读取的是否为属性
    bool has_primary_key = false;
    while (is_attr)
    {
        is_attr = false;
        string keyword = sql_vector[pos];
        transform(keyword.begin(), keyword.end(), keyword.begin(), (int (*)(int))tolower);
        // 第一种情况,这是在设置主键
        if (keyword == "primary")
        {
            if (has_primary_key)
            {
                // TODO:已经有主键了,报错
            }
            pos++;
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            if (sql_vector[pos] != "key")
            {
                // TODO:ERROR OCCUR
            }
            pos++;
            if (sql_vector[pos] != "(")
            {
                // TODO:ERROR OCCUR
            }
            pos++;
            for (auto &v : attrs_)
            {
                if (v.col_name_ == sql_vector[pos])
                {
                    // 找到主键,设置
                    v.is_primary_ = true;
                    v.is_not_null = true;
                    has_primary_key = true;
                }
            }
            if (!has_primary_key)
            {
                // TODO:找不到主键报错
            }
            pos++;
            if (sql_vector[pos] != ")")
            {
                // TODO:ERROR OCCUR
            }
        }
        else
        {
            // 第二种情况,新增列
            TableColAttribute attr;
            attr.is_primary_ = false;
            attr.is_not_null = false;
            // 第一个元素是列名称
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            attr.col_name_ = sql_vector[pos];
            pos++;
            // 第二个元素是列数据类型
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            if (type2uint.find(sql_vector[pos]) != type2uint.end())
            {
                // 存在这样的数据类型
                uint8 data_type = type2uint[sql_vector[pos]];
                attr.data_type_ = data_type;
                attr.length_ = SIZE_OF_T[data_type];
            }
            else
            {
                // TODO: 错误处理
            }
            keyword = sql_vector[pos];
            pos++;
            if (keyword == "char" || keyword == "varchar")
            {
                // 可变长度类型,查看是否有设置长度(没有则最大)
                if (sql_vector[pos] == "(")
                {
                    // 有设置长度
                    pos++;
                    uint16 data_size = stoi(sql_vector[pos]);
                    if (data_size < SIZE_OF_T[attr.data_type_])
                    {
                        attr.length_ = data_size;
                    }
                    pos++;
                    if (sql_vector[pos] != ")")
                    {
                        // TODO:报错
                    }
                    pos++;
                }
            }
            // 检查是否为逗号/右括号,不是则检测是否为not null关键字
            if (sql_vector[pos] != "," && sql_vector[pos] != ")")
            {
                transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
                if (sql_vector[pos] == "not")
                {
                    pos++;
                    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
                    if (sql_vector[pos] == "null")
                    {
                        attr.is_not_null = true;
                    }
                    else
                    {
                        // TODO:报错
                    }
                    pos++;
                }
                else
                {
                    // TODO:报错
                }
            }
            // 将这行新增
            attrs_.push_back(attr);
        }
        if (sql_vector[pos] == ",")
        {
            // 还有
            is_attr = true;
        }
        else if (sql_vector[pos] != ")")
        {
            // TODO:报错
        }
    }
    // 限制列元素长度
    if (attrs_.size() > 15)
    {
        // TODO:过长报错
    }
}

/*******************SQLCreateIndex**********************************/
SQLCreateIndex::SQLCreateIndex(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLCreateIndex::get_tb_name() { return tb_name_; }
string SQLCreateIndex::get_col_name() { return col_name_; }
string SQLCreateIndex::get_index_name() { return index_name_; }
void SQLCreateIndex::PraseSQLVector(vector<string> &sql_vector)
{
    // CREATE INDEX index_name ON table_name (col_name_)
    uint16 pos = 2;
    if (sql_vector.size() < 8)
    {
        // TODO: ERROR
        return;
    }
    index_name_ = sql_vector[pos];
    pos++;
    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
    // 检测ON
    if (sql_vector[pos] != "on")
    {
        // TODO:Error
    }
    pos++;
    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
    tb_name_ = sql_vector[pos];
    pos++;
    if (sql_vector[pos] != "(" && sql_vector[pos + 2] != ")")
    {
        // TODO:Throw error
    }
    pos++;
    col_name_ = sql_vector[pos];
}
/*******************SQLCreateIndex**********************************/

/*******************SQLInsert**********************************/
SQLInsert::SQLInsert(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
void SQLInsert::PraseSQLVector(vector<string> &sql_vector)
{
    auto it = sql_vector.begin() + 1;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "into")
    {
        // TODO FORMAT ERROR
        return;
    }
    it++;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    tb_name_ = *it;
    it++;
    bool is_attr = true;
    if (*it == "(")
    {
        // 如果指定了添加的列
        it++;
        while (is_attr)
        {
            is_attr = false;
            if (*it == "\'")
            {
                it++;
                if (*it == "\'")
                {
                    // 数据为空
                    col_name_.push_back("");
                }
                else
                {
                    //列名固定小写
                    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
                    col_name_.push_back(*it);
                    it++;
                }
                if (*it != "\'")
                {
                    // TODO FORMAT ERROR
                    return;
                }
            }
            else if (*it == "\"")
            {
                it++;
                if (*it == "\"")
                {
                    // 数据为空
                    col_name_.push_back("");
                }
                else
                {
                    col_name_.push_back(*it);
                    it++;
                }
                if (*it != "\"")
                {
                    // TODO FORMAT ERROR
                    return;
                }
            }
            else
            {
                // TODO FORMAT ERROR
                return;
            }
            if (*it == ",")
            {
                is_attr = true;
            }
            else if (*it == ")")
            {
                break;
            }
            else
            {
                // TODO FORMAT ERROR
            }
        }
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "values")
    {
        // TODO FORMAT ERROR
        return;
    }
    it++;
    // 接下来解析values中的值
    if (*it != "(")
    {
        // TODO FORMAT ERROR
        return;
    }
    it++;
    is_attr = true;
    while (is_attr)
    {
        is_attr = false;
        if (*it == "\'")
        {
            it++;
            if (*it == "\'")
            {
                // 数据为空
                values_.push_back("");
            }
            else
            {
                values_.push_back(*it);
                it++;
            }
            if (*it != "\'")
            {
                // TODO FORMAT ERROR
                return;
            }
        }
        else if (*it == "\"")
        {
            it++;
            if (*it == "\"")
            {
                // 数据为空
                values_.push_back("");
            }
            else
            {
                values_.push_back(*it);
                it++;
            }
            if (*it != "\"")
            {
                // TODO FORMAT ERROR
                return;
            }
        }
        else
        {
            // TODO FORMAT ERROR
            return;
        }
        if (*it == ",")
        {
            is_attr = true;
        }
        else if (*it == ")")
        {
            break;
        }
        else
        {
            // TODO FORMAT ERROR
        }
    }
    if (col_name_.size() != 0 && col_name_.size() != values_.size())
    {
        // 设置的行和值不配对，报错
        // TODO FORMAT ERROR
        return;
    }
    return;
}

/*******************SQLInsertValue**********************************/
/*
SQLInsertValue::SQLInsertValue(vector<string> &sql_vector){

}
void SQLInsertValue::PraseSQLVector(vector<string> &sql_vector){

}
*/

/*******************SQLDelete**********************************/
SQLDelete::SQLDelete(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
void SQLDelete::PraseSQLVector(vector<string> &sql_vector)
{
}

/*******************SQLUpdate**********************************/
SQLUpdate::SQLUpdate(vector<string> &sql_vector)
{
}
void SQLUpdate::PraseSQLVector(vector<string> &sql_vector)
{
}
/*******************SQLSelect**********************************/
SQLSelect::SQLSelect(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
void SQLSelect::PraseSQLVector(vector<string> &sql_vector)
{
    auto it = sql_vector.begin() + 1;
    if(*it != "*"){
        //查询的不是全体,是特定列
        bool is_attr = true;
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        while(is_attr){
            is_attr = false;
            transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            col_name_.push_back(*it);
            it++;
            if(*it == ","){
                //如果还有
                is_attr = true;
            }
        }
    }else{
        it++;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if(*it != "from"){
        //TODO FORMAT ERROR
        return;
    }
    it++;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    //表名
    tb_name_ = *it;
    it++;
    if(it != sql_vector.end()){
        //如果没到结尾,说明有where条件
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        if(*it != "where"){
            //TODO FORMAT ERROR
            return;
        }
        it++;
        bool is_attr = true;
        while(is_attr){
            is_attr = false;
            //对应列名
            SQLWhere sq;
            transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            //判断有没有NOT运算符
            if(*it == "not"){
                sq.is_need_ = false;
            }else{
                sq.is_need_ = true;
                it++;
                transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            }
            sq.key_ = *it;
            //接下来是运算符处理
            it++;
            if(*it == "="){
                sq.op_type_ = OP_EQUAL;
                it++;
            }else if(*it == ">"){
                it++;
                if(*it == "="){
                    sq.op_type_ = OP_GREATER_EQUAL;
                    it++;
                }else{
                    sq.op_type_ = OP_GREATER;
                }
            }else if(*it == "<"){
                it++;
                if(*it == "="){
                    sq.op_type_ = OP_LESS_EQUAL;
                    it++;
                }else if(*it == ">"){
                    sq.op_type_ = OP_NOT_EQUAL;
                    it++;
                }else{  
                    sq.op_type_ = OP_LESS;
                }
            }else{
                //TODO FORMAT ERROR
                return;
            }
            //接下来是值
            sq.value_ = *it;
            condition_.push_back(sq);
            it++;
            if(it!=sql_vector.end()){
                transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
                if(*it == "and"){
                    relation_.push_back(OP_AND);
                }else if(*it == "or"){
                    relation_.push_back(OP_OR);
                }else{
                    //TODO FORMAT ERROR
                    return;
                }
                is_attr = true;
            }
        }
    }
}