#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include "datatype.hpp"
using namespace std;
//表每一列的属性,最多有16列
typedef struct 
{
    uint8 data_type_;//数据类型
    bool is_primary_;//是否为主键
    bool is_not_null;//是否必须存在
    uint16 length_;//该数据类型长度(比特)
    char col_name_[27];//列名,最长为27个字符     
}TableColAttribute;
//定义表结构
//表采用InnoDB的结构,即每一条记录都是定长的,每一条记录都有一个row_id,用于索引
//row_id是一个64位的无符号整数,每一条记录都有一个row_id,且不可更改
//如果指定了主键,则主键为row_id,否则自动生成一个row_id
//Table类用于操作表,包括新建表,删除表,插入记录,删除记录,修改记录,查询记录
//表的文件结构如下
//表头(文件头) 16KB 用于存储表的信息,包括表名,列名,列的属性,索引名,索引的位置

class Table
{
private:
    /* data */
    unique_ptr<ifstream> table_start_;//操作表的文件指针,打开文件后将表头储存在这
    vector<TableColAttribute> col_info_;//表每一行的信息,最多有16列
    vector<uint16> col_shift_;//列位置偏移量
    uint16 record_length_;//每一条记录的长度
    string table_name_;//表名
    vector<string> index_name_;//这个表相关的索引名,大小与col_info_.size()相同


public:
    Table(const string& filename);//打开一张已有的表
    Table(const string& filename,vector<TableColAttribute> &col_info);//新建一张表
    int get_record_length();//获取每一条记录长度
    void set_record_length();//设置
    
    ~Table();
};
