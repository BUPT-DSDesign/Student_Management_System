#include "table.hpp"
#include "MyException.hpp"
#include <fstream>
#include <algorithm>
#include <sstream>
#include <filesystem>
using namespace std;
//默认构造函数
Table::Table():col_cnt_(0),index_cnt_(0),record_length_(0){}
//拷贝构造函数
Table::Table(const Table& tb):col_cnt_(tb.col_cnt_),index_cnt_(tb.index_cnt_),record_length_(tb.record_length_),table_name_(tb.table_name_),db_path_(tb.db_path_),col_info_(tb.col_info_),col_shift_(tb.col_shift_),index_name_(tb.index_name_),col2index_(tb.col2index_),col2id_(tb.col2id_){
    //重新打开表的数据文件
    string fileData = db_path_+"/"+table_name_+".table";
    tb_data_ = make_unique<BPTree>(fileData);
}
//打开一张已有的表,将表头信息读入内存
Table::Table(const string& db_path,const string& table_name):table_name_(table_name),db_path_(db_path){
    //首先打开表的配置文件,其文件路径为db_path/table_name.tbinfo
    string fileInfo = db_path_+"/"+table_name_+".tbinfo";
    unique_ptr<fstream> table_info_ = make_unique<fstream>(fstream(fileInfo.data(),ios::in|ios::out|ios::binary));
    if(!table_info_->is_open()){
        throw TableOpenError("Can't open table info:"+fileInfo);
    }
    //读取表头信息
    //1.读取列的数量
    table_info_->read(reinterpret_cast<char*>(&col_cnt_),sizeof(uint16));
    //一共有col_cnt_列,每一列的信息为TableColAttribute
    for(int i = 0;i < col_cnt_;i++){
        TableColAttribute tmp;
        //读取每一列的信息,即TableColAttribute中除了default_和comment_的部分
        table_info_->read(reinterpret_cast<char*>(&tmp),sizeof(TableColAttribute)-sizeof(any)-sizeof(string));
        //读取default_和comment_
        if(tmp.default_length_!=0){
            //如果有默认值,则读取默认值
            char buffer[PAGE_SIZE]={0};//输出缓冲
            table_info_->read(reinterpret_cast<char*>(buffer),tmp.default_length_);
            tmp.default_ = string_view(buffer,tmp.default_length_);
        }
        if(tmp.comment_length_!=0){
            //如果有注释,则读取注释
            char buffer[65535]={0};//输出缓冲
            table_info_->read(reinterpret_cast<char*>(buffer),tmp.comment_length_);
            tmp.comment_ = string_view(buffer,tmp.comment_length_);
        }
        //将读取到的信息放入col_info_中
        col_info_.push_back(tmp);
        //将列名和列id的映射关系放入col2id_中
        col2id_[tmp.col_name_] = i;
    }
    //计算每一列的偏移量
    uint16 shift = 0;
    for(int i = 0;i < col_cnt_;i++){
        col_shift_.push_back(shift);
        shift += col_info_[i].length_;
    }
    //计算每一条记录的长度
    record_length_ = shift;
    //随后读取索引的数量
    table_info_->read(reinterpret_cast<char*>(&index_cnt_),sizeof(uint16));
    //读取索引信息,格式同IndexAttribute
    for(int i = 0;i < index_cnt_;i++){
        IndexAttribute tmp;
        table_info_->read(reinterpret_cast<char*>(&tmp),sizeof(IndexAttribute));
        index_name_.push_back(tmp.index_name_);
        col2index_[col_info_[tmp.col_id_].col_name_] = tmp.index_name_;
    }
    //最后打开表的数据文件,其文件路径为db_path/table_name.table,
    string fileData = db_path_+"/"+table_name_+".table";
    tb_data_ = make_unique<BPTree>(fileData);
    //如果有索引文件,一并将其加载入内存
    for(auto &it:index_name_){
        string fileIndex = db_path_+"/"+it+".idx";
        tb_index_[it] = make_unique<BPTree>(fileIndex);
    }
}

//新建一张表,将表头信息写入文件
Table::Table(const string& db_path,const string& table_name,vector<TableColAttribute> &col_info){
    //先确认有没有同名文件
    string fileInfo = db_path_+"/"+table_name+".tbinfo";
    if(filesystem::exists(fileInfo)){
        throw TableCreateError("Table "+table_name+" already exists");
    }
    //确认没有同名文件后,创建文件
    unique_ptr<fstream> table_info_ = make_unique<fstream>(fstream(fileInfo.data(),ios::in|ios::out|ios::binary|ios::trunc));
    if(!table_info_->is_open()){
        throw TableCreateError("Can't create table info:"+fileInfo);
    }
    //写入列的数量
    col_cnt_ = col_info.size();
    table_info_->write(reinterpret_cast<char*>(&col_cnt_),sizeof(uint16));
    //写入列的信息
    for(auto &it:col_info){
        //写入除了default_和comment_的部分
        table_info_->write(reinterpret_cast<char*>(&it),sizeof(TableColAttribute)-sizeof(any)-sizeof(string));
        //写入default_和comment_
        if(it.default_length_!=0){
            //如果有默认值,则写入默认值
            //根据数据类型,将any转换为对应的数据类型
            istringstream iss(any_cast<string_view>(it.default_).data());
            switch(it.data_type_){
                //1.数值类型:整型(符号型)
                case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
                    {
                        int64 val_int64;
                        val_int64 = any_cast<int64>(it.default_);
                        table_info_->write(reinterpret_cast<char*>(&val_int64),it.default_length_);
                    }
                    break;
                //2.数值类型:浮点型(单精度)
                case T_FLOAT:
                    {
                        float val_float;
                        val_float = any_cast<float>(it.default_);
                        table_info_->write(reinterpret_cast<char*>(&val_float),it.default_length_);
                    }
                    break;
                //3.数值类型:浮点型(双精度)
                case T_DOUBLE:
                    {
                        double val_double;
                        val_double = any_cast<double>(it.default_);
                        table_info_->write(reinterpret_cast<char*>(&val_double),it.default_length_);
                    }
                    break;
                //4.数值类型:无符号类型(日期和时间类型),传入时为字符串类型,需要转换为uint64
                case T_DATE:
                    //格式为YYYY-MM-DD
                    //范围为1000-01-01到9999-12-31
                    {
                        uint64 val_uint64;
                        uint64 year,month,day;
                        char c;
                        iss>>year>>c>>month>>c>>day;
                        //判断输入格式是否正确
                        if(iss.fail()){
                            throw TableCreateError("Type Date Error:Date format error");
                        }
                        //判断是否在范围里,不在则抛出异常
                        if(year<1000||year>9999||month<1||month>12||day<1||day>31){
                            throw TableCreateError("Type Date Error:Date out of range");
                        }
                        val_uint64 = (year<<9ULL)|(month<<5ULL)|day;
                        table_info_->write(reinterpret_cast<char*>(&val_uint64),it.default_length_);
                    }
                    break;
                case T_TIME:
                    //格式为HH:MM:SS
                    //范围为-838:59:59到838:59:59,可表示一段时间或当天的某一时刻
                    {
                        int32 val_int32;
                        int32 hour,minute,second;
                        char c;
                        iss>>hour>>c>>minute>>c>>second;
                        //判断输入格式是否正确
                        if(iss.fail()){
                            throw TableCreateError("Type Time Error:Time format error");
                        }
                        //判断是否在范围里,不在则抛出异常
                        if(hour<-838||hour>838||minute<0||minute>59||second<0||second>59){
                            throw TableCreateError("Type Time Error:Time out of range");
                        }
                        val_int32 = hour*3600+minute*60+second;
                        table_info_->write(reinterpret_cast<char*>(&val_int32),it.default_length_);
                    }
                    break;
                case T_YEAR:
                    //格式为YYYY
                    //范围为1901到2155
                    {
                        uint8 val_uint8;
                        int32 year;
                        iss>>year;
                        //判断输入格式是否正确
                        if(iss.fail()){
                            throw TableCreateError("Type Year Error:Year format error");
                        }
                        //判断是否在范围里,不在则抛出异常
                        if(year<1901||year>2155){
                            throw TableCreateError("Type Year Error:Year out of range");
                        }
                        val_uint8 = year-1901;
                        table_info_->write(reinterpret_cast<char*>(&val_uint8),it.default_length_);
                    }
                    break;
                case T_TIMESTAMP:
                    //格式为YYYY-MM-DD HH:MM:SS,采用64位时间戳
                    //以1970-01-01 00:00:00为起始值
                    {
                        std::tm timestamp = {};
                        iss>> std::get_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                        if(iss.fail()){
                            throw TableCreateError("Type Timestamp Error:Timestamp out of range or format error");
                        }
                        std::time_t val_int64;//存储时间戳,时间戳的数据类型为int64
                        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&timestamp));
                        val_int64 = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
                        table_info_->write(reinterpret_cast<char*>(&val_int64),it.default_length_);
                    }
                    break;
                //5.字符类型
                default:
                    table_info_->write(any_cast<string_view>(it.default_).data(),it.default_length_);
                    break;
            }
        }
        if(it.comment_length_!=0){
            //如果有注释,则写入注释
            table_info_->write(it.comment_.data(),it.comment_length_);
        }
    }
    //计算每一列的偏移量
    uint16 shift = 0;
    for(int i = 0;i < col_cnt_;i++){
        col_shift_.push_back(shift);
        shift += col_info[i].length_;
    }
    //计算每一条记录的长度
    record_length_ = shift;
    //写入索引的数量
    index_cnt_ = 0;
    table_info_->write(reinterpret_cast<char*>(&index_cnt_),sizeof(uint16));
    //最后打开表的数据文件,其文件路径为db_path/table_name.tb,
    string fileData = db_path_+"/"+table_name_+".table";
    tb_data_ = make_unique<BPTree>(fileData);
    //如果有索引文件,一并将其加载入内存
    for(auto &it:index_name_){
        string fileIndex = db_path_+"/"+it+".idx";
        tb_index_[it] = make_unique<BPTree>(fileIndex);
    }
}
Table::~Table(){}
void Table::DropTable(){
    //删除表
    //删除表的配置文件,其文件路径为db_path/table_name.tbinfo
    string fileInfo = db_path_+"/"+table_name_+".tbinfo";
    if(filesystem::exists(fileInfo)){
        filesystem::remove(fileInfo);
    }
    //删除表的数据文件,其文件路径为db_path/table_name.table,
    string fileData = db_path_+"/"+table_name_+".table";
    if(filesystem::exists(fileData)){
        filesystem::remove(fileData);
    }
    //删除表的索引文件,其文件路径为db_path/index_name.idx
    for(auto &it:index_name_){
        string fileIndex = db_path_+"/"+it+".idx";
        if(filesystem::exists(fileIndex)){
            filesystem::remove(fileIndex);
        }
    }
}

string Table::deserialize(vector<byte> &data){
    //每次只能解析一个行数据
    if(data.size()!=record_length_){
        //数据长度不对
        throw TableDataError("Data Deserialize Error");
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
                {
                    int64 val_int64;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_int64));
                    it += nowProcess->length_;
                    json << val_int64;
                }
                break;
            //2.数值类型:浮点型(单精度)
            case T_FLOAT:
                {
                    float val_float;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_float));
                    it += nowProcess->length_;
                    json << val_float;
                }
                break;
            //3.数值类型:浮点型(双精度)
            case T_DOUBLE:
                {
                    double val_double;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_double));
                    it += nowProcess->length_;
                    json <<val_double;
                }
                break;
            //4.数值类型:无符号类型(日期和时间类型),需要将uint64转换为字符串
            case T_DATE:
                //格式为YYYY-MM-DD
                //范围为1000-01-01到9999-12-31
                {
                    uint64 val_uint64;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_uint64));
                    it += nowProcess->length_;
                    uint64 year,month,day;
                    day = val_uint64&0x1fULL;
                    val_uint64 >>= 5ULL;
                    month = val_uint64&0xfULL;
                    val_uint64 >>= 4ULL;
                    year = val_uint64;
                    json << year << "-" << month << "-" << day;
                }
                break;
            case T_TIME:
                //格式为HH:MM:SS
                //范围为-838:59:59到838:59:59
                {
                    int32 val_int32;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_int32));
                    it += nowProcess->length_;
                    int32 hour,minute,second;
                    hour = val_int32/3600;
                    val_int32 %= 3600;
                    minute = val_int32/60;
                    val_int32 %= 60;
                    second = val_int32;
                    json << hour << ":" << minute << ":" << second;
                }
                break;
            case T_YEAR:
                //格式为YYYY
                //范围为1901到2155
                {
                    uint64 val_uint64;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_uint64));
                    it += nowProcess->length_;
                    uint64 year;
                    year = val_uint64+1901;
                    json << year;
                }
                break;
            case T_TIMESTAMP:
                //格式为YYYY-MM-DD HH:MM:SS
                //以1970-01-01 00:00:00为起始值
                {
                    std::time_t val_time_t;
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(&val_time_t));
                    it += nowProcess->length_;
                    std::tm timestamp = *std::localtime(&val_time_t);
                    json<<std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                }
                break;
            //5.字符类型
            default:
                {
                    char buffer[PAGE_SIZE]={0};//输出缓冲
                    std::copy(it,it+nowProcess->length_,reinterpret_cast<std::byte*>(buffer));
                    it += nowProcess->length_;
                    buffer[nowProcess->length_] = 0;//截断
                    string varchar;
                    varchar.assign(buffer);
                    json << varchar;
                } 
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
        throw TableDataError("Data Deserialize Error");
        return "{}";
    }
    return json.str();
}
void Table::CreateIndex(const string& col_name,const string& index_name){
    //新建索引,创建索引类型为B+树
    //首先判断有没有同名索引
    if(col2index_.find(col_name)!=col2index_.end()){
        throw TableIndexError("Index "+index_name+" already exists");
    }
    //确认没有同名索引后,创建索引
    //索引文件路径为db_path/index_name.idx
    string fileIndex = db_path_+"/"+index_name+".idx";
    if(filesystem::exists(fileIndex)){
        throw TableIndexError("Index "+index_name+" already exists");
    }
    //确认没有同名索引后,创建索引
    tb_index_[index_name] = make_unique<BPTree>(fileIndex);
    //TODO 利用表的数据文件,创建索引文件
}