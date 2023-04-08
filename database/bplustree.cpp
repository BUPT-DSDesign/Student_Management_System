#include "datatype.hpp"
#include "bplustree.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
using namespace std;
BPNode::BPNode(){
    //初始化vector大小
    int max_space = PAGE_SIZE - sizeof(BPNodeHead);//最大的空闲空间
    //假设每个data最小为1字节
    int max_degree = (max_space-sizeof(streampos))/(sizeof(streampos)+1)+1;
    child_ = vector<streampos>(max_degree);
    data_ = vector<byte>(max_degree);
}
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
    //TODO 将一个区块读取到字节流中
    //1.打开文件读写流
    fstream fp(file_name_.data(),ios::binary|ios::in);
    fp.seekg(pos,ios::beg);
    if(!fp.eof()){
        //如果没有读写到文件末尾
        //第一个部分,结构体头
        fp.read((char*)&head_,sizeof(head_));
        //第二个部分,读取节点数据
        //整体结构为 指针0 | 数据1 | 指针1 | ... | 数据n | 指针n 
        data_.clear();
        child_.clear();
        streampos child_p;
        for(int i=0;i<head_.busy_;i++){
            fp.read((char*)&child_p,sizeof(child_p));
            child_.push_back(child_p);
            fp.read((char*)&data_[data_.size()],head_.data_size_);
        }
        fp.read((char*)&child_p,sizeof(child_p));
        child_.push_back(child_p);
    }
    //读取完毕,关闭文件读写流
    fp.close();
}
void BPNode::CreateChunk(bool is_leaf,int data_size)
{
    head_.is_leaf_ = is_leaf;
    head_.data_size_ = data_size;
}

BPTree::BPTree(string tbname)
{
    string_view table_path = tbname + ".db";
    bufnode_.file_name_ = table_path;
    fstream fp(table_path.data(),ios::binary|ios::out|ios::in);
    if (!fp.good()) {
        cout << "Table does not exist." << endl;
        exit(1);
    }
    //读取列数量
    int col_num = 0;
    fp.read((char*)&col_num,sizeof(col_num));
    cout<<"有"<<col_num<<"列\n";
    //(*table_item_)>>col_num;
    //开始读取表属性
    TableColAttribute buf_atr;
    for(int i=0;i<col_num;i++){
        fp.read((char*)&buf_atr,sizeof(buf_atr));
        bufnode_.col_info_.push_back(buf_atr);
    }
    //记录B+树根节点位置
    root_pos_ = fp.tellg();
    fp.close();
}
BPTree::BPTree(string tbname,vector<TableColAttribute> &col_info)
{
    string_view table_path = tbname + ".db";
    bufnode_.file_name_ = table_path;
    fstream fp(table_path.data(),ios::binary|ios::out|ios::in);
    //写入列长度
    int col_num = col_info.size();
    fp.write((char*)&col_num,sizeof(col_num));
    //写入每一列的属性
    for(auto &t:col_info){
        fp.write((char*)&t,sizeof(t));
        bufnode_.col_info_.push_back(t);
    }
    //记录B+树根节点位置
    root_pos_ = fp.tellg();
    fp.close();
}
BPTree::~BPTree(){
    
}
void BPTree::PrintAttr(){
    for(auto &t:bufnode_.col_info_){
        cout<<t.col_name_<<","
            <<(int)t.data_type_<<","
            <<t.is_not_null<<","
            <<t.is_primary_<<","
            <<t.length_<<"\n";
    }
}
int BPTree::binary_search(const uint64 &key){
    int l=-1,r=bufnode_.getElemCount() - 1;
    while(l + 1 < r){
        int mid = (l+r) >> 1;
        uint64 node_key = bufnode_.getKey(mid);
        if(key >= node_key){
            r = mid;
        }else{
            l = mid;
        }
    }
    return r;
}
void BPTree::search_leaf(const uint64 &key){
    while(cur_!=-1){
        //第一步,利用cur_来载入一个块
        bufnode_.ReadChunk(cur_);
        if(bufnode_.isLeaf()){
            //如果是叶子节点了,就开始找值
            break;
        }
        //否则开始迭代叶子结点
        //每个区块里的元素都是从小到大排列
        int child_id = binary_search(key);
        cur_ = bufnode_.getChild(child_id);
    }
}
string BPTree::Search(const uint64 &key){
    //搜索(有索引的)
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    search_leaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binary_search(key);
    //第三步,读取对应数据,以JSON格式返回
    vector<byte> elem = bufnode_.getRawData(pos);
    return deserialize(elem);
    
}
bool BPTree::Insert(const uint64 &key,vector<byte> &data){
    cur_ = root_pos_;
    //先搜索这个叶子节点
    search_leaf(key);
    //接着判断键值是否重复
    int pos = binary_search(key);
    if(is_table_&&key == bufnode_.getKey(pos)){
        //键值重复,无法插入(表的主键不能重复)
        return false;
    }
    if(bufnode_.head_.busy_==bufnode_.head_.degree_){
        //如果节点满了,需要分裂
        splitNode(key,data);
        //把旧的根节点作为新的根节点的孩子
    }else{
        //如果节点没有满,直接添加
        insertNoSplit(key,data);
    }
}
void BPTree::splitNode(const uint64 &key,vector<byte> &data){
    //首先需要一分为二,故先创建一个邻居
    BPNode new_node;
    new_node.CreateChunk(true,PAGE_SIZE);
    //找到二分点
    //在分裂为两个叶子结点之后，左节点包含前m/2个记录，右节点包含剩下的记录
    //将第m/2+1个记录的关键字进位到父节点中(下标m/2)
    uint16 pos = bufnode_.head_.busy_/2;
    //比较函数
    bool place_right = false;
    if(key > bufnode_.getKey(pos)){
        //key应该放在右边
        place_right = true;
        pos++;
    }
    //TODO 分裂,将左节点的从pos到end全拷贝到new_node里
    // TODO 写出计算好节点准确位置的函数
    new_node.head_.busy_ = bufnode_.head_.busy_ - pos;
    //然后减小左节点规模
    bufnode_.head_.busy_ = pos;
    //接下来开始决定放在那一边
    if(place_right){
        //放在右边,先改bufnode
        bufnode_ = new_node;
    }
    //插入
    insertNoSplit(key,data);
    //随后把键值向上插入
    insertKey2Index(0);
}
