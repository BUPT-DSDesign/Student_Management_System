#include "datatype.hpp"
#include "bplustree.hpp"
#include "MyException.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
#include <sstream>
#include <filesystem>
using namespace std;
const int32 INVALID_OFFSET = -1;
BPNode::BPNode(){
    //初始化vector大小
    int max_space = PAGE_SIZE - sizeof(BPNodeHead);//最大的空闲空间
    //假设每个data最小为1字节
    int max_degree = (max_space-sizeof(streampos))/(sizeof(streampos)+1)+1;
    child_ = vector<streampos>();
    child_.reserve(max_degree);
    //data需要开大一点
    data_ = vector<byte>();
    data_.reserve(max_space);
    //node_pos设置为INVALID_OFFSET,表示没有新建
    node_pos = INVALID_OFFSET;
}
void BPNode::setElem(int id,const vector<byte> &data){
    //设置第id个元素的值
    //先找出第id个元素的位置
    uint16 pos = getElemLocInData(id);
    //将data复制到data_中
    std::copy(data.begin(),data.end(),data_.begin()+pos);
}
bool BPNode::isLeaf(){
    return is_leaf_;
}
uint16 BPNode::getElemCount(){
    return busy_;
}
streampos BPNode::getChild(int id){
    if(id < 0){
        return -1;
    }
    if(id < child_.size())
        return child_[id];
    return -1;
}
void BPNode::ReadChunk(streampos pos){
    if(pos == INVALID_OFFSET){
        //如果是无效的偏移量,则不读取
        return;
    }
    BPNodeHead head;
    //TODO 将一个区块读取到字节流中
    //1.打开文件读写流
    fstream fp(file_name_.data(),ios::binary|ios::in);
    fp.seekg(pos,ios::beg);
    if(!fp.eof()){
        //如果没有读写到文件末尾
        //第一个部分,结构体头
        fp.read((char*)&head,sizeof(head));
        //用结构体头的数据更新当前节点的成员变量
        is_leaf_ = head.is_leaf_;
        is_dirty_ = head.is_dirty_;
        key_type_ = head.key_type_;
        degree_ = head.degree_;
        busy_ = head.busy_;
        data_size_ = head.data_size_;
        father_ = head.father_;
        node_pos = head.chunk_pos_;
        key_size_ = head.key_size_;
        //第二个部分,读取节点数据
        //整体结构为 指针0 | 数据1 | 指针1 | ... | 数据n | 指针n 
        //如果是叶子节点的话,仅在开始和结束有指针
        data_.clear();
        child_.clear();
        streampos child_p;
        if(head.is_leaf_){
            //如果是叶子节点,读出前一个节点
            fp.read((char*)&child_p,sizeof(child_p));
            child_.push_back(child_p);
        }
        for(int i=0;i<head.busy_;i++){
            //如果不是叶节点,才有child
            if(!head.is_leaf_){
                fp.read((char*)&child_p,sizeof(child_p));
                child_.push_back(child_p);
            }
            //读取数据
            vector<byte> tmp(head.data_size_);
            fp.read((char*)tmp.data(),head.data_size_);
            data_.insert(data_.end(),tmp.begin(),tmp.end());
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
    fp.seekp(node_pos,ios::beg);
    //2.写入
    //第一部分,写入结构体头
    BPNodeHead head_;
    head_.is_dirty_ = is_dirty_;
    head_.is_leaf_ = is_leaf_;
    head_.key_type_ = key_type_;
    head_.degree_ = degree_;
    head_.busy_ = busy_;
    head_.data_size_ = data_size_;
    head_.father_ = father_;
    head_.key_size_ = key_size_;
    head_.chunk_pos_ = node_pos;
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
void BPNode::CreateChunk(bool is_leaf,int data_size,uint16 key_size,uint8 key_type)
{
    //0.计算出头信息
    father_ = INVALID_OFFSET;
    is_dirty_ = false;
    is_leaf_ = is_leaf;
    data_size_ = data_size;
    key_type_ = key_type;
    key_size_ = key_size;
    degree_ = (PAGE_SIZE-sizeof(BPNodeHead)-sizeof(streampos))/(sizeof(streampos)+data_size)+1;
    busy_ = 0;
    //去系统中要一块新内存写入
    //1.获取新页位置
    //此处可改为获取一块脏页
    char buffer[PAGE_SIZE] = {0};
    fstream fp(file_name_.data(),ios::out|ios::binary);
    fp.seekp(0,ios::end);
    node_pos = fp.tellp();
    BPNodeHead head = {is_dirty_,is_leaf_,key_type_,key_size_,degree_,busy_,data_size_,father_,node_pos};
    //2.将头信息写入
    std::copy((char*)&head,(char*)&head+sizeof(head),buffer);
    //3.填充空数据
    fp.write(buffer,sizeof(buffer));
    //4.关闭读写流
    fp.close();
}
streampos BPNode::releaseChunk(){
    //从磁盘中卸载当前块
    //1.打开文件读写流
    fstream fp(file_name_.data(),ios::binary|ios::in|ios::out);
    fp.seekp(node_pos,ios::beg);
    //2.将其标记为脏页
    is_dirty_ = true;
    //3.写入
    BPNodeHead head_ = {is_dirty_,is_leaf_,key_type_,key_size_,degree_,busy_,data_size_,father_,node_pos};
    fp.write((char*)&head_,sizeof(head_));
    //4.关闭文件读写流
    fp.close();
    return node_pos;
}
vector<byte> BPNode::getRawData(int id){
    //获取第id个元素的原始数据
    vector<byte> tmp(data_size_);
    uint16 pos = getElemLocInData(id);
    std::copy(data_.begin()+pos,data_.begin()+pos+data_size_,tmp.begin());
    return tmp;
}
void BPNode::setKey(int id,const Key &key){
    //设置第id个元素的key
    uint16 pos = getElemLocInData(id);
    std::copy(key.getBytes().begin(),key.getBytes().end(),data_.begin()+pos);
}
uint16 BPNode::getElemLocInData(int id){
    return id*data_size_;
}
void BPNode::insertDataAtPos(int id,const Key &key,const vector<byte>& data){
    
    //先拿到写入位置
    uint16 pos = getElemLocInData(id);
    //随后开始写入
    //前8个字节为key,后面为data
    vector<byte> tmp;
    //先把key拷贝进去
    tmp.insert(tmp.end(),key.getBytes().begin(),key.getBytes().end());
    //再把data拷贝进去
    tmp.insert(tmp.end(),data.begin(),data.end());
    //最后写入data_
    data_.insert(data_.begin()+pos,tmp.begin(),tmp.end());
}

//对于内部节点,data中存储的是key,child中存储的是孩子的位置
//对于叶子节点,data中存储的是key和data,child中存储的是指向下一个叶子节点的指针
std::vector<std::byte>::iterator BPNode::dataBegin(){
    return data_.begin();
}
std::vector<std::byte>::iterator BPNode::dataEnd(){
    return data_.end();
}
std::vector<std::byte>::iterator BPNode::dataLoc(int id){
    //得判断是不是叶子节点,叶子节点需要另外一套操作
    //TODO id可能为-1
    if(id < 0)
        return data_.begin();
    //如果是叶子节点,data_size_部分没包括key,所以要加上key的长度
    uint16 key_shift = (is_leaf_)?key_size_:0;
    if(id*(data_size_+key_shift) <= data_.size() && id >= 0){
        return data_.begin()+id*(data_size_+key_shift);
    }
    return data_.begin()+busy_*(data_size_+key_shift);
}
std::vector<std::streampos>::iterator BPNode::childBegin(){
    return child_.begin();
}
std::vector<std::streampos>::iterator BPNode::childEnd(){
    return child_.end();
}
std::vector<std::streampos>::iterator BPNode::childLoc(int id){
    if(id <= busy_ && id >= 0){
        return child_.begin()+id;
    }
    if(id < 0)
        return child_.begin();
    return childEnd();
}
Key BPNode::getKey(int id){
    Key key(dataLoc(id),key_size_,key_type_);
    return key;
}

//下面是B+树的实现
BPTree::BPTree(string path)
{
    bufnode_.file_name_ = path;
    //如果没有文件,则创建一个文件
    
    if(!std::filesystem::exists(bufnode_.file_name_)){
        throw BPTreeException("File Do not Exist!");
    }
}
BPTree::BPTree(string path,bool is_table,int data_size,uint16 key_size,uint8 key_type)
:is_table_(is_table),size_of_item_(data_size),key_type_(key_type)
{
    bufnode_.file_name_ = path;
    //如果已经新建过了,就报错
    if(std::filesystem::exists(bufnode_.file_name_)){
        throw BPTreeException("File Exist!");
    }
    //如果是表,则需要初始化一些属性
    bufnode_.CreateChunk(true,data_size,key_size,key_type);
    //初始化BPTree类

}
BPTree::~BPTree(){
    
}
bool BPTree::isBufLeaf(){
    return bufnode_.isLeaf();
}
//核心算法1:二分查找
int BPTree::binarySearch(BPNode &node,const Key &key){
    int l=-1,r=node.getElemCount() - 1;
    while(l + 1 < r){
        int mid = (l+r) >> 1;
        Key node_key = node.getKey(mid);
        if(key >= node_key){
            r = mid;
        }else{
            l = mid;
        }
    }
    return l;
}
void BPTree::searchLeaf(const Key &key){
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
vector<byte> BPTree::Search(const Key &key){
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
streampos BPTree::SearchPos(const Key &key){
    //搜索
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    binarySearch(bufnode_,key);
    //第三步,返回对应位置
    return cur_;
}
vector<vector<byte>> BPTree::SearchRange(const Key& left,const Key& right){
    //搜索
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(left);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,left);
    if(bufnode_.getKey(pos) < left){
        //如果找到的不是第一个,则需要向后移动一位
        pos++;
    }
    //第三步,读取对应数据,返回字节流供解析
    vector<vector<byte>> result;
    while(bufnode_.getKey(pos) <= right){
        result.push_back(bufnode_.getRawData(pos));
        pos++;
    }
    return result;
}

bool BPTree::Insert(const Key &key,vector<byte> &data){
    cur_ = root_pos_;
    //先搜索这个叶子节点
    searchLeaf(key);
    //接着判断键值是否重复
    int pos = binarySearch(bufnode_,key);
    if(is_table_&&key == bufnode_.getKey(pos)){
        //键值重复,无法插入(表的主键不能重复)
        return false;
    }
    if(bufnode_.busy_==bufnode_.degree_){
        //如果节点满了,需要分裂
        //TODO 利用延时写入来优化分裂,即不立即写入,而是等到需要的时候再写入,或等到缓存满了再写入
        splitTreeNode(key,data);
        //把旧的根节点作为新的根节点的孩子
    }else{
        //如果节点没有满,直接添加
        insertNoSplit(bufnode_,key,data);
        bufnode_.WriteChunk();
    }
    return true;
}
void BPTree::insertKey(const Key &key,const streampos &old,const streampos &after){
    if(cur_==-1){
        //新建一个根节点
        BPNode root;
        //非叶子节点,节点单元素大小为streampos大小
        root.CreateChunk(false,sizeof(streampos),bufnode_.key_size_,bufnode_.key_type_);
        //节点只有一个,即键值
        root.busy_ = 1;
        //将值写入
        root.data_.insert(root.data_.begin(),key.getBytes().begin(),key.getBytes().end());
        //然后old为第一个孩子,after为第二个孩子(正好一前一后)
        root.child_[0] = old;
        root.child_[1] = after;
        //新建完了之后写入
        root.WriteChunk();
        //最后将其更改为真正的根节点
        root_pos_ = root.node_pos;
        //接着更新各个节点的父亲
        resetIndexChildrenParent(root);
        //结束
        return;
    }
    //如果这不算根节点
    BPNode inner_node;
    inner_node.ReadChunk(cur_);
    if(inner_node.busy_ == inner_node.degree_){
        //满啦,需要分裂
        BPNode new_node;
        new_node.CreateChunk(false,sizeof(streampos),bufnode_.key_size_,bufnode_.key_type_);
        //找到二分点
        uint16 pos = (inner_node.busy_ - 1) / 2;
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
        new_node.data_ = vector<byte>(inner_node.dataLoc(pos+1),inner_node.dataEnd());
        //然后分裂孩子节点
        //如 child0|key0|child1|key1|child2|key2|child3
        //分裂为 child0|key0|child1 = child2|key2|child3
        new_node.child_ = vector<streampos>(inner_node.childLoc(pos+1),inner_node.childEnd());
        //然后修改两个节点的busy
        new_node.busy_ = inner_node.busy_ - pos - 1;
        inner_node.busy_ = pos + 1;
        //然后选一个位置放下吧

        if(place_right){
            insertNoSplit(new_node,key,vector<byte>{});
        }else{
            insertNoSplit(inner_node,key,vector<byte>{});
        }
        //保存修改过的两个区块
        new_node.WriteChunk();
        inner_node.WriteChunk();
        new_node.father_ = inner_node.father_;
        resetIndexChildrenParent(new_node);
        //然后把键值向上插入
        cur_ = inner_node.father_;
        insertKey(new_node.getKey(0),new_node.node_pos,new_node.node_pos);
    }else{
        //不需要拓,直接插入
        insertNoSplit(inner_node,key,vector<byte>{});
        inner_node.WriteChunk();
    }
}

void BPTree::splitTreeNode(const Key &key,vector<byte> &data){
    //首先需要一分为二,故先创建一个邻居
    BPNode new_node;
    //此处新建的为叶子节点
    new_node.CreateChunk(true,size_of_item_,bufnode_.key_size_,bufnode_.key_type_);
    //找到二分点
    //在分裂为两个叶子结点之后，左节点包含前m/2个记录，右节点包含剩下的记录
    //将第m/2+1个记录的关键字进位到父节点中(下标m/2)
    uint16 pos = bufnode_.busy_/2;
    //比较函数
    bool place_right = false;
    if(key > bufnode_.getKey(pos)){
        //key应该放在右边
        place_right = true;
        pos++;
    }
    //分裂,将左节点的从pos到end全拷贝到new_node里
    //由于是叶子节点,只需要考虑copy data即可
    new_node.busy_ = bufnode_.busy_ - pos;
    new_node.data_ = vector<byte>(bufnode_.dataLoc(pos),bufnode_.dataEnd());
    //然后减小左节点规模
    bufnode_.busy_ = pos;
    bufnode_.data_.resize(pos*bufnode_.data_size_);
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
    new_node.child_[0] = bufnode_.node_pos;
    //保存修改后的两个节点
    new_node.WriteChunk();
    bufnode_.WriteChunk();
    //更新父亲节点
    new_node.father_ = bufnode_.father_;
    //随后把键值向上插入
    cur_ = bufnode_.father_;
    bufnode_ = new_node;
    insertKey(new_node.getKey(0),bufnode_.node_pos,bufnode_.child_[1]);
}

void BPTree::insertNoSplit(BPNode &node,const Key &key,const vector<byte> &data){
    //先用二分查找定位
    int pos = binarySearch(node,key);
    //O(n)的插入操作
    //增加busy数量
    node.busy_++;
    node.insertDataAtPos(pos,key,data);
    
}
void BPTree::resetIndexChildrenParent(BPNode &node){
    //简简单单更新下所有节点的父亲节点
    for(auto it=node.childBegin();it!=node.childEnd();it++){
        BPNode tmpNode;
        tmpNode.ReadChunk(*it);
        tmpNode.father_ = node.node_pos;
        tmpNode.WriteChunk();
    }
}
void BPTree::ReadChunk(streampos pos){
    if(pos == cur_){
        //如果已经读取过了,就不用再读了
        return;
    }
    bufnode_.ReadChunk(pos);
    cur_ = pos;
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
        for(auto it=bufnode_.dataBegin();it!=bufnode_.dataEnd();it+=bufnode_.data_size_){
            vector<byte> elem(it,it+bufnode_.data_size_);
            res.push_back(elem);
        }
    }
    return res;
}

vector<vector<byte>> BPTree::GetAllElemInTree(){
    //获取整个树的所有元素
    vector<vector<byte>> res;
    //先读取第一个叶子节点
    ReadFirstChunk();
    //然后开始迭代
    while(cur_!=-1){
        //获取当前节点的所有元素
        vector<vector<byte>> tmp = GetAllElemInChunk();
        //将其插入到res中
        res.insert(res.end(),tmp.begin(),tmp.end());
        //读取下一个节点
        ReadNextChunk();
    }
    return res;
}

bool BPTree::Update(const Key &key,vector<byte> &data){
    //更新,只适用于不更改键值的情况
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,key);
    if(key != bufnode_.getKey(pos)){
        //找不到这样的键值
        return false;
    }
    //如果找到了,则更新
    while(cur_!=-1&&bufnode_.getKey(pos) == key){
        //更新数据
        bufnode_.insertDataAtPos(pos,key,data);
        //更新下一个
        pos++;
        //如果到了这个页的末尾,则需要读取下一个页
        if(pos == bufnode_.getElemCount()){
            //写入当前页
            bufnode_.WriteChunk();
            //读取下一个页
            ReadNextChunk();
            pos = 0;
        }
    }

    bufnode_.WriteChunk();
    return true;
}

vector<byte> BPTree::Remove(const Key &key){
    //删除
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,key);
    if(key != bufnode_.getKey(pos)){
        //找不到这样的键值
        return vector<byte>();
    }
    //如果找到了,则删除
    //先获取数据
    vector<byte> res = bufnode_.getRawData(pos);
    
    if(bufnode_.father_ == -1){
        //当前节点为根节点
        if(bufnode_.busy_ == 1){
            //就剩下一个了,删除就润
            bufnode_.releaseChunk();
        }else{
            //正常删除就行
            simpleLeafRemove(bufnode_,pos);
            bufnode_.WriteChunk();
        }
    }else if(bufnode_.busy_ < (bufnode_.degree_ + 1)/2){
        //如果删除之后,节点的元素个数小于等于阶数的一半,则需要进行调整操作
        BPNode parent,lSibling,rSibling;
        parent.ReadChunk(bufnode_.father_);
        //TODO 此处空该怎么办？
        lSibling.ReadChunk(parent.getChild(0));
        rSibling.ReadChunk(parent.getChild(1));
        //查找父节点中,当前页对应的孩子的位置
        int parent_pos = binarySearch(parent,bufnode_.getKey(0));
        //下面决定是从左边借还是从右边借
        enum class BorrowFrom{
            LEFT,
            RIGHT
        };
        //判断逻辑:如果当前节点是父节点的第一个孩子,则从右边借;
        //如果当前节点是最后一个孩子,则从左边借;
        //如果都不是,哪边更多就从哪边借
        BorrowFrom borrow_from;
        if(parent_pos == -1){
            //从右边借
            borrow_from = BorrowFrom::RIGHT;
        }else if(parent_pos == parent.busy_ - 1){
            //从左边借
            borrow_from = BorrowFrom::LEFT;
        }else{
            //从左边借还是从右边借呢?
            if(lSibling.busy_ > rSibling.busy_){
                //从左边借
                borrow_from = BorrowFrom::LEFT;
            }else{
                //从右边借
                borrow_from = BorrowFrom::RIGHT;
            }
        }
        //TODO 开借
        if(borrow_from == BorrowFrom::LEFT){
            if(lSibling.busy_ >= (lSibling.degree_ + 1)/2){
                //左边的邻居大过m/2,把左边的邻居的最后一个元素借过来
                leafShiftFromLeft(bufnode_,lSibling,parent,parent_pos);
                //更新叶子信息
                bufnode_.WriteChunk();
                rSibling.WriteChunk();
                lSibling.WriteChunk();
                parent.WriteChunk();
            }else{
                //左边的邻居小于等于m/2,把当前节点合并到左邻居去
                leafMergeToLeft(bufnode_,lSibling,pos);
                //然后删除这个叶子结点
                deleteLeafNode(bufnode_,lSibling,rSibling);
                //随后向上更新
                innerNodeRemove(parent,parent_pos);
                //TODO 等会看看这里要不要把节点给写磁盘里
            }
        }else{
            //TODO 先删除,免得合并时溢出
            simpleLeafRemove(bufnode_,pos);
            if(rSibling.busy_ >= (rSibling.degree_ + 1)/2){
                //右边的孩子大过m/2,把右边元素的第一个元素借过来
                leafShiftFromRight(bufnode_,rSibling,parent,parent_pos);
                //更新叶子信息
                bufnode_.WriteChunk();
                rSibling.WriteChunk();
                lSibling.WriteChunk();
                parent.WriteChunk();
            }else{
                //把右边的节点合并过来
                leafMergeFromRight(bufnode_,rSibling);
                //随后向上更新
                int right_key_pos = parent_pos + 1;
                innerNodeRemove(parent,right_key_pos);
            }
        }
    }else{
        //正常删除就行
        simpleLeafRemove(bufnode_,pos);
        bufnode_.WriteChunk();
    }
    return res;
}
void BPTree::simpleLeafRemove(BPNode &node,int pos){
    node.data_.erase(node.dataLoc(pos),node.dataLoc(pos+1));
    node.busy_--;
}

void BPTree::leafShiftFromLeft(BPNode &leaf,BPNode &left_sibling,BPNode &parent,int parent_pos){
    //把leaf中的节点整体向后移动一个单位,把left_sibling的最后一个元素移动到leaf的第一个元素
    //std::copy_backward(leaf.dataLoc(0),leaf.dataLoc(leaf.busy_),leaf.dataLoc(1));
    leaf.data_.insert(leaf.dataLoc(0),left_sibling.dataLoc(left_sibling.busy_-1),left_sibling.dataLoc(left_sibling.busy_));
    left_sibling.busy_--;
    left_sibling.data_.resize(left_sibling.busy_*left_sibling.data_size_);
    //把parent中的parent_pos的元素的key值更新为leaf的第一个元素的key值
    parent.setKey(parent_pos,leaf.getKey(0));
}


void BPTree::leafShiftFromRight(BPNode &leaf,BPNode &right_sibling,BPNode &parent,int parent_pos){
    //从右边借,把right_sibling的第一个元素移动到leaf的最后一个元素
    leaf.data_.insert(leaf.dataLoc(leaf.busy_),right_sibling.dataLoc(0),right_sibling.dataLoc(1));
    leaf.busy_++;
    //把右边的第一个元素删除
    right_sibling.data_.erase(right_sibling.dataLoc(0),right_sibling.dataLoc(1));
    right_sibling.busy_--;
    //把parent中的parent_pos的元素的key值更新为right_sibling的第一个元素的key值
    parent.setKey(parent_pos,right_sibling.getKey(0));
}

void BPTree::leafMergeToLeft(BPNode &leaf,BPNode &left_sibling,int remove_pos){
    //将leaf节点合并到left_sibling中
    left_sibling.data_.insert(left_sibling.dataEnd(),leaf.dataLoc(0),leaf.dataLoc(remove_pos));
    left_sibling.busy_ += remove_pos;
    if(leaf.dataLoc(remove_pos) != leaf.dataEnd()){
        left_sibling.data_.insert(left_sibling.dataEnd(),leaf.dataLoc(remove_pos + 1),leaf.dataEnd());
        left_sibling.busy_ += leaf.busy_ - remove_pos;
    }
}

void BPTree::leafMergeFromRight(BPNode &leaf,BPNode &right_sibling){
    //将right_sibling合并到leaf中
    leaf.data_.insert(leaf.dataEnd(),right_sibling.dataLoc(0),right_sibling.dataEnd());
    leaf.busy_ += right_sibling.busy_;
    //然后把right_sibling删除
    //先获取right_sibling的下一个
    BPNode r_next;
    r_next.ReadChunk(right_sibling.child_[1]);
    //然后把right_sibling的下一个节点的前一个节点指向leaf
    r_next.child_[0] = leaf.node_pos;
    leaf.child_[1] = r_next.node_pos;
    right_sibling.releaseChunk();
    leaf.WriteChunk();
    r_next.WriteChunk();
}


void BPTree::deleteLeafNode(BPNode &leaf,BPNode &left_sibling,BPNode &right_sibling){
    //把这个叶子节点删除掉
    //如果有左边的节点和右边的节点,就让左边的节点指向右边的节点
    if(left_sibling.busy_ != 0){
        if(right_sibling.busy_ != 0){
            left_sibling.child_[1] = leaf.child_[1];
            right_sibling.child_[0] = leaf.child_[0];
            leaf.releaseChunk();
        }else{
            //如果没有右边的节点,就让左边的节点指向leaf的下一个节点(应该是个非法值)
            left_sibling.child_[1] = leaf.child_[1];
            leaf.releaseChunk();
        }
    }else{
        if(right_sibling.busy_ != 0){
            //如果没有左边的节点,就让右边的节点指向leaf的上一个节点(应该是个非法值)
            right_sibling.child_[0] = leaf.child_[0];
            leaf.releaseChunk();
        }
        //TODO 可以考虑如何将这个脏页重新利用
    }
}

void BPTree::innerNodeRemove(BPNode &node,int pos){
    //内部节点的值删除,此处可参考B树
    if(node.father_ == INVALID_OFFSET){
        //这是个根节点
        if(node.busy_ == 1){
            //如果只有一个元素,就用左孩子来当新的根节点
            BPNode new_root;
            new_root.ReadChunk(node.child_[0]);
            new_root.father_ = INVALID_OFFSET;
            //让new_root覆盖掉原来的根节点
            new_root.node_pos = root_pos_;
            new_root.WriteChunk();
        }else{
            //如果不止一个元素,就直接删除
            simpleInnerRemove(node,pos);
            node.WriteChunk();
        }
    }else if(node.busy_ < (node.degree_ + 1)/2){
        //孩子节点的数量小于一半,需要进行调整
        //首先找到父节点
        BPNode parent;
        parent.ReadChunk(node.father_);
        //找到左右兄弟节点
        BPNode left_sibling,right_sibling;
        int parent_pos = binarySearch(parent,node.getKey(0));
        
        
        //下面决定是从左边借还是从右边借
        enum class BorrowFrom{
            LEFT,
            RIGHT
        };
        //判断逻辑:如果当前节点是父节点的第一个孩子,则从右边借;
        //如果当前节点是最后一个孩子,则从左边借;
        //如果都不是,哪边更多就从哪边借
        BorrowFrom borrow_from;
        if(parent_pos == -1){
            //从右边借
            borrow_from = BorrowFrom::RIGHT;
            right_sibling.ReadChunk(parent.child_[parent_pos + 1]);
        }else if(parent_pos == parent.busy_ - 1){
            //从左边借
            borrow_from = BorrowFrom::LEFT;
            left_sibling.ReadChunk(parent.child_[parent_pos - 1]);
        }else{
            left_sibling.ReadChunk(parent.child_[parent_pos - 1]);
            right_sibling.ReadChunk(parent.child_[parent_pos + 1]);
            //从左边借还是从右边借呢?
            if(left_sibling.busy_ > right_sibling.busy_){
                //从左边借
                borrow_from = BorrowFrom::LEFT;
            }else{
                //从右边借
                borrow_from = BorrowFrom::RIGHT;
            }
        }

        //下面开始借
        if(borrow_from == BorrowFrom::LEFT){
            //和左边的借
            if(left_sibling.busy_ >= (left_sibling.degree_ + 1)/2){
                //左边元素多,借一个过来
                innerShiftFromLeft(node,left_sibling,parent,parent_pos);
                //然后把相关节点刷写一遍
                node.WriteChunk();
                left_sibling.WriteChunk();
                parent.WriteChunk();
            }else{
                //左边元素不够,把当前节点向左边合并
                innerMergeToLeft(node,left_sibling,parent,parent_pos,pos);
                //然后把相关节点刷写一遍
                node.releaseChunk();
                left_sibling.WriteChunk();
                parent.WriteChunk();
                //向上递归
                innerNodeRemove(parent,parent_pos);
            }
        }else{
            //先删除元素,腾出空间
            simpleInnerRemove(node,pos);
            //和右边的借
            if(right_sibling.busy_ >= (right_sibling.degree_ + 1)/2){
                //右边元素多,借一个过来
                innerShiftFromRight(node,right_sibling,parent,parent_pos + 1);
                //然后把相关节点刷写一遍
                node.WriteChunk();
                right_sibling.WriteChunk();
                parent.WriteChunk();
            }else{
                //右边元素不够,把右边节点向当前节点合并
                innerMergeFromRight(node,right_sibling,parent,parent_pos + 1);
                //然后把相关节点刷写一遍
                node.WriteChunk();
                right_sibling.releaseChunk();
                parent.WriteChunk();
                //向上递归
                innerNodeRemove(parent,parent_pos + 1);
            }    
        }
    }else{
        //借啥借,直接删
        simpleInnerRemove(node,pos);
        node.WriteChunk();
    }
}

void BPTree::simpleInnerRemove(BPNode &node,int pos){
    //简单的删除内部节点的值
    //首先把pos后面的元素都往前移动一位
    node.data_.erase(node.dataLoc(pos),node.dataLoc(pos + 1));
    //键值删除了,对应的孩子也得删
    node.child_.erase(node.childLoc(pos + 1),node.childLoc(pos + 2));
    node.busy_--;
}

void BPTree::innerShiftFromLeft(BPNode &node,BPNode &left_sibling,BPNode &parent,int parent_pos){
    //从左边借一个元素过来
    //首先把父节点的值插入到当前节点的最前面
    node.data_.insert(node.dataBegin(),parent.dataLoc(parent_pos),parent.dataLoc(parent_pos + 1));
    node.busy_++;
    //然后把左边节点的最后一个元素插入到父节点的位置
    parent.data_.erase(parent.dataLoc(parent_pos),parent.dataLoc(parent_pos + 1));
    parent.data_.insert(parent.dataLoc(parent_pos),left_sibling.dataLoc(left_sibling.busy_ - 1),left_sibling.dataLoc(left_sibling.busy_));
    //然后把左边节点的最后一个孩子节点删掉
    left_sibling.child_.erase(left_sibling.childLoc(left_sibling.busy_),left_sibling.childLoc(left_sibling.busy_ + 1));
    left_sibling.busy_--;
}

void BPTree::innerShiftFromRight(BPNode &node,BPNode &right_sibling,BPNode &parent,int parent_pos){
    //从右边借一个元素过来
    //首先把父节点的值插入到当前节点的最后面
    node.data_.insert(node.dataLoc(node.busy_),parent.dataLoc(parent_pos),parent.dataLoc(parent_pos + 1));
    node.busy_++;
    //然后把右边节点的第一个元素插入到父节点的位置
    parent.data_.erase(parent.dataLoc(parent_pos),parent.dataLoc(parent_pos + 1));
    parent.data_.insert(parent.dataLoc(parent_pos),right_sibling.dataLoc(0),right_sibling.dataLoc(1));
    //然后把右边节点的第一个孩子节点删掉
    right_sibling.child_.erase(right_sibling.childLoc(0),right_sibling.childLoc(1));
    right_sibling.busy_--;
}

void BPTree::innerMergeToLeft(BPNode &leaf,BPNode &left_sibling,BPNode &parent,int parent_pos,int remove_pos){
    //把当前节点和左边节点合并
    //首先把父节点的值插入到左边节点的最后面
    left_sibling.data_.insert(left_sibling.dataEnd(),parent.dataLoc(parent_pos - 1),parent.dataLoc(parent_pos));
    left_sibling.busy_++;
    //合并到左节点去(除了被删除的那个)
    left_sibling.data_.insert(left_sibling.dataEnd(),leaf.dataLoc(0),leaf.dataLoc(remove_pos));
    left_sibling.child_.insert(left_sibling.childEnd(),leaf.childLoc(0),leaf.childLoc(remove_pos + 1));
    left_sibling.busy_ += remove_pos;
    left_sibling.data_.insert(left_sibling.dataEnd(),leaf.dataLoc(remove_pos + 1),leaf.dataLoc(leaf.busy_));
    left_sibling.child_.insert(left_sibling.childEnd(),leaf.childLoc(remove_pos + 2),leaf.childLoc(leaf.busy_ + 1));
    left_sibling.busy_ += (leaf.busy_ - remove_pos - 1);
    //将孩子节点的父亲给更改了(刚被合并过来的部分)
    for(auto it = left_sibling.childLoc(left_sibling.busy_ - (leaf.busy_ - remove_pos - 1));it != left_sibling.childLoc(left_sibling.busy_);it++){
        BPNode child;
        child.ReadChunk(*it);
        child.father_ = left_sibling.node_pos;
        child.WriteChunk();
    }
}

void BPTree::innerMergeFromRight(BPNode &leaf,BPNode &right_sibling,BPNode &parent,int parent_pos){
    //把右边节点合并到当前节点
    //首先把父节点的值插入到当前节点的最后面
    leaf.data_.insert(leaf.dataEnd(),parent.dataLoc(parent_pos),parent.dataLoc(parent_pos + 1));
    leaf.busy_++;
    //合并到当前节点去
    leaf.data_.insert(leaf.dataEnd(),right_sibling.dataLoc(0),right_sibling.dataLoc(right_sibling.busy_));
    leaf.child_.insert(leaf.childEnd(),right_sibling.childLoc(0),right_sibling.childLoc(right_sibling.busy_ + 1));
    leaf.busy_ += right_sibling.busy_;
    //将孩子节点的父亲给更改了
    for(auto it = leaf.childLoc(leaf.busy_ - right_sibling.busy_);it != leaf.childLoc(leaf.busy_);it++){
        BPNode child;
        child.ReadChunk(*it);
        child.father_ = leaf.node_pos;
        child.WriteChunk();
    }

}

void BPTree::subNodeUpdateParent(BPNode &node,streampos parent_pos){
    node.father_ = parent_pos;
}
uint8 BPTree::getKeyType(){
    return key_type_;
}