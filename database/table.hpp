#pragma once
#include <vector>
#include <iostream>
#include <memory>
#include <string>
#include <any>
#include <map>
#include "datatype.hpp"
#include "bplustree.hpp"
#include "SQLWhere.hpp"
using namespace std;
//表每一列的属性,最多有16列
typedef struct 
{
    uint8 data_type_;//数据类型
    bool is_primary_;//是否为主键
    bool is_not_null;//是否必须存在
    uint16 length_;//该数据类型长度(字节),最大为65535
    char col_name_[27];//列名,最长为27个字符
    //char comment_[256];//注释,最长为256个字符
    uint16 default_length_;//默认值长度,0表示没有默认值
    uint16 comment_length_;//注释长度,0表示没有注释
    any default_;//默认值,最长为65535个字符
    string_view comment_;//注释,最长为2048个字符     
}TableColAttribute;
//定义表结构
//表采用InnoDB的结构,即每一条记录都是定长的,每一条记录都有一个row_id,用于索引,且使用B+树为表基本存储结构
//row_id是一个64位的无符号整数,每一条记录都有一个row_id,且不可更改
//如果指定了主键,则主键为row_id,否则自动生成一个row_id
//Table类用于操作表,包括新建表,删除表,插入记录,删除记录,修改记录,查询记录
//表的文件结构如下
//表头(文件头) 16KB 用于存储表的信息,包括列名,列的属性,索引名,索引的位置

typedef struct{
    uint16 col_id_;//索引所对应的列
    char index_name_[30];//索引文件名,最长为30个字符
}IndexAttribute;

//行,表示一条记录
class Row{
public:
    Row(vector<TableColAttribute>& col_info,vector<byte> data);//用于构造一条记录
    bool isSatisfied(const SQLWhere& where) const;//判断是否满足where条件
    string getRowJSON() const;//获取一条记录的JSON格式
    ColValue getValue(string col_name) const;//获取一条记录的某一列的值
private:
    vector<ColValue> col_value_;//记录的每一列的值
};


//表
//这个类用于操作表,包括新建表,删除表,插入记录,删除记录,修改记录,查询记录
//同时还包括索引的操作,包括新建索引,删除索引,查询索引
class Table
{
private:
    friend class BPTree;//BPTree需要访问Table的私有成员,完成字节流解析和写入
    /* data */
    string primary_key_;//主键
    vector<TableColAttribute> col_info_;//表每一行的信息,最多有16列
    vector<uint16> col_shift_;//列位置偏移量
    uint16 col_cnt_;//列的数量
    uint16 record_length_;//每一条记录的长度
    uint16 index_cnt_;//索引的数量
    string table_name_;//表名
    string db_path_;//数据库路径
    vector<string> index_name_;//这个表相关的索引名,大小与col_info_.size()相同
    vector<string> index_col_name_;//这个表相关的索引所对应的列名,大小与col_info_.size()相同    
    unique_ptr<BPTree> tb_data_;//表的数据,因其仿造的是InnoDB,所以使用B+树
    map<string,unique_ptr<BPTree>> tb_index_;//表的索引,使用map存储,键为索引名,值为B+树
    map<string,string> col2index_;//列名到索引名的映射
    map<string,int> col2id_;//列名到列id的映射
    string deserialize(vector<byte> &data);//反序列化单条数据为json
    vector<byte> serialize(vector<pair<string,string>> &col_item);//序列化数据
    void PrintToStream(string msg,vector<Row> result);//输出结果到标准输入输出流
    void saySuccess();//输出成功信息
    any getValue(vector<byte> &data,uint16 col_id);//获取某一列的值
    vector<byte> getValueInBytes(vector<byte> &data,uint16 col_id);//获取某一列的值
public:
    //默认构造函数
    Table();
    //拷贝构造函数
    Table(const Table& tb);
    //打开一张已有的表
    Table(const string& db_path,const string& table_name);
    //新建一张表
    Table(const string& db_path,const string& table_name,vector<TableColAttribute> &col_info);
    //删除一张表
    void DropTable();
    //新建索引
    void CreateIndex(const string& col_name,const string& index_name);
    //下面为处理CRUD的相关函数定义
    //插入记录
    void InsertRecord(vector<pair<string,string>> &col_item);
    //删除记录
    void DeleteRecord(SQLWhere &where);
    //修改记录,通过解析Where的数据,更改符合条件的记录
    void UpdateRecord(vector<pair<string,string>> &col_item,SQLWhere &where);
    //查询记录,通过解析Where的数据返回符合条件的记录
    void SelectRecord(SQLWhere &where);
    
    ~Table();
};
