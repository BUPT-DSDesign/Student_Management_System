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
    //前四个字节为key
    std::copy(&key,&key+sizeof(key),dataLoc(pos));
    pos += sizeof(key);
    //随后开始把要改写的数据拷贝进入data
    std::copy(data.begin(),data.end(),dataLoc(pos));
}
auto BPNode::dataBegin(){
    return data_.begin();
}
auto BPNode::dataEnd(){
    //得判断是不是叶子节点,叶子节点需要另外一套操作
    uint16 key_shift = (head_.is_leaf_)?sizeof(uint64):0;
    return data_.begin()+head_.busy_*(head_.data_size_+key_shift);
}
auto BPNode::dataLoc(int id){
    //得判断是不是叶子节点,叶子节点需要另外一套操作
    uint16 key_shift = (head_.is_leaf_)?sizeof(uint64):0;
    if(id*(head_.data_size_+key_shift) <= data_.size() && id >= 0){
        return data_.begin()+id*(head_.data_size_+key_shift);
    }
    return dataEnd();
}
auto BPNode::childBegin(){
    return child_.begin();
}
auto BPNode::childEnd(){
    return child_.begin()+head_.busy_+1;
}
auto BPNode::childLoc(int id){
    if(id <= head_.busy_ && id >= 0){
        return child_.begin()+id;
    }
    return childEnd();
}
uint64 BPNode::getKey(int id){
    uint64 key;
    //获取键值,键值存在位置不同,如果是叶子则为实际存储数据前的4个字节,如果是非叶节点则就是data
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
        col_info_.push_back(buf_atr);
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
        col_info_.push_back(t);
    }
    //记录B+树根节点位置
    root_pos_ = fp.tellg();
    fp.close();
}
BPTree::~BPTree(){
    
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
    return r;
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
        int child_id = binarySearch(bufnode_,key);
        cur_ = bufnode_.getChild(child_id);
    }
}
string BPTree::Search(const uint64 &key){
    //搜索(有索引的)
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,key);
    //第三步,读取对应数据,以JSON格式返回
    vector<byte> elem = bufnode_.getRawData(pos);
    return deserialize(elem);
    
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
        std::copy(&key,&key+sizeof(key),root.data_.begin());
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
        //源代码这里有个Prevent,不知道啥意思
        if(place_right&& key < inner_node.getKey(pos))pos--;
        //开始分裂吧
        //先分裂数据
        std::copy(inner_node.dataLoc(pos+1),inner_node.dataEnd(),new_node.dataBegin());
        //TODO 然后分裂孩子节点
        //std::copy();
        new_node.head_.busy_ = inner_node.head_.busy_ - pos - 1;
        inner_node.head_.busy_ = pos + 1;
        //然后选一个位置放下吧
        if(place_right){
            insertNoSplit(new_node,key,{});
        }else{
            insertNoSplit(inner_node,key,{});
        }
        //保存修改过的两个区块
        new_node.WriteChunk();
        inner_node.WriteChunk();
        resetIndexChildrenParent(new_node);
    }else{
        //不需要拓,直接插入
        insertNoSplit(inner_node,key,{});
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
    new_node.head_.busy_ = bufnode_.head_.busy_ - pos;
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
string BPTree::deserialize(vector<byte> &data){
    //TODO 解析字节流数据为JSON并输出
    //每次只能解析一个行数据
    if(data.size()!=bufnode_.head_.data_size_){
        //TODO 传入的不是bufnode的数据,报错
        return "{}";
    }
   stringstream json;
    //一条JSON信息举例
    /*
   {
     "ColName1":Val1,
     "ColName2":Val2,
    }
    */
    
    auto nowProcess = col_info_.begin();
    for(auto it=data.begin();it!=data.end()&&nowProcess!=col_info_.end();){
        if(nowProcess==col_info_.begin()){
            //如果是开头,需要加上一个花括号
            json<<"{";
        }
        //先加上名字
        json << "\"" << nowProcess->col_name_ <<"\":";
        //cout<<json.str()<<endl;
        //cout<<json<<endl;
        //先判断现在读取的是哪一列
        switch(nowProcess->data_type_){
            //1.数值类型:整型(符号型)
            case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
                int64 val_int64;
                std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_int64));
                it += nowProcess->length_;
                json << val_int64;
                break;
            //2.数值类型:浮点型(单精度)
            case T_FLOAT:
                float val_float;
                std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_float));
                it += nowProcess->length_;
                json << val_float;
                break;
            //3.数值类型:浮点型(双精度)
            case T_DOUBLE:
                double val_double;
                std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_double));
                it += nowProcess->length_;
                json <<val_double;
                break;
            //4.数值类型:无符号类型(日期和时间类型)
            case T_DATE:case T_TIME:case T_YEAR:case T_TIMESTAMP:
                uint64 val_uint64;
                std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_uint64));
                it += nowProcess->length_;
                json << val_uint64;
                break;
            //5.字符类型
            default:
                char buffer[PAGE_SIZE]={0};//输出缓冲
                std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(buffer));
                it += nowProcess->length_;
                buffer[nowProcess->length_] = 0;//截断
                string varchar;
                varchar.assign(buffer);
                json << varchar;
                break;
        }
        nowProcess++;
        if(nowProcess==col_info_.end()){
            //到结尾了,需要加上},
            json<<"}";
            break;
        }else{
            json<<",";
        }
    }
    if(nowProcess!=col_info_.end()){
        //TODO 数据错误
        return "{}";
    }
    return json.str();
}