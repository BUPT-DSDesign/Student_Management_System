#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include "datatype.hpp"
#include "table.hpp"
#define PAGE_SIZE 16*1024 //参考INNODB
using namespace std;
struct TableColAttribute
{
    uint8 data_type_;//数据类型
    bool is_primary_;//是否为主键
    bool is_not_null;//是否必须存在
    uint16 length_;//该数据类型长度(比特)
    char col_name_[24];//列名     
};
struct BPNodeHead{
    bool is_leaf_;//判断是不是叶子结点
    uint8 key_type_;//键的类型
    uint16 key_pos_;//键相对于原数据的偏移量
    uint16 degree_;//阶数/孩子的最大数量
    uint16 busy_;// 当前的孩子数量
    uint16 data_size_;//每个元素的大小
    streampos father_;//父节点的位置
};
class BPTree;
class BPNode
{
private:
    BPNodeHead head_;//节点头
    vector<streampos> child_;//孩子的数量
    vector<byte> data_;//数据
public:
    BPNode();
    void ReadChunk(streampos pos);//读取节点
    void CreateChunk(bool is_leaf,int data_size);//新建节点
    bool isLeaf();//判断是否为叶子结点
    uint16 getElemCount();//获取节点的元素个数
    uint64 getKey(int id);//获取第k个元素的key
    streampos getChild(int id);//获取第k个孩子
};
class BPTree
{
private:
    /* data */
    shared_ptr<fstream> table_;//表文件
    streampos root_pos_;//根节点位置
    streampos cur_;//当前读取的位置
    vector<TableColAttribute> col_info_;//表的信息
    uint16 size_of_item;//每一个元素的大小
    //反序列化数据为json
    BPNode bufnode_;//当前读取的叶子节点
    string deserialize(vector<byte> &data);
    friend class BPNode;
public:
    //以下为打开表文件的构造函数
    //打开已经存在的表文件
    BPTree(string tbname);
    //新建不存在的表文件
    BPTree(string tbname,vector<TableColAttribute> &col_info);
    //析构函数
    ~BPTree();
    //以下为BPlusTree的操作
    //默认键值可以由unsigned long long存储
    //用键值寻找单个元素,返回值为json
    string Search(const uint64 &key);
    //按范围寻找元素,返回值为json
    string SearchRange(const uint64& left,const uint64& right);
    //删除某个元素
    bool Remove(const uint64 &key);
    //插入元素
    bool Insert(const uint64 &key,vector<byte> &data);
    //更新元素
    bool Update(const uint64 &key,vector<byte> &data);
    //序列化数据(json转换为字节流)
    vector<byte> Serialize(string rawJson);
    //打印设置信息
    void PrintAttr();
};

