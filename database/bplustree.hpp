#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "datatype.hpp"
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
    unique_ptr<ifstream> table_;//表对象,用于打开文件
public:
    BPlusTree();
    ~BPlusTree();
};

