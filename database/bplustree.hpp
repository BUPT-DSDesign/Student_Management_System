#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include "datatype.hpp"
#include "table.hpp"
//B树有关的定义
using namespace std;
//B树节点
struct BPlusTreeNode{
    streampos self_pos;//自身的位置
    streampos parent_pos;//父亲的位置
    streampos prev_pos;//前一页位置
    streampos next_pos;//后一页位置
    bool is_leaf;//是否为叶子节点
    uint8 elem_count;//元素的个数
};
class BPlusTree
{
private:
    /* data */
    streampos root_pos_;//根节点位置
    size_t file_size_;//表大小
    unique_ptr<BPlusTreeNode> cur_;//B树指针,指向当前被加载的页
    unique_ptr<byte> cache_;//缓存,用于读取字节流,byte为Cpp17新增类型
    unique_ptr<Table> table_;//表对象
public:
    BPlusTree(Table &table);
    //以下为增删改查
    void Add(int key,byte &data);//新增元素
    string_view Delete(int key);//利用键值删除元素,返回被删除元素的json字符串
    void Update(int key,byte& data);//更改元素
    string_view Search(int key);//利用键值搜索元素,返回被删除元素的json字符串
    ~BPlusTree();
};

