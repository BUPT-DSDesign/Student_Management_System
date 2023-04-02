#include "datatype.hpp"
#include "bplustree.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
using namespace std;
bool BPNode::isLeaf(){
    return head_.is_leaf_;
}
uint16 BPNode::getElemCount(){
    return head_.busy_;
}
streampos BPNode::getChild(int id){
    if(id < child_.size())
        return child_[id];
    return -1;
}
void BPNode::ReadChunk(streampos pos){
    
}
void BPNode::CreateChunk(bool is_leaf,int data_size)
{
    head_.is_leaf_ = is_leaf;
    head_.data_size_ = data_size;
}

BPTree::BPTree(string tbname)
{
    string_view table_path = tbname + ".db";
    table_ = make_shared<fstream>(table_path.data(),ios::binary|ios::out|ios::in);
    if (!table_->good()) {
        cout << "Table does not exist." << endl;
        exit(1);
    }
    //读取列数量
    int col_num = 0;
    table_->read((char*)&col_num,sizeof(col_num));
    cout<<"有"<<col_num<<"列\n";
    //(*table_item_)>>col_num;
    //开始读取表属性
    TableColAttribute buf_atr;
    for(int i=0;i<col_num;i++){
        table_->read((char*)&buf_atr,sizeof(buf_atr));
        col_info_.push_back(buf_atr);
    }
    //记录B+树根节点位置
    root_pos_ = table_->tellg();
}
BPTree::BPTree(string tbname,vector<TableColAttribute> &col_info)
{
    string_view table_path = tbname + ".db";
    table_ = (make_shared<fstream>(table_path.data(),ios::binary|ios::out|ios::in));
    //写入列长度
    int col_num = col_info.size();
    table_->write((char*)&col_num,sizeof(col_num));
    //写入每一列的属性
    for(auto &t:col_info){
        table_->write((char*)&t,sizeof(t));
        col_info_.push_back(t);
    }
    //记录B+树根节点位置
    root_pos_ = table_->tellg();
}
BPTree::~BPTree(){
    table_->close();
}
void BPTree::PrintAttr(){
    for(auto &t:col_info_){
        cout<<t.col_name_<<","
            <<(int)t.data_type_<<","
            <<t.is_not_null<<","
            <<t.is_primary_<<","
            <<t.length_<<"\n";
    }
}
string BPTree::Search(const uint64 &key){
    //搜索
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    while(cur_!=-1){
        //第一步,利用cur_来载入一个块
        bufnode_.ReadChunk(cur_);
        if(bufnode_.isLeaf()){
            //如果是叶子节点了,就开始找值
            break;
        }
        //否则开始迭代叶子结点
        //每个区块里的元素都是从小到大排列
        int child_id = bufnode_.getElemCount();
        for(int i=0;i<bufnode_.getElemCount();i++){
            uint64 node_key = bufnode_.getKey(i);
            if(node_key > key)//此时,第i个孩子即为对应的节点
            {
                child_id = i;
                break;
            }
                
        }
        cur_ = bufnode_.getChild(child_id);
    }
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    
}