#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include "datatype.hpp"
#include "value.hpp"
#define PAGE_SIZE 16*1024 //参考INNODB
using namespace std;


//设计思路:
//B+树的每一个节点都是一个区块,区块的大小为16KB
//其中,节点分为内部节点和叶子节点,内部节点存储的是键值和孩子的位置,叶子节点存储的是键值和数据的位置
//每一个区块的头部都有一个头部信息,用于存储节点的信息,包括是否为叶子节点,是否为脏页,键的类型,孩子的最大数量,当前孩子数量,每个元素的大小,父节点的位置,区块偏移位置
//如果为内部节点，则节点内部(除头部)数据存储结构如下
//child0|key1|child1|key2|child2|...|keyn|childn|
//如果为叶子结点，则节点内部(除头部)数据存储结构如下
//front|key1|data1|key2|data2|...|keyn|datan|tail|
//叶子节点为双向链表,front和tail为指向前后叶子节点的指针
//同时假设单个表文件不会大于4GB,所以可以用uint32存储位置信息(100w+条数据是不是不大可能)
struct BPNodeHead{
    bool is_leaf_;//判断是不是叶子结点
    bool is_dirty_;//判断是否为脏页(即该页数据是否已经被删除)
    uint8 key_type_;//键的类型
    uint16 key_size_;//键的大小
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
    bool is_leaf_;//判断是不是叶子结点
    bool is_dirty_;//判断是否为脏页(即该页数据是否已经被删除)
    uint8 key_type_;//键的类型
    uint16 degree_;//阶数/孩子的最大数量
    uint16 busy_;// 当前的元素数量
    uint16 child_cnt_;//当前孩子数量
    uint16 data_size_;//每个元素的大小
    uint16 key_size_;//键的大小
    streampos father_;//父节点的位置
    //shared_ptr<fstream> file_head_;//文件读写头
    //BPNodeHead head_;//节点头
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
    void ReadChunk(streampos pos);//读取节点,将区块信息写入
    void CreateChunk(bool is_leaf,int data_size,uint16 key_size,uint8 key_type);//TODO 新建节点
    bool isLeaf();//判断是否为叶子结点
    uint16 getElemCount();//获取节点的元素个数
    Key getKey(int id);//获取第k个元素的key
    streampos getChild(int id);//获取第k个孩子
    vector<byte> getRawData(int id);//获取第k个元素的字节流数据
    void setKey(int id,const Key &key);//设置第k个元素的key
    void setElem(int id,const vector<byte> &data);//设置第k个元素的data
    void WriteChunk();//将节点写入
    streampos releaseChunk();//释放节点
    //streampos getElemLocation(int id);//获取节点的真实位置
    uint16 getElemLocInData(int id);//获取节点在data中的开始下标
    
    void insertDataAtPos(int id,const Key &key,const vector<byte>& data);
};
class BPTree
{
private:
    /* data */
    bool is_table_;//是table还是索引
    streampos root_pos_;//根节点位置
    streampos cur_;//当前读取的位置
    uint16 size_of_item_;//每一个元素的大小
    BPNode bufnode_;//当前读取的叶子节点
    uint8 key_type_;//键的类型
    //找到叶子节点,并将数据载入节点
    void searchLeaf(const Key &key);
    //二分查找键值,返回结果的对应下标
    int binarySearch(BPNode &node,const Key &key);
    //叶子结点满了之后分裂
    void splitTreeNode(const Key &key,vector<byte> &data);
    //无分裂的插入
    void insertNoSplit(BPNode &node,const Key &key,const vector<byte> &data);
    //分裂后将节点向上传递
    void insertKey(const Key &key,const streampos &old,const streampos &after);
    //将节点的父亲全部更新
    void resetIndexChildrenParent(BPNode &node);
    //删除相关
    //简单将一个叶子结点的数据删除
    void simpleLeafRemove(BPNode &node,int pos);
    //将一个内部节点的数据删除
    void simpleInnerRemove(BPNode &node,int pos);
    //从左兄弟借一个元素
    void leafShiftFromLeft(BPNode &leaf,BPNode &left_sibling,BPNode &parent,int parent_pos);
    void innerShiftFromLeft(BPNode &leaf,BPNode &left_sibling,BPNode &parent,int parent_pos);
    //把当前叶子结点和左兄弟合并
    void leafMergeToLeft(BPNode &leaf,BPNode &left_sibling,int remove_pos);
    void innerMergeToLeft(BPNode &leaf,BPNode &left_sibling,BPNode &parent,int parent_pos,int remove_pos);
    //从右兄弟借一个元素
    void leafShiftFromRight(BPNode &leaf,BPNode &right_sibling,BPNode &parent,int parent_pos);
    void innerShiftFromRight(BPNode &leaf,BPNode &right_sibling,BPNode &parent,int parent_pos);
    //把右兄弟合并到当前叶子结点
    void leafMergeFromRight(BPNode &leaf,BPNode &right_sibling);
    void innerMergeFromRight(BPNode &leaf,BPNode &right_sibling,BPNode &parent,int parent_pos);
    //删除叶子节点
    void deleteLeafNode(BPNode &leaf,BPNode &left_sibling,BPNode &right_sibling);
    //删除内部节点
    void innerNodeRemove(BPNode &node,int pos);
    //更新子节点的父亲
    void subNodeUpdateParent(BPNode &node,streampos parent_pos);
public:
    //以下为打开
    //打开一个已有的B+树文件
    BPTree(string path);
    //新建一个B+树文件
    BPTree(string path,bool is_table,int data_size,uint16 key_size,uint8 key_type);
    //析构函数
    ~BPTree();
    //以下为BPlusTree的操作
    //默认键值可以由unsigned long long存储
    //用键值寻找单个元素,返回值为字节流
    vector<byte> Search(const Key &key);
    //用键值寻找单个元素,返回值为数据的位置
    streampos SearchPos(const Key &key);
    //利用键值,按范围寻找元素,返回值为字节流
    vector<vector<byte>> SearchRange(const Key& left,const Key& right);
    //读取指定的块到bufnode中
    void ReadChunk(streampos pos);
    //读取最开头的叶节点所对应区块(当查找条件不为键值时)
    void ReadFirstChunk();
    //读取上一个叶节点所对应区块
    void ReadPrevChunk();
    //读取下一个叶节点所对应区块
    void ReadNextChunk();
    //判断当前节点是否为叶节点/是否读取成功
    bool isBufLeaf();
    //返回当前叶节点的所有元素
    vector<vector<byte>> GetAllElemInChunk();
    //返回整个树的所有元素
    vector<vector<byte>> GetAllElemInTree();
    //删除单个元素,并输出删除的对应元素
    vector<byte> Remove(const Key &key);
    //插入元素
    bool Insert(const Key &key,vector<byte> &data);
    //更新元素
    bool Update(const Key &key,vector<byte> &data);
    //查询键值类型
    uint8 getKeyType();
};