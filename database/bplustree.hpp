#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include "datatype.hpp"
#define PAGE_SIZE 16*1024 //参考INNODB
using namespace std;


struct BPNodeHead{
    bool is_leaf_;//判断是不是叶子结点
    bool is_dirty_;//判断是否为脏页(即该页数据是否已经被删除)
    uint8 key_type_;//键的类型
   // uint16 key_pos_;//键相对于原数据的偏移量
    uint16 degree_;//阶数/孩子的最大数量
    uint16 busy_;// 当前的孩子数量
    uint16 data_size_;//每个元素的大小
    streampos father_;//父节点的位置
    streampos chunk_pos_;//区块偏移位置
};
class BPTree;
class BPNode
{
private:
    string_view file_name_;//文件名
    //streampos chunk_pos_;//当前节点位置
    //shared_ptr<fstream> file_head_;//文件读写头
    BPNodeHead head_;//节点头
    vector<streampos> child_;//孩子的位置
    vector<byte> data_;//数据
    streampos node_pos;//当前节点的位置
    friend class BPTree;
public:
    BPNode();
    std::vector<std::streampos>::iterator childBegin();//返回child的开始位置的迭代器
    std::vector<std::streampos>::iterator childEnd();//返回child结束位置的迭代器
    std::vector<std::streampos>::iterator childLoc(int id);//返回指向child id的迭代器
    std::vector<std::byte>::iterator dataBegin();//返回data开始位置的迭代器
    std::vector<std::byte>::iterator dataEnd();//返回data的真实结束位置的迭代器
    std::vector<std::byte>::iterator dataLoc(int id);//返回指向id的迭代器
    void ReadChunk(streampos pos);//TODO 读取节点,将区块信息写入
    void CreateChunk(bool is_leaf,int data_size);//TODO 新建节点
    bool isLeaf();//判断是否为叶子结点
    uint16 getElemCount();//获取节点的元素个数
    uint64 getKey(int id);//TODO 获取第k个元素的key
    streampos getChild(int id);//获取第k个孩子
    vector<byte> getRawData(int id);//TODO 获取第k个元素的字节流数据
    void WriteChunk();//将节点写入
    //streampos getElemLocation(int id);//获取节点的真实位置
    uint16 getElemLocInData(int id);//获取节点在data中的开始下标
    
    void insertDataAtPos(int id,const uint64 &key,const vector<byte>& data);
};
class BPTree
{
private:
    /* data */
    bool is_table_;//是table还是索引
    streampos root_pos_;//根节点位置
    streampos cur_;//当前读取的位置
    uint16 size_of_item;//每一个元素的大小
    BPNode bufnode_;//当前读取的叶子节点
    
    //找到叶子节点,并将数据载入节点
    void searchLeaf(const uint64 &key);
    //二分查找键值,返回结果的对应下标
    int binarySearch(BPNode &node,const uint64 &key);
    //叶子结点满了之后分裂
    void splitTreeNode(const uint64 &key,vector<byte> &data);
    //无分裂的插入
    void insertNoSplit(BPNode &node,const uint64 &key,vector<byte> &data);
    //分裂后将节点向上传递
    void insertKey(const uint64 &key,const streampos &old,const streampos &after);
    //将节点的父亲全部更新
    void resetIndexChildrenParent(BPNode &node);
public:
    //以下为打开
    //打开一个已有的B+树文件
    BPTree(string path);
    //析构函数
    ~BPTree();
    //以下为BPlusTree的操作
    //默认键值可以由unsigned long long存储
    //用键值寻找单个元素,返回值为字节流
    vector<byte> Search(const uint64 &key);
    //利用键值,按范围寻找元素,返回值为字节流
    vector<byte> SearchRange(const uint64& left,const uint64& right);
    //读取最开头的叶节点所对应区块(当查找条件不为键值时)
    void ReadFirstChunk();
    //读取上一个叶节点所对应区块
    void ReadPrevChunk();
    //读取下一个叶节点所对应区块
    void ReadNextChunk();
    //返回当前叶节点的所有元素
    vector<vector<byte>> GetAllElem();
    //删除某个元素
    bool Remove(const uint64 &key);
    //插入元素
    bool Insert(const uint64 &key,vector<byte> &data);
    //更新元素
    bool Update(const uint64 &key,vector<byte> &data);
    
};