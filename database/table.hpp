#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "datatype.hpp"
using namespace std;
//表的列属性
struct TableColAttribute
{
    uint8 data_type_;//数据类型
    bool is_primary_;//是否为主键
    uint16 length_;//该数据类型长度(比特)
    string col_name_;//列名     
};
//定义表结构
class Table
{
private:
    /* data */
    unique_ptr<ifstream> fp_;//操作表的文件指针
    vector<TableColAttribute> col_info_;//表的信息
    vector<uint16> col_shift_;//列位置偏移量
public:
    Table(const string& filename);//打开一张已有的表
    Table(const string& filename,vector<TableColAttribute> &col_info);//新建一张表
     
    ~Table();
};
