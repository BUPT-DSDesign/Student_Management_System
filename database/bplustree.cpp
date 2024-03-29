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
#include <chrono>
using namespace std;
const filepos INVALID_OFFSET = -1;
BPNode::~BPNode(){
    //析构函数
    fp->close();
}
BPNode::BPNode(string filename)
:file_name_(filename),is_leaf_(false),is_dirty_(false),key_type_(0),degree_(0),busy_(0),child_cnt_(0),data_size_(0),key_size_(0),father_(INVALID_OFFSET),node_pos(INVALID_OFFSET),fp(make_shared<fstream>(filename,ios::in|ios::out|ios::binary))
{
    if(!fp->is_open()){
        fp->open(filename,ios::out|ios::binary);
        if(!fp->is_open()){
            throw BPNodeException(filename+"文件打开失败");
        }
        fp->open(filename,ios::in|ios::out|ios::binary);
    }
    //初始化vector大小
    int max_space = PAGE_SIZE - sizeof(BPNodeHead);//最大的空闲空间
    //假设每个data最小为1字节
    int max_degree = (max_space-sizeof(filepos))/(sizeof(filepos)+1)+1;
    child_ = vector<filepos>();
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
filepos BPNode::getChild(int id){
    if(id < 0){
        return -1;
    }
    if(id < child_.size())
        return child_[id];
    return INVALID_OFFSET;
}
void BPNode::ReadChunk(filepos pos){
    if(pos == INVALID_OFFSET){
        //如果是无效的偏移量,则不读取
        return;
    }
    /*if(pos == node_pos){
        //如果是当前节点,则不读取
        return;
    }*/
    BPNodeHead head;
    //将一个区块读取到字节流中
    //1.打开文件读写流
    //fstream fp(file_name_,ios::binary|ios::in);
    //cerr << "Reading Chunk,file_name_:" << file_name_ <<" "<<pos<< endl;
    fp->seekg(pos,ios::beg);
    if(!fp->eof()){
        //如果没有读写到文件末尾
        //第一个部分,结构体头
        fp->read((char*)&head,sizeof(head));
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
        if(is_dirty_){
            //脏页,需要重新读取
            if(pos != 0){
                throw BPNodeException("BPNode is dirty,can't read");
            }else{
                is_dirty_ = false;
            }
        }
        //第二个部分,读取节点数据
        //整体结构为 指针0 | 数据1 | 指针1 | ... | 数据n | 指针n 
        //如果是叶子节点的话,仅在开始和结束有指针
        data_.clear();
        child_.clear();
        filepos child_p;
        if(head.is_leaf_){
            //如果是叶子节点,读出前一个节点
            fp->read((char*)&child_p,sizeof(child_p));
            child_.push_back(child_p);
        }
        for(int i=0;i<head.busy_;i++){
            //如果不是叶节点,才有child
            if(!head.is_leaf_){
                fp->read((char*)&child_p,sizeof(child_p));
                child_.push_back(child_p);
            }
            //读取数据
            vector<byte> tmp(head.data_size_+head.key_size_);
            fp->read((char*)tmp.data(),head.data_size_+head.key_size_);
            data_.insert(data_.end(),tmp.begin(),tmp.end());
        }
        //如果不是叶子结点,这里记录最后一个孩子
        //如果是叶子节点,这里记录下一个元素位置
        fp->read((char*)&child_p,sizeof(child_p));
        child_.push_back(child_p);
    }
    //读取完毕,关闭文件读写流
    //fp->close();
}
void BPNode::WriteChunk(){
    //将节点按格式写入硬盘
    //1.打开文件读写流
    //fstream fp(file_name_,ios::binary|ios::in|ios::out);
    fp->seekp(node_pos,ios::beg);
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
    fp->write((char*)&head_,sizeof(head_));
    //第二部分,写入节点数据
    if(head_.is_leaf_){
        //如果是叶子节点,写出前一个节点
         fp->write((char*)&child_[0],sizeof(child_[0]));
    }
    for(int i=0;i<head_.busy_;i++){
        if(!head_.is_leaf_)
            //如果不是叶子节点的话才有child
            fp->write((char*)&child_[i],sizeof(child_[i]));
        fp->write((char*)&data_[getElemLocInData(i)],data_size_+key_size_);
    }
    if(!head_.is_leaf_)
        fp->write((char*)&child_[head_.busy_],sizeof(child_[head_.busy_]));
    else
        //如果是leaf节点,则仅有一个,指向下一个节点
        fp->write((char*)&child_[1],sizeof(child_[1]));
    //写入完毕,刷写文件写入流
    fp->flush();
}
void BPNode::CreateChunk(bool is_leaf,int data_size,uint16 key_size,uint8 key_type,filepos &end_pos)
{
    //0.计算出头信息
    father_ = INVALID_OFFSET;
    is_dirty_ = false;
    is_leaf_ = is_leaf;
    data_size_ = data_size;
    key_type_ = key_type;
    key_size_ = key_size;
    busy_ = 0;
    child_cnt_ = 0;
    if(is_leaf){
        //如果是叶,新建时把一前一后俩指针给初始化了
        degree_ = (PAGE_SIZE-sizeof(BPNodeHead)-sizeof(filepos)*2)/(data_size+key_size);
        child_.push_back(INVALID_OFFSET);
        child_.push_back(INVALID_OFFSET);
        child_cnt_ = 2;
    }else{
        degree_ = (PAGE_SIZE-sizeof(BPNodeHead)-sizeof(filepos))/(sizeof(filepos)+data_size+key_size);
    }
    busy_ = 0;
    //去系统中要一块新内存写入
    //1.获取新页位置
    //此处可改为获取一块脏页
    
    //fstream fp(file_name_,ios::in|ios::out|ios::binary);
    if(fp->tellp()!=INVALID_OFFSET){
        fp->seekp(end_pos,ios::beg);
        node_pos = end_pos;
    }else{
        //新建文件
        end_pos = 0;
        node_pos = 0;
        fp->open(file_name_,ios::out|ios::binary);
    }
    
    //2.填充头信息
    BPNodeHead head_ = {is_leaf_,is_dirty_,key_type_,key_size_,degree_,busy_,data_size_,father_,node_pos};
    //如果当前是文件末尾,则给文件预填充一段16MB的空间
    if(fp->peek()==EOF){
        fp->close();
        fp->open(file_name_,ios::binary|ios::in|ios::out);
        fp->seekp(end_pos,ios::beg);
        char data[PAGE_SIZE] = {0};
        for(int i=0;i<1024;i++){
            fp->write(data,PAGE_SIZE);
        }
        fp->close();
        fp->open(file_name_,ios::binary|ios::in|ios::out);
        fp->seekp(end_pos,ios::beg);
    }
    fp->write((char*)&head_,sizeof(head_));
    end_pos += PAGE_SIZE;
    fp->flush();
}
filepos BPNode::releaseChunk(){
    //从磁盘中卸载当前块
    //1.打开文件读写流
    //fstream fp(file_name_,ios::binary|ios::in|ios::out);
    fp->seekp(node_pos,ios::beg);
    //2.将其标记为脏页
    is_dirty_ = true;
    //3.写入
    BPNodeHead head_ = {is_dirty_,is_leaf_,key_type_,key_size_,degree_,busy_,data_size_,father_,node_pos};
    fp->write((char*)&head_,sizeof(head_));
    //4.刷写文件读写流
    fp->flush();
    return node_pos;
}
vector<byte> BPNode::getRawData(int id){
    //获取第id个元素的原始数据
    vector<byte> tmp(data_size_);
    uint16 pos = getElemLocInData(id);
    std::copy(data_.begin()+pos+key_size_,data_.begin()+pos+data_size_+key_size_,tmp.begin());
    return tmp;
}
void BPNode::setKey(int id,const Key &key){
    if(id < 0){
        throw BPNodeException("func setKey:id must be positive");
    }
    //设置第id个元素的key
    uint16 pos = getElemLocInData(id);
    vector<byte> tmp_key = key.getBytes();
    std::copy(tmp_key.begin(),tmp_key.end(),data_.begin()+pos);
}
uint16 BPNode::getElemLocInData(int id){
    return id*(data_size_+key_size_);
}
void BPNode::updateDataAtPos(int id,const Key &key,const vector<byte>& data){
    if(id < 0){
        throw BPNodeException("func updateDataAtPos:id must be positive");
    }
    //先拿到写入位置
    uint16 pos = getElemLocInData(id);
    //随后开始写入
    //前8个字节为key,后面为data
    vector<byte> tmp;
    //先把key拷贝进去
    vector<byte> tmp_key = key.getBytes();
    tmp.insert(tmp.end(),tmp_key.begin(),tmp_key.end());
    //再把data拷贝进去
    tmp.insert(tmp.end(),data.begin(),data.end());
    //最后写入data_
    std::copy(tmp.begin(),tmp.end(),data_.begin()+pos);
}
void BPNode::insertDataAtPos(int id,const Key &key,const vector<byte>& data){
    if(id < 0){
        throw BPNodeException("func insertDataAtPos:id must be positive");
    }
    //先拿到写入位置
    uint16 pos = getElemLocInData(id);
    //随后开始写入
    //前8个字节为key,后面为data
    vector<byte> tmp;
    //先把key拷贝进去
    vector<byte> tmp_key = key.getBytes();
    tmp.insert(tmp.end(),tmp_key.begin(),tmp_key.end());
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
    uint16 key_shift = key_size_;
    if(id*(data_size_+key_shift) <= data_.size() && id >= 0){
        return data_.begin()+id*(data_size_+key_shift);
    }
    return data_.begin()+busy_*(data_size_+key_shift);
}
std::vector<filepos>::iterator BPNode::childBegin(){
    return child_.begin();
}
std::vector<filepos>::iterator BPNode::childEnd(){
    return child_.end();
}
std::vector<filepos>::iterator BPNode::childLoc(int id){
    if(id <= busy_ && id >= 0){
        return child_.begin()+id;
    }
    if(id < 0)
        return child_.begin();
    return childEnd();
}
Key BPNode::getKey(int id){
    //TODO id可能为-1
    if(id < 0)
        return Key(true);
    Key key(dataLoc(id),key_size_,key_type_);
    return key;
}

//下面是B+树的实现
BPTree::BPTree(string path,bool is_table):bufnode_(path)
{
    is_table_ = is_table;
    //bufnode_.file_name_ = path;
    //如果没有文件,则创建一个文件
    if(!std::filesystem::exists(bufnode_.file_name_)){
        throw BPTreeException("File Do not Exist!");
    }
    //加载第一页
    bufnode_.ReadChunk(0);
    cur_ = 0;
    //寻找父亲节点
    while(bufnode_.father_ != -1){
        bufnode_.ReadChunk(bufnode_.father_);
        cur_ = bufnode_.node_pos;
    }
    root_pos_ = cur_;
    size_of_item_ = bufnode_.data_size_;
    key_type_ = bufnode_.key_type_;
    //确认文件大小
    std::ifstream in(bufnode_.file_name_,std::ios::binary);
    //读取头部信息,直到头部信息为空或读到文件末尾,此时方可确认文件大小
    int page_num = 0;
    while(!in.eof()){
        BPNodeHead head;
        char buffer[PAGE_SIZE];
        in.read(buffer,PAGE_SIZE);
        std::copy(buffer,buffer+sizeof(head),(char*)&head);
        //如果head为空,则说明已经读到文件末尾
        if(in.eof() || head.chunk_pos_ != page_num * PAGE_SIZE)
            break;
        page_num++;
    }
    key_size_ = bufnode_.key_size_;
    end_pos_ = page_num*PAGE_SIZE;
    in.close();
}
BPTree::BPTree(string path,bool is_table,int data_size,uint16 key_size,uint8 key_type)
:is_table_(is_table),size_of_item_(data_size),key_type_(key_type),cur_(0),root_pos_(0),end_pos_(0),key_size_(key_size),bufnode_(path)
{
    //bufnode_.file_name_ = path;
    //如果已经新建过了,就报错
    if(!std::filesystem::exists(bufnode_.file_name_)){
        throw BPTreeException("File Do not Exist!");
    }
    //如果是表,则需要初始化一些属性
    bufnode_.CreateChunk(true,data_size,key_size,key_type,end_pos_);
    bufnode_.WriteChunk();
    //初始化BPTree类

}
BPTree::~BPTree(){
    
}
bool BPTree::isBufLeaf(){
    if(cur_ == INVALID_OFFSET)
        return false;
    return bufnode_.isLeaf();
}
//核心算法1:二分查找
int BPTree::binarySearch(BPNode &node,const Key &key){
    int l=-1,r=node.getElemCount() - 1;
    while(l + 1 < r){
        int mid = (l+r) >> 1;
        Key node_key = node.getKey(mid);
        if(key <= node_key){
            r = mid;
        }else{
            l = mid;
        }
    }
    return r;
}
void BPTree::searchLeaf(const Key &key){
    cur_ = root_pos_;
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
        //得到孩子的位置,接下来决定选哪边的孩子
        if(bufnode_.getKey(child_id) > key){
            //如果孩子的key比要查找的键值大,那么就取左边的孩子
            cur_ = bufnode_.getChild(child_id);
        }else{
            //否则取右边的
            cur_ = bufnode_.getChild(child_id+1);
        }
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
    if(pos < 0 || bufnode_.getKey(pos) != key){
        return vector<byte>{};//如果没找到,返回空vector
    }
    //第三步,读取对应数据,返回字节流供解析
    vector<byte> elem = bufnode_.getRawData(pos);
    return elem;
}
filepos BPTree::SearchPos(const Key &key){
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
    while(pos < bufnode_.busy_ &&bufnode_.getKey(pos) <= right){
        result.push_back(bufnode_.getRawData(pos));
        pos++;
        if(pos == bufnode_.busy_ && bufnode_.child_[1] != INVALID_OFFSET){
            //如果当前块已经读完,则读取下一个块
            cur_ = bufnode_.child_[1];
            bufnode_.ReadChunk(cur_);
            pos = 0;
        }
    }
    return result;
}

vector<Key> BPTree::Insert(const Key &key,vector<byte> &data){
    cur_ = root_pos_;
    //先搜索这个叶子节点
    searchLeaf(key);
    //接着判断键值是否重复
    if(is_table_){
        int pos = binarySearch(bufnode_,key);
        if(pos >= 0 &&key == bufnode_.getKey(pos)){
            //键值重复,无法插入(表的主键不能重复)
            throw BPTreeException("Key Duplicate!");
            return vector<Key>{};
        }
    }
    
    
    if(bufnode_.busy_==bufnode_.degree_){
        //如果节点满了,需要分裂
        return splitTreeNode(key,data);
        //把旧的根节点作为新的根节点的孩子
    }else{
        //如果节点没有满,直接添加
        insertNoSplit(bufnode_,key,data);
        bufnode_.WriteChunk();
    }
    return vector<Key>{};
}
void BPTree::insertKey(const Key &key,const filepos &old,const filepos &after){
    if(cur_==-1){
        //新建一个根节点
        BPNode root(bufnode_.file_name_);
        //root.file_name_ = bufnode_.file_name_;
        //非叶子节点,节点单元素大小为streampos大小
        root.CreateChunk(false,0,bufnode_.key_size_,bufnode_.key_type_,end_pos_);
        //节点只有一个,即键值
        root.busy_ = 1;
        //将值写入
        vector<byte> key_bytes = key.getBytes();
        root.data_.insert(root.data_.begin(),key_bytes.begin(),key_bytes.end());
        //然后old为第一个孩子,after为第二个孩子(正好一前一后)
        root.child_.push_back(old);
        root.child_.push_back(after);
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
    BPNode inner_node(bufnode_.file_name_);
    //inner_node.file_name_ = bufnode_.file_name_;
    inner_node.ReadChunk(cur_);
    if(inner_node.busy_ == inner_node.degree_){
        //满啦,需要分裂
        BPNode new_node(bufnode_.file_name_);
        //new_node.file_name_ = bufnode_.file_name_;
        new_node.CreateChunk(false,0,bufnode_.key_size_,bufnode_.key_type_,end_pos_);
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
        new_node.child_ = vector<filepos>(inner_node.childLoc(pos+1),inner_node.childEnd());
        //然后修改两个节点的busy
        new_node.busy_ = inner_node.busy_ - pos - 1;
        inner_node.busy_ = pos + 1;
        //然后选一个位置放下吧
        vector<byte> child_bytes;
        child_bytes.resize(sizeof(filepos));
        std::copy((byte*)&after,(byte*)&after+sizeof(filepos),child_bytes.begin());
        if(place_right){
            insertNoSplit(new_node,key,child_bytes);
        }else{
            insertNoSplit(inner_node,key,child_bytes);
        }
        //保存修改过的两个区块
        new_node.father_ = inner_node.father_;
        new_node.WriteChunk();
        inner_node.WriteChunk();
        resetIndexChildrenParent(new_node);
        //然后把键值向上插入
        cur_ = inner_node.father_;
        insertKey(new_node.getKey(0),inner_node.node_pos,new_node.node_pos);
    }else{
        //不需要拓,直接插入
        //将after插入到inner_node中
        vector<byte> child_bytes;
        child_bytes.resize(sizeof(filepos));
        std::copy((byte*)&after,(byte*)&after+sizeof(filepos),child_bytes.begin());
        insertNoSplit(inner_node,key,child_bytes);
        //inner_node.child_.push_back(after);
        inner_node.WriteChunk();
    }
}
BPNodeHead BPTree::readNodeHead(filepos pos){
    fstream file(bufnode_.file_name_,ios::binary|ios::in|ios::out);
    BPNodeHead tmpHeader;
    file.seekg(pos,ios::beg);
    file.read((char*)&tmpHeader,sizeof(BPNodeHead));
    file.close();
    return tmpHeader;
}
void BPTree::writeNodeHead(const BPNodeHead &head,filepos pos){
    fstream file(bufnode_.file_name_,ios::binary|ios::in|ios::out);
    file.seekp(pos,ios::beg);
    file.write((char*)&head,sizeof(BPNodeHead));
    file.close();
}
vector<Key> BPTree::splitTreeNode(const Key &key,vector<byte> &data){
    //首先需要一分为二,故先创建一个邻居
    BPNode new_node(bufnode_.file_name_);
    //new_node.file_name_ = bufnode_.file_name_;
    //此处新建的为叶子节点
    new_node.CreateChunk(true,size_of_item_,bufnode_.key_size_,bufnode_.key_type_,end_pos_);
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
    new_node.data_.insert(new_node.data_.begin(),bufnode_.dataLoc(pos),bufnode_.dataEnd());
    //new_node.data_ = vector<byte>(bufnode_.dataLoc(pos),bufnode_.dataEnd());
    //把new_node中data的键值提取出来,加到返回值中
    vector<Key> ret;
    for(int i = 0;i < new_node.busy_;++i){
        ret.push_back(new_node.getKey(i));
    }
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
    bufnode_.child_[1] = new_node.node_pos;
    
    
    //更新父亲节点
    new_node.father_ = bufnode_.father_;
    //随后把键值向上插入
    cur_ = bufnode_.father_;
    //保存修改后的两个节点
    new_node.WriteChunk();
    bufnode_.WriteChunk();
    //bufnode_ = new_node;
    insertKey(new_node.getKey(0),bufnode_.node_pos,bufnode_.child_[1]); 
    return ret;
}

void BPTree::insertNoSplit(BPNode &node,const Key &key,const vector<byte> &data){
    //先用二分查找定位
    int pos = binarySearch(node,key);
    while(pos == -1 || (pos < node.busy_ && node.getKey(pos) < key)){
        //如果没找到,或者找到了一个比key小的值,则继续向后找
        pos++;
    }
    //插入的位置应该是pos+1
    node.busy_++;
    //增加busy数量
    if(!node.isLeaf()){
        //如果不是叶子节点,则data里存的是filepos
        //需要插入到child里
        filepos after;
        std::copy(data.begin(),data.end(),(byte*)&after);
        node.child_.insert(node.child_.begin()+pos+1,after);
        vector<byte> empty;
        node.insertDataAtPos(pos,key,empty);
    }else{
        //叶子节点,正常插入
        node.insertDataAtPos(pos,key,data);
    }
    
}
void BPTree::resetIndexChildrenParent(BPNode &node){
    //简简单单更新下所有节点的父亲节点
    for(auto it=node.childBegin();it!=node.childEnd();it++){
        //只更新头部就行
        BPNodeHead tmpHeader = readNodeHead(*it);
        if(tmpHeader.father_ != node.node_pos){
            tmpHeader.father_ = node.node_pos;
            writeNodeHead(tmpHeader,*it);
        }
    }
}
filepos BPTree::GetChunkPos(){
    return bufnode_.node_pos;
}
void BPTree::ReadChunk(filepos pos){
    bufnode_.ReadChunk(pos);
    cur_ = pos;
}
void BPTree::ReadFirstChunk(){
    //即读取最小的那个叶子节点
    //如果没初始化父亲节点,就先找到父亲节点
    if(root_pos_ == INVALID_OFFSET){
        cur_ = 0;
        bufnode_.ReadChunk(cur_);
        while(bufnode_.father_ != INVALID_OFFSET){
            cur_ = bufnode_.father_;
            bufnode_.ReadChunk(cur_);
        }
        root_pos_ = cur_;
    }
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
        if(bufnode_.child_.size() == 0){
            //如果没有孩子,则直接返回
            cur_ = INVALID_OFFSET;
            return;
        }
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
        if(cur_ == INVALID_OFFSET){
            //如果已经是最后一个了,就不用再读了
            bufnode_.is_leaf_ = false;
            return;
        }
        bufnode_.ReadChunk(cur_);
    }
}

vector<vector<byte>> BPTree::GetAllElemInChunk(){
    //获取当前节点的所有元素
    vector<vector<byte>> res;
    if(bufnode_.isLeaf()){
        for(auto it=bufnode_.dataBegin();it!=bufnode_.dataEnd();it+=(bufnode_.data_size_+bufnode_.key_size_)){
            vector<byte> elem(it+bufnode_.key_size_,it+bufnode_.key_size_+bufnode_.data_size_);
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
        bufnode_.updateDataAtPos(pos,key,data);
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
    vector<Key> useless;
    return Remove(key,useless);
}
vector<byte> BPTree::Remove(const Key &key,vector<Key> &adjust_keys){
    //删除
    cur_ = root_pos_;
    //第一步,定位到叶子节点
    searchLeaf(key);
    //此时叶子结点已经载入到内存中,进行二分查找返回数据即可
    //第二步,二分查找数据
    int pos = binarySearch(bufnode_,key);
    if(pos == -1 || key != bufnode_.getKey(pos)){
        //找不到这样的键值
        return vector<byte>();
    }
    //如果找到了,则删除
    //先获取数据
    vector<byte> res = bufnode_.getRawData(pos);
    
    if(bufnode_.father_ == -1){
        //当前节点为根节点
        //正常删除就行
        simpleLeafRemove(bufnode_,pos);
        bufnode_.WriteChunk();
    }else if(bufnode_.busy_ < (bufnode_.degree_ + 1)/2){
        //如果删除之后,节点的元素个数小于等于阶数的一半,则需要进行调整操作
        BPNode parent(bufnode_.file_name_),lSibling(bufnode_.file_name_),rSibling(bufnode_.file_name_);
        //parent.file_name_ = bufnode_.file_name_;
        //lSibling.file_name_ = bufnode_.file_name_;
        //rSibling.file_name_ = bufnode_.file_name_;
        parent.ReadChunk(bufnode_.father_);
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
        //开借
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
            }
        }else{
            //先删除,免得合并时溢出
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
        //最后,把涉及到的更新过的键值都返回
        //先返回左边的那个节点
        for(int i=0;i<lSibling.busy_;i++){
            adjust_keys.push_back(lSibling.getKey(i));
        }
        //再返回右边的那个节点
        for(int i=0;i<rSibling.busy_;i++){
            adjust_keys.push_back(rSibling.getKey(i));
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
    BPNode r_next(right_sibling.file_name_);
    //r_next.file_name_ = right_sibling.file_name_;
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
            BPNode new_root(node.file_name_);
            //new_root.file_name_ = node.file_name_;
            new_root.ReadChunk(node.child_[0]);
            new_root.father_ = INVALID_OFFSET;
            //让new_root放到0位置
            new_root.node_pos = 0;
            new_root.WriteChunk();
        }else{
            //如果不止一个元素,就直接删除
            simpleInnerRemove(node,pos);
            node.WriteChunk();
        }
    }else if(node.busy_ < (node.degree_ + 1)/2){
        //孩子节点的数量小于一半,需要进行调整
        //首先找到父节点
        BPNode parent(node.file_name_);
        //parent.file_name_ = node.file_name_;
        parent.ReadChunk(node.father_);
        //找到左右兄弟节点
        BPNode left_sibling(node.file_name_),right_sibling(node.file_name_);
        //left_sibling.file_name_ = node.file_name_;
        //right_sibling.file_name_ = node.file_name_;
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
        BPNode child(bufnode_.file_name_);
        //child.file_name_ = bufnode_.file_name_;
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
        BPNode child(bufnode_.file_name_);
        //child.file_name_ = bufnode_.file_name_;
        child.ReadChunk(*it);
        child.father_ = leaf.node_pos;
        child.WriteChunk();
    }

}

void BPTree::subNodeUpdateParent(BPNode &node,filepos parent_pos){
    node.father_ = parent_pos;
}
uint8 BPTree::getKeyType(){
    return key_type_;
}
uint16 BPTree::getKeySize(){
    return bufnode_.key_size_;
}