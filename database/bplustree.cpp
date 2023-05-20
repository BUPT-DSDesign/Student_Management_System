#include "datatype.hpp"
#include "bplustree.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <sstream>
using namespace std;
BPNode::BPNode(){
    //初始化vector大小
    int max_space = PAGE_SIZE - sizeof(BPNodeHead);//最大的空闲空间
    //假设每个data最小为1字节
    int max_degree = (max_space-sizeof(streampos))/(sizeof(streampos)+1)+1;
    child_ = vector<streampos>(max_degree);
    //data需要开大一点
    data_ = vector<byte>(max_space);
    //node_pos设置为0,表示没有新建
    node_pos = 0;
}

/*streampos BPNode::getElemLocation(int id){
    return node_pos + head_.key_pos_ + id*head_.data_size_;
}*/
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
        //如果是叶子节点的话,仅在开始和结束有指针
        data_.clear();
        child_.clear();
        streampos child_p;
        if(head_.is_leaf_){
            //如果是叶子节点,读出前一个节点
            fp.read((char*)&child_p,sizeof(child_p));
            child_.push_back(child_p);
        }
        for(int i=0;i<head_.busy_;i++){
            //如果不是叶节点,才有child
            if(!head_.is_leaf_){
                fp.read((char*)&child_p,sizeof(child_p));
                child_.push_back(child_p);
            }
            fp.read((char*)&data_[data_.size()],head_.data_size_);
        }
        //如果不是叶子结点,这里记录最后一个孩子
        //如果是叶子节点,这里记录下一个元素位置
        fp.read((char*)&child_p,sizeof(child_p));
        child_.push_back(child_p);
    }
    //读取完毕,关闭文件读写流
    fp.close();
}
void BPNode::WriteChunk(){
    //将节点按格式写入硬盘
    //1.打开文件读写流
    fstream fp(file_name_.data(),ios::binary|ios::in|ios::out);
    fp.seekp(head_.chunk_pos_,ios::beg);
    //2.写入
    //第一部分,写入结构体头
    fp.write((char*)&head_,sizeof(head_));
    //第二部分,写入节点数据
    if(head_.is_leaf_){
        //如果是叶子节点,写出前一个节点
         fp.write((char*)&child_[0],sizeof(child_[0]));
    }
    for(int i=0;i<head_.busy_;i++){
        if(!head_.is_leaf_)
            //如果不是叶子节点的话才有child
            fp.write((char*)&child_[i],sizeof(child_[i]));
        fp.write((char*)&data_[data_.size()],head_.data_size_);
    }
    if(!head_.is_leaf_)
        fp.write((char*)&child_[head_.busy_],sizeof(child_[head_.busy_]));
    else
        //如果是leaf节点,则仅有一个,指向下一个节点
        fp.write((char*)&child_[1],sizeof(child_[1]));
    //写入完毕,关闭文件写入流
    fp.close();
}
void BPNode::CreateChunk(bool is_leaf,int data_size)
{
    //0.计算出头信息
    head_.father_ = 0;
    head_.is_dirty_ = false;
    head_.is_leaf_ = is_leaf;
    head_.data_size_ = data_size;
    head_.key_type_ = T_BIG_INT;
    head_.degree_ = (PAGE_SIZE-sizeof(BPNodeHead)-sizeof(streampos))/(sizeof(streampos)+data_size)+1;
    head_.busy_ = 0;
    //去系统中要一块新内存写入
    //1.获取新页位置
    //TODO 此处可改为获取一块脏页
    char buffer[PAGE_SIZE];
    fstream fp(file_name_.data(),ios::out|ios::binary);
    fp.seekp(0,ios::end);
    node_pos = fp.tellp();
    head_.chunk_pos_ = node_pos;
    //2.填充空数据
    //TODO 如果是脏数据则忽略
    fp.write(buffer,sizeof(buffer));
    //3.关闭读写流
    fp.close();
}
uint16 BPNode::getElemLocInData(int id){
    return id*head_.data_size_;
}
void BPNode::insertDataAtPos(int id,const uint64 &key,const vector<byte>& data){
    
    //先拿到写入位置
    uint16 pos = getElemLocInData(id);
    //随后开始写入
    //前8个字节为key
    std::copy(reinterpret_cast<const std::byte*>(&key),reinterpret_cast<const std::byte*>(&key)+sizeof(key),dataLoc(pos));
    pos += sizeof(key);
    //随后开始把要改写的数据拷贝进入data
    std::copy(data.begin(),data.end(),dataLoc(pos));
}
//对于内部节点,data中存储的是key,child中存储的是孩子的位置
//对于叶子节点,data中存储的是key和data,child中存储的是指向下一个叶子节点的指针
std::vector<std::byte>::iterator BPNode::dataBegin(){
    return data_.begin();
}
std::vector<std::byte>::iterator BPNode::dataEnd(){
    //得判断是不是叶子节点,叶子节点需要另外一套操作
    uint16 key_shift = (head_.is_leaf_)?sizeof(uint64):0;
    return data_.begin()+head_.busy_*(head_.data_size_+key_shift);
}
std::vector<std::byte>::iterator BPNode::dataLoc(int id){
    //得判断是不是叶子节点,叶子节点需要另外一套操作
    uint16 key_shift = (head_.is_leaf_)?sizeof(uint64):0;
    if(id*(head_.data_size_+key_shift) <= data_.size() && id >= 0){
        return data_.begin()+id*(head_.data_size_+key_shift);
    }
    return data_.begin()+head_.busy_*(head_.data_size_+key_shift);
}
std::vector<std::streampos>::iterator BPNode::childBegin(){
    return child_.begin();
}
std::vector<std::streampos>::iterator BPNode::childEnd(){
    return child_.begin()+head_.busy_;
}
std::vector<std::streampos>::iterator BPNode::childLoc(int id){
    if(id <= head_.busy_ && id >= 0){
        return child_.begin()+id;
    }
    return childEnd();
}
uint64 BPNode::getKey(int id){
    uint64 key;
    //获取键值,键值存在位置不同,如果是叶子则为实际存储数据的前8个字节,如果是非叶节点则就是data
    std::copy(dataLoc(id*sizeof(uint64)),dataLoc(id*sizeof(uint64)+sizeof(uint64)),reinterpret_cast<std::byte*>(&key));
    return key;
}

//下面是B+树的实现
BPTree::BPTree(string path)
{
    bufnode_.file_name_ = path;
}

BPTree::~BPTree(){
    
}
//核心算法1:二分查找
int BPTree::binarySearch(BPNode &node,const uint64 &key){
    int l=-1,r=node.getElemCount() - 1;
    while(l + 1 < r){
        int mid = (l+r) >> 1;
        uint64 node_key = node.getKey(mid);
        if(key >= node_key){
            r = mid;
        }else{
            l = mid;
        }
    }
    return l;
}
void BPTree::searchLeaf(const uint64 &key){
    while(cur_!=-1){
        //第一步,利用cur_来载入一个块
        bufnode_.ReadChunk(cur_);
        if(bufnode_.isLeaf()){
            //如果是叶子节点了,就开始找值
            break;
        }
        //否则开始迭代叶子结点
        //每个区块里的元素都是从小到大排列
        //child_id是最靠左且小于等于key的孩子的id
        int child_id = binarySearch(bufnode_,key);
        cur_ = bufnode_.getChild(child_id);
    }
}
vector<byte> BPTree::Search(const uint64 &key){
    //搜索
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,key);
    //第三步,读取对应数据,返回字节流供解析
    vector<byte> elem = bufnode_.getRawData(pos);
    return elem;
    
}
bool BPTree::Insert(const uint64 &key,vector<byte> &data){
    cur_ = root_pos_;
    //先搜索这个叶子节点
    searchLeaf(key);
    //接着判断键值是否重复
    int pos = binarySearch(bufnode_,key);
    if(is_table_&&key == bufnode_.getKey(pos)){
        //键值重复,无法插入(表的主键不能重复)
        return false;
    }
    if(bufnode_.head_.busy_==bufnode_.head_.degree_){
        //如果节点满了,需要分裂
        //TODO 利用延时写入来优化分裂,即不立即写入,而是等到需要的时候再写入,或等到缓存满了再写入
        splitTreeNode(key,data);
        //把旧的根节点作为新的根节点的孩子
    }else{
        //如果节点没有满,直接添加
        insertNoSplit(bufnode_,key,data);
        bufnode_.WriteChunk();
    }
}
void BPTree::insertKey(const uint64 &key,const streampos &old,const streampos &after){
    if(cur_==0){
        //新建一个根节点
        BPNode root;
        //非叶子节点,节点单元素大小为键值大小
        root.CreateChunk(false,sizeof(uint64));
        //节点只有一个,即键值
        root.head_.busy_ = 1;
        //将值写入
        std::copy(reinterpret_cast<const std::byte*>(&key),reinterpret_cast<const std::byte*>(&key)+sizeof(key),root.data_.begin());
        //然后old为第一个孩子,after为第二个孩子(正好一前一后)
        root.child_[0] = old;
        root.child_[1] = after;
        //新建完了之后写入
        root.WriteChunk();
        //最后将其更改为真正的根节点
        root_pos_ = root.head_.chunk_pos_;
        //接着更新各个节点的父亲
        resetIndexChildrenParent(root);
        //结束
        return;
    }
    //如果这不算根节点
    BPNode inner_node;
    inner_node.ReadChunk(cur_);
    if(inner_node.head_.busy_ == inner_node.head_.degree_){
        //满啦,需要分裂
        BPNode new_node;
        new_node.CreateChunk(false,sizeof(uint64));
        //找到二分点
        uint16 pos = (inner_node.head_.busy_ - 1) / 2;
        bool place_right = key > inner_node.getKey(pos);
        if(place_right){
            ++pos;
        }
        //此处主要避免一种神奇的情况
        //比如说,阶数为3,原始数据分别为18 19 30,需要插入的数据为20
        //此时pos为1,place_right为true,但是此时应该插入到左边
        //所以需要判断一下,如果place_right为true,且key小于pos的值,则pos--
        if(place_right&& key < inner_node.getKey(pos))pos--;
        //开始分裂吧
        //先分裂数据
        std::copy(inner_node.dataLoc(pos+1),inner_node.dataEnd(),new_node.dataBegin());
        //然后分裂孩子节点
        //如 child0|key0|child1|key1|child2|key2|child3
        //分裂为 child0|key0|child1 = child2|key2|child3
        std::copy(inner_node.childLoc(pos+1),inner_node.childEnd(),new_node.childBegin());
        //然后修改两个节点的busy
        new_node.head_.busy_ = inner_node.head_.busy_ - pos - 1;
        inner_node.head_.busy_ = pos + 1;
        //然后选一个位置放下吧
        if(place_right){
            insertNoSplit(new_node,key,vector<byte>{});
        }else{
            insertNoSplit(inner_node,key,vector<byte>{});
        }
        //保存修改过的两个区块
        new_node.WriteChunk();
        inner_node.WriteChunk();
        new_node.head_.father_ = inner_node.head_.father_;
        resetIndexChildrenParent(new_node);
        //然后把键值向上插入
        cur_ = inner_node.head_.father_;
        insertKey(new_node.getKey(0),new_node.head_.chunk_pos_,new_node.head_.chunk_pos_);
    }else{
        //不需要拓,直接插入
        insertNoSplit(inner_node,key,vector<byte>{});
        inner_node.WriteChunk();
    }
}

void BPTree::splitTreeNode(const uint64 &key,vector<byte> &data){
    //首先需要一分为二,故先创建一个邻居
    BPNode new_node;
    //此处新建的为叶子节点
    new_node.CreateChunk(true,size_of_item);
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
    //分裂,将左节点的从pos到end全拷贝到new_node里
    //由于是叶子节点,只需要考虑copy data即可
    new_node.head_.busy_ = bufnode_.head_.busy_ - pos;
    new_node.data_.resize(new_node.head_.busy_*new_node.head_.data_size_);
    std::copy(bufnode_.dataLoc(pos),bufnode_.dataEnd(),new_node.data_.begin());
    //然后减小左节点规模
    bufnode_.head_.busy_ = pos;
    bufnode_.data_.resize(pos*new_node.head_.data_size_);
    //接下来开始决定放在那一边
    if(place_right){
        //插入到new_node里
        insertNoSplit(new_node,key,data);
    }else{
        //否则插入到bufnode里
        insertNoSplit(bufnode_,key,data);
    }
    //new_node应该插入到bufnode_的右侧,执行双向链表的插入操作
    new_node.child_[1] = bufnode_.child_[1];
    new_node.child_[0] = bufnode_.head_.chunk_pos_;
    //保存修改后的两个节点
    new_node.WriteChunk();
    bufnode_.WriteChunk();
    //更新父亲节点
    new_node.head_.father_ = bufnode_.head_.father_;
    //随后把键值向上插入
    cur_ = bufnode_.head_.father_;
    bufnode_ = new_node;
    insertKey(new_node.getKey(0),bufnode_.head_.chunk_pos_,bufnode_.child_[1]);
}

void BPTree::insertNoSplit(BPNode &node,const uint64 &key,vector<byte> &data){
    //先用二分查找定位
    int pos = binarySearch(node,key);
    //O(n)的插入操作
    //先将前面的元素结尾挪到后面
    std::copy_backward(node.dataLoc(pos),node.dataEnd(),node.dataEnd()+node.head_.data_size_);
    //增加busy数量
    node.head_.busy_++;
    node.insertDataAtPos(pos,key,data);
    
}
void BPTree::resetIndexChildrenParent(BPNode &node){
    //简简单单更新下所有节点的父亲节点
    for(auto it=node.childBegin();it!=node.childEnd();it++){
        BPNode tmpNode;
        tmpNode.ReadChunk(*it);
        tmpNode.head_.father_ = node.head_.chunk_pos_;
        tmpNode.WriteChunk();
    }
}

void BPTree::ReadFirstChunk(){
    //即读取最小的那个叶子节点
    cur_ = root_pos_;
    while(cur_!=-1){
        bufnode_.ReadChunk(cur_);
        if(bufnode_.isLeaf()){
            //如果是叶子节点了,就开始找值
            break;
        }
        //否则开始迭代叶子结点
        //每个区块里的元素都是从小到大排列
        //child_id是最靠左且小于等于key的孩子的id
        cur_ = bufnode_.getChild(0);
    }
}

void BPTree::ReadPrevChunk(){
    //此时必须为叶子节点
    if(bufnode_.isLeaf()){
        cur_ = bufnode_.child_[0];
        bufnode_.ReadChunk(cur_);
    }
}
void BPTree::ReadNextChunk(){
    //此时必须为叶子节点
    if(bufnode_.isLeaf()){
        cur_ = bufnode_.child_[1];
        bufnode_.ReadChunk(cur_);
    }
}

vector<vector<byte>> BPTree::GetAllElemInChunk(){
    //获取当前节点的所有元素
    vector<vector<byte>> res;
    if(bufnode_.isLeaf()){
        for(auto it=bufnode_.dataBegin();it!=bufnode_.dataEnd();it+=bufnode_.head_.data_size_){
            vector<byte> elem(it,it+bufnode_.head_.data_size_);
            res.push_back(elem);
        }
    }
    return res;
}