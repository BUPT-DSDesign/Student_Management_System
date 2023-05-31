#include "SQLstatement.hpp"
#include "MyException.hpp"
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
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
        throw SQLSyntaxError("SQL CREATE DATABASE SYNTAX ERROR,TOO SHORT");
        return;
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
vector<pair<string, string>> &SQLCreateTable::get_index()
{
    return indexs_;
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
vector<TableColAttribute>& SQLCreateTable::get_attr()
{
    return attrs_;
}
void SQLCreateTable::PraseSQLVector(vector<string> &sql_vector)
{
    /*
    语法
    CREATE TABLE Persons
    (
    id BIGINT NOT NULL COMMENT '主键',
    name varchar(20) NOT NULL COMMENT '群聊名称',
    person_size INT NOT NULL DEFAULT 2 COMMENT '人数',
    owner_id BIGINT NOT NULL COMMENT '群主id',
    create_time DATETIME NULL DEFAULT NULL COMMENT '创建时间',
    update_time DATETIME NULL DEFAULT NULL COMMENT '更新时间',
    PRIMARY KEY (id)
    UNIQUE KEY (name)
    );
    */
    uint16 pos = 2;
    if (sql_vector.size() < 3)
    {
        throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,TOO SHORT");
        return;
    }
    tb_name_ = sql_vector[pos];
    pos++;
    if (sql_vector[pos] != "(")
    {
        throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO (");
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
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,TOO MANY PRIMARY KEY");
                return;
            }
            pos++;
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            if (sql_vector[pos] != "key")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO KEYWORD 'KEY'");
                return;
            }
            pos++;
            if (sql_vector[pos] != "(")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO (");
                return;
            }
            pos++;
            for (auto &v : attrs_)
            {
                //cout<<v.col_name_<<endl;
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
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO KEY NAMED "+sql_vector[pos]);
                return;
            }
            pos++;
            if (sql_vector[pos] != ")")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,BRACKET NOT MATCH");
                return;
            }
        }
        else if(keyword == "unique")
        {
            //TODO 第二种情况,增加唯一索引
            pos++;
            //格式UNIQUE KEY `username` (`username`)
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            if(sql_vector[pos] != "key")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO KEYWORD 'KEY'");
                return;
            }
            pos++;
            //索引名
            string index_name = sql_vector[pos];
            pos++;
            if(sql_vector[pos] != "(")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO (");
                return;
            }
            pos++;
            //索引列名
            string col_name = sql_vector[pos];
            pos++;
            if(sql_vector[pos] != ")")
            {
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,BRACKET NOT MATCH");
                return;
            }
            indexs_.push_back({index_name,col_name});
        }
        else
        {
            // 第三种情况,新增列
            TableColAttribute attr;
            attr.is_primary_ = false;
            attr.is_not_null = false;
            attr.default_length_ = 0;
            // 第一个元素是列名称
            transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
            memset(attr.col_name_, 0, 27);
            if (sql_vector[pos].length() < 26)
            {
                std::copy(sql_vector[pos].begin(), sql_vector[pos].end(), attr.col_name_);
            }
            else
            {
                std::copy(sql_vector[pos].begin(), sql_vector[pos].begin() + 26, attr.col_name_);
            }
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
                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,NO SUCH DATA TYPE:"+sql_vector[pos]+" Col Name:"+sql_vector[pos-1]);
                return;
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
                        throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,BRACKET NOT MATCH");
                        return;
                    }
                    pos++;
                }
            }
            // 检查是否为逗号/右括号,不是则检测关键字,包含有NOT NULL,DEFAULT,COMMENT
            if (sql_vector[pos] != "," && sql_vector[pos] != ")")
            {
                bool is_key = true;
                while (is_key)
                {
                    is_key = false;
                    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
                    if (sql_vector[pos] == "not") // NOT NULL
                    {
                        pos++;
                        transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
                        if (sql_vector[pos] == "null")
                        {
                            attr.is_not_null = true;
                        }
                        else
                        {
                            throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,EXCEPT KEYWORD 'NULL',BUT GET OTHER:"+sql_vector[pos]);
                            return;
                        }
                        pos++;
                    }
                    else if (sql_vector[pos] == "default") // DEFAULT
                    {
                        pos++;
                        if (sql_vector[pos] == "\'")
                        {
                            pos++;
                            char buffer[256] = {0};
                            if (sql_vector[pos] == "\'")
                            {
                                // 数据为空
                                attr.default_length_ = 0;
                            }
                            else
                            {
                                attr.default_length_ = sql_vector[pos].length();
                                attr.default_ = sql_vector[pos];
                                pos++;
                            }
                            if (sql_vector[pos] != "\'")
                            {
                                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,QUOTATION MARK NOT MATCH");
                                return;
                            }
                        }
                        else if (sql_vector[pos] == "\"")
                        {
                            pos++;
                            if (sql_vector[pos] == "\"")
                            {
                                // 数据为空
                                attr.default_length_ = 0;
                            }
                            else
                            {
                                attr.default_length_ = sql_vector[pos].length();
                                attr.default_ = sql_vector[pos];
                                pos++;
                            }
                            if (sql_vector[pos] != "\"")
                            {
                                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,QUOTATION MARK NOT MATCH");
                                return;
                            }
                        }else{
                            //判断是否为数字类型(包括各种int和float,不是则报错)
                            //只有数字类型不需要用单引号或者双引号括起来
                            if((attr.data_type_>>4) != 0x0){
                                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,EXCEPT QUOTATION MARK,BUT GET OTHER:"+sql_vector[pos]);
                                return;
                            }
                            attr.default_length_ = sql_vector[pos].length();
                            //将数字解析为对应类型

                            switch (attr.data_type_){
                                case T_TINY_INT:case T_SMALL_INT:case T_INT:
                                    attr.default_ = stoi(sql_vector[pos]);
                                    break;
                                case T_BIG_INT:
                                    attr.default_ = stol(sql_vector[pos]);
                                    break;
                                case T_FLOAT:
                                    attr.default_ = stof(sql_vector[pos]);
                                    break;
                                case T_DOUBLE:
                                    attr.default_ = stod(sql_vector[pos]);
                                    break;
                                default:
                                    throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,EXCEPT NUMBER,BUT GET OTHER:"+sql_vector[pos]);
                                    return;
                            }
                            pos++;
                        }
                    }
                    else if (sql_vector[pos] == "comment")// COMMENT
                    {
                        pos++;
                        if (sql_vector[pos] == "\'")
                        {
                            pos++;
                            //memset(attr.comment_, 0, 256);
                            if (sql_vector[pos] == "\'")
                            {
                                // 数据为空
                                attr.comment_length_ = 0;
                            }
                            else
                            {
                                attr.comment_length_ = sql_vector[pos].length();
                                attr.comment_ = sql_vector[pos];
                                pos++;
                            }
                            if (sql_vector[pos] != "\'")
                            {
                                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,QUOTATION MARK NOT MATCH");
                                return;
                            }
                        }
                        else if (sql_vector[pos] == "\"")
                        {
                            pos++;
                            if (sql_vector[pos] == "\"")
                            {
                                // 数据为空
                                attr.comment_length_ = 0;
                            }
                            else
                            {
                                attr.comment_length_ = sql_vector[pos].length();
                                attr.comment_ = sql_vector[pos];
                                pos++;
                            }
                            if (sql_vector[pos] != "\"")
                            {
                                throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,QUOTATION MARK NOT MATCH");
                                return;
                            }
                        }
                        else
                        {
                            //COMMENT內容必然是字符串,如果不是,则报错
                            throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,EXCEPT STRING VALUE,BUT GET OTHER:"+sql_vector[pos]);
                            return;
                        }
                    }
                    else
                    {
                        throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,EXCEPT KEYWORD 'NOT NULL' 'COMMENT' 'DEFAULT',BUT GET OTHER:"+sql_vector[pos]);
                        return;
                    }
                    if(sql_vector[pos] != "," && sql_vector[pos] != ")"){
                        //如果不是逗号或者右括号,则继续检测
                        is_key = true;
                    }
                }
            }
            // 将这行新增
            attrs_.push_back(attr);
        }
        if (sql_vector[pos] == ",")
        {
            // 还有
            is_attr = true;
            pos++;
        }
        else if (sql_vector[pos] != ")")
        {
            throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,BRACKET NOT MATCH");
            return;
        }
    }
    // 限制列元素长度
    if (attrs_.size() > 16)
    {

        throw SQLSyntaxError("SQL CREATE TABLE SYNTAX ERROR,TOO MANY COLUMNS");
        return;
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
        throw SQLSyntaxError("SQL CREATE INDEX SYNTAX ERROR,TOO SHORT");
        return;
    }
    index_name_ = sql_vector[pos];
    pos++;
    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
    // 检测ON
    if (sql_vector[pos] != "on")
    {
        throw SQLSyntaxError("SQL CREATE INDEX SYNTAX ERROR,NO KEYWORD 'ON'");
        return;
    }
    pos++;
    transform(sql_vector[pos].begin(), sql_vector[pos].end(), sql_vector[pos].begin(), (int (*)(int))tolower);
    tb_name_ = sql_vector[pos];
    pos++;
    if (sql_vector[pos] != "(" && sql_vector[pos + 2] != ")")
    {
        throw SQLSyntaxError("SQL CREATE INDEX SYNTAX ERROR,BRACKET NOT MATCH");
        return;
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
string SQLInsert::get_tb_name()
{
    return tb_name_;
}
vector<pair<string,string>> SQLInsert::get_values()
{
    //将col_name_和values_组合成一个pair
    vector<pair<string,string>> values;
    for(int i=0;i<col_name_.size();i++){
        values.push_back(make_pair(col_name_[i],values_[i]));
    }
    return values;
}


void SQLInsert::PraseSQLVector(vector<string> &sql_vector)
{
    // SQL Insert 的语法如下
    // INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
    // 或者
    // INSERT INTO table_name VALUES (值1, 值2,....)
    // 值可以是字符串或者数字
    // 如果值为数字,可以不用引号括起来

    if (sql_vector.size() < 4)
    {
        throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,TOO SHORT");
        return;
    }
    auto it = sql_vector.begin() + 1;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "into")
    {
        throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,NO KEYWORD 'INTO'");
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
            // 列名固定小写,而且列名不需要用引号括起来
            transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            col_name_.push_back(*it);
            it++;
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
                throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,EXCEPT ',' OR ')', BUT GET OTHER:"+*it);
                return;
            }
        }
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "values")
    {
        throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,NO KEYWORD 'VALUES'");
        return;
    }
    it++;
    // 接下来解析values中的值
    if (*it != "(")
    {
        throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,NO '('");
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
                throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,QUOTATION MARK NOT MATCH");
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
                throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,QUOTATION MARK NOT MATCH");
                return;
            }
        }
        else
        {
            //只有数字类型不需要用单引号或者双引号括起来
            values_.push_back(*it);
            it++;
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
            throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,EXCEPT ',' OR ')', BUT GET OTHER:"+*it);
            return;
        }
    }
    if (col_name_.size() != 0 && col_name_.size() != values_.size())
    {
        // 设置的行和值不配对，报错
        throw SQLSyntaxError("SQL INSERT SYNTAX ERROR,NUMBER OF COLUMNS AND VALUES NOT MATCH");
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
string SQLDelete::get_tb_name()
{
    return tb_name_;
}
SQLWhere &SQLDelete::get_condition()
{
    return condition_;
}

void SQLDelete::PraseSQLVector(vector<string> &sql_vector)
{
    // 将诸如下列的SQL语句解析到私有成员变量中
    // DELETE FROM table_name WHERE [condition];

    if (sql_vector.size() < 3)
    {
        throw SQLSyntaxError("SQL DELETE SYNTAX ERROR,TOO SHORT");
        return;
    }
    auto it = sql_vector.begin() + 1;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "from")
    {
        throw SQLSyntaxError("SQL DELETE SYNTAX ERROR, EXCEPT KEYWORD 'FROM', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    tb_name_ = *it;
    it++;
    if (it != sql_vector.end())
    {
        // 证明有WHERE条件,解析
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        if (*it != "where")
        {
            throw SQLSyntaxError("SQL DELETE SYNTAX ERROR, EXCEPT KEYWORD 'WHERE', BUT GET OTHER:"+*it);
            return;
        }
        // 将WHERE条件解析到condition_中
        condition_.PraseSQLVector(vector<string>(it, sql_vector.end()));
    }
}

/*******************SQLUpdate**********************************/
SQLUpdate::SQLUpdate(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLUpdate::get_tb_name()
{
    return tb_name_;
}
vector<pair<string,string>> SQLUpdate::get_values()
{
    vector<pair<string,string>> res;
    for(int i=0;i<col_name_.size();i++){
        res.push_back(make_pair(col_name_[i],values_[i]));
    }
    return res;
}
SQLWhere &SQLUpdate::get_condition()
{
    return condition_;
}

void SQLUpdate::PraseSQLVector(vector<string> &sql_vector)
{
    // 将诸如下列的SQL语句切分成的sql_vector解析到类的私有成员变量中
    // UPDATE table_name SET column1 = value1, column2 = value2...., columnN = valueN WHERE [condition];
    // WHERE的condition部分示例如下
    // WHERE column_name operator value
    // operator可以是=,>,<,>=,<=,<>
    // value可以是数字或者字符串
    // 如果value是字符串,则需要用单引号或者双引号括起来
    // 如果value是数字,则不需要用单引号或者双引号括起来
    // 如果value是字符串,则需要用单引号或者双引号括起来

    if (sql_vector.size() < 5)
    {
        throw SQLSyntaxError("SQL UPDATE SYNTAX ERROR,TOO SHORT");
        return;
    }
    auto it = sql_vector.begin() + 1;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    tb_name_ = *it;
    it++;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "set")
    {
        throw SQLSyntaxError("SQL UPDATE SYNTAX ERROR, EXCEPT KEYWORD 'SET', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    bool is_attr = true;
    while (is_attr)
    {
        is_attr = false;
        // 对应列名
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        col_name_.push_back(*it);
        // 接下来是运算符处理
        it++;
        if (*it != "=")
        {
            throw SQLSyntaxError("SQL UPDATE SYNTAX ERROR, EXCEPT OPERATOR '=', BUT GET OTHER:"+*it);
            return;
        }
        // 接下来是值
        it++;
        values_.push_back(*it);
        it++;
        if (it != sql_vector.end())
        {
            transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            if (*it == ",")
            {
                is_attr = true;
            }
            else if (*it == "where")
            {
                break;
            }
            else
            {
                throw SQLSyntaxError("SQL UPDATE SYNTAX ERROR, EXCEPT ',' OR 'WHERE', BUT GET OTHER:"+*it);
                return;
            }
        }
    }
    if (col_name_.size() != values_.size())
    {
        throw SQLSyntaxError("SQL UPDATE SYNTAX ERROR, NUMBER OF COLUMNS AND VALUES NOT MATCH");
        return;
    }
    if (*it == "where")
    {
        it++;
        //将WHERE条件解析到condition_中
        //从it开始解析
        vector<string> condition_vector;
        while (it != sql_vector.end())
        {
            condition_vector.push_back(*it);
            it++;
        }
        condition_.PraseSQLVector(condition_vector);
    }
}
/*******************SQLSelect**********************************/
SQLSelect::SQLSelect(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLSelect::get_tb_name()
{
    return tb_name_;
}
vector<string> &SQLSelect::get_col_name()
{
    return col_name_;
}
SQLWhere &SQLSelect::get_condition()
{
    return condition_;
}


void SQLSelect::PraseSQLVector(vector<string> &sql_vector)
{
    // SQL Select 语句标准如下
    // SELECT column_name,column_name FROM table_name WHERE column_name operator value;
    // 其中operator可以是=,>,<,>=,<=,<>
    // value可以是数字或者字符串
    // 如果value是字符串,则需要用单引号或者双引号括起来
    // 如果value是数字,则不需要用单引号或者双引号括起来

    if (sql_vector.size() < 4)
    {
        throw SQLSyntaxError("SQL SELECT SYNTAX ERROR,TOO SHORT");
        return;
    }
    auto it = sql_vector.begin() + 1;
    if (*it != "*")
    {
        // 查询的不是全体,是特定列
        bool is_attr = true;
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        while (is_attr)
        {
            is_attr = false;
            transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
            col_name_.push_back(*it);
            it++;
            if (*it == ",")
            {
                // 如果还有
                is_attr = true;
            }
        }
    }
    else
    {
        it++;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "from")
    {
        throw SQLSyntaxError("SQL SELECT SYNTAX ERROR, EXCEPT KEYWORD 'FROM', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    // 表名
    tb_name_ = *it;
    it++;
    if (it != sql_vector.end())
    {
        // 如果没到结尾,说明有where条件
        transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
        if (*it != "where")
        {
            throw SQLSyntaxError("SQL SELECT SYNTAX ERROR, EXCEPT KEYWORD 'WHERE', BUT GET OTHER:"+*it);
            return;
        }
        // 将WHERE条件解析到condition_中
        condition_.PraseSQLVector(vector<string>(it, sql_vector.end()));
    }
}

/***************SQLDropDatabase**********/
SQLDropDatabase::SQLDropDatabase(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLDropDatabase::get_db_name()
{
    return db_name_;
}
void SQLDropDatabase::PraseSQLVector(vector<string> &sql_vector)
{
    // 常见SQL语法
    // DROP DATABASE name
    auto it = sql_vector.begin() + 1;
    if (sql_vector.size() < 3)
    {
        throw SQLSyntaxError("SQL DROP DATABASE SYNTAX ERROR,TOO SHORT");
        return;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "database")
    {
        throw SQLSyntaxError("SQL DROP DATABASE SYNTAX ERROR,EXCEPT KEYWORD 'DATABASE', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    db_name_ = *it;
}

/***************SQLDropIndex**********/
SQLDropIndex::SQLDropIndex(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLDropIndex::get_index_name()
{
    return index_name_;
}
void SQLDropIndex::PraseSQLVector(vector<string> &sql_vector)
{
    // 常见SQL语法
    // DROP INDEX index_name
    auto it = sql_vector.begin() + 1;
    if (sql_vector.size() < 3)
    {
        throw SQLSyntaxError("SQL DROP INDEX SYNTAX ERROR,TOO SHORT");
        return;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "index")
    {
        throw SQLSyntaxError("SQL DROP INDEX SYNTAX ERROR,EXCEPT KEYWORD 'INDEX', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    index_name_ = *it;
}

/***************SQLDropTable***************/
SQLDropTable::SQLDropTable(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLDropTable::get_tb_name()
{
    return tb_name_;
}
void SQLDropTable::PraseSQLVector(vector<string> &sql_vector)
{
    // 常见SQL语法
    // DROP TABLE table_name
    auto it = sql_vector.begin() + 1;
    if (sql_vector.size() < 3)
    {
        throw SQLSyntaxError("SQL DROP TABLE SYNTAX ERROR,TOO SHORT");
        return;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    if (*it != "table")
    {
        throw SQLSyntaxError("SQL DROP TABLE SYNTAX ERROR,EXCEPT KEYWORD 'TABLE', BUT GET OTHER:"+*it);
        return;
    }
    it++;
    tb_name_ = *it;
}

/********SQLUse*********/
SQLUse::SQLUse(vector<string> &sql_vector)
{
    PraseSQLVector(sql_vector);
}
string SQLUse::get_db_name()
{
    return db_name_;
}
void SQLUse::PraseSQLVector(vector<string> &sql_vector)
{
    // 常见SQL语法
    // USE database_name
    auto it = sql_vector.begin() + 1;
    if (sql_vector.size() < 2)
    {
        throw SQLSyntaxError("SQL USE SYNTAX ERROR,TOO SHORT");
        return;
    }
    transform((*it).begin(), (*it).end(), (*it).begin(), (int (*)(int))tolower);
    db_name_ = *it;
}

