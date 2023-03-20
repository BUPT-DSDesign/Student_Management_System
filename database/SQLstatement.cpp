#include "SQLstatement.hpp"
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//创建数据库相关
SQLCreateDatabase::SQLCreateDatabase(vector<string> &sql_vector)
{
    
    PraseSQLVector(sql_vector);
}
string SQLCreateDatabase::get_db_name(){
    return db_name_;
}
void SQLCreateDatabase::PraseSQLVector(vector<string> &sql_vector){
    //常见SQL语法
    //CREATE DATABASE name
    if (sql_vector.size() < 3){
        //TODO:处理错误
    }else{
        db_name_ = sql_vector[2];
    }
}
//创建表相关
SQLCreateTable::SQLCreateTable(vector<string> &sql_vector){
    set_type2uint();//设置映射表
    PraseSQLVector(sql_vector);
}
void SQLCreateTable::set_type2uint(){
    //字符串转标记符
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
string SQLCreateTable::get_tb_name(){
    return tb_name_;
}
vector<TableColAttribute> SQLCreateTable::get_attr(){
    return attrs_;
}
void SQLCreateTable::PraseSQLVector(vector<string> &sql_vector){
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
    if (sql_vector.size() < 3){
        //TODO:Error
        return;
    }
    tb_name_ = sql_vector[pos];
    pos++;
    if(sql_vector[pos]!="("){
        //TODO:Format Error
        return;
    }
    pos++;
    bool is_attr = true;//判断当前读取的是否为属性
    bool has_primary_key = false;
    while(is_attr){
        is_attr = false;
        string keyword = sql_vector[pos];
        transform(keyword.begin(),keyword.end(),keyword.begin(),(int (*)(int))tolower);
        //第一种情况,这是在设置主键
        if(keyword == "primary"){
            if(has_primary_key){
                //TODO:已经有主键了,报错
            }
            pos++;
            transform(sql_vector[pos].begin(),sql_vector[pos].end(),sql_vector[pos].begin(),(int (*)(int))tolower);
            if(sql_vector[pos] != "key"){
                //TODO:ERROR OCCUR
            }
            pos++;
            if(sql_vector[pos]!="("){
                //TODO:ERROR OCCUR
            }
            pos++;
            for(auto &v:attrs_){
                if(v.col_name_ == sql_vector[pos]){
                    //找到主键,设置
                    v.is_primary_ = true;
                    v.is_not_null = true;
                    has_primary_key = true;
                }
            }
            if(!has_primary_key){
                //TODO:找不到主键报错
            }
            pos++;
            if(sql_vector[pos]!=")"){
                //TODO:ERROR OCCUR
            }
        }else{
            //第二种情况,新增列
            TableColAttribute attr;
            attr.is_primary_ = false;
            attr.is_not_null = false;
            //第一个元素是列名称
            attr.col_name_ = sql_vector[pos];
            pos++;
            //第二个元素是列数据类型
            transform(sql_vector[pos].begin(),sql_vector[pos].end(),sql_vector[pos].begin(),(int (*)(int))tolower);
            if(type2uint.find(sql_vector[pos]) != type2uint.end()){
                //存在这样的数据类型
                uint8 data_type = type2uint[sql_vector[pos]];
                attr.data_type_ = data_type;
                attr.length_ = SIZE_OF_T[data_type];
            }else{
                //TODO: 错误处理
            }
            keyword = sql_vector[pos];
            pos++;
            if(keyword=="char"||keyword=="varchar"){
                //可变长度类型,查看是否有设置长度(没有则最大)
                if(sql_vector[pos]=="("){
                    //有设置长度
                    pos++;
                    uint16 data_size = stoi(sql_vector[pos]);
                    if(data_size < SIZE_OF_T[attr.data_type_]){
                        attr.length_ = data_size;
                    }
                    pos++;
                    if(sql_vector[pos]!=")"){
                        //TODO:报错
                    }
                    pos++;
                }
            }
            //检查是否为逗号/右括号,不是则检测是否为not null关键字
            if(sql_vector[pos]!=","&&sql_vector[pos]!=")"){
                transform(sql_vector[pos].begin(),sql_vector[pos].end(),sql_vector[pos].begin(),(int (*)(int))tolower);
                if(sql_vector[pos]=="not"){
                    pos++;
                    transform(sql_vector[pos].begin(),sql_vector[pos].end(),sql_vector[pos].begin(),(int (*)(int))tolower);
                    if(sql_vector[pos]=="null"){
                        attr.is_not_null = true;
                    }else{
                        //TODO:报错
                    }
                    pos++;
                }else{
                    //TODO:报错
                }
            }
            //将这行新增
            attrs_.push_back(attr);
        }
        if(sql_vector[pos]==","){
            //还有
            is_attr = true;
        }else if(sql_vector[pos]!=")"){
            //TODO:报错
        }
    }

}

//创建索引相关
SQLCreateIndex::SQLCreateIndex(vector<string> &sql_vector){
    PraseSQLVector(sql_vector);
}

void SQLCreateIndex::PraseSQLVector(vector<string> &sql_vector){
    //常见SQL语法(仅支持单一索引)
    //CREATE INDEX index_name ON table_name (col_name_)
}