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
class BPTree
{
private:
    /* data */
    unique_ptr<fstream> table_;//表文件
    streampos root_pos_;//根节点位置
    vector<TableColAttribute> col_info_;//表的信息
    uint16 size_of_item;//每一个元素的大小
public:
    //打开已经存在的表文件
    BPTree(string tbname);
    //新建不存在的表文件
    BPTree(string tbname,vector<TableColAttribute> &col_info);
    //析构函数
    ~BPTree();
    //打印设置信息
    void PrintAttr();
};

