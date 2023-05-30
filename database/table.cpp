#include "table.hpp"
#include "MyException.hpp"

#include <fstream>
#include <algorithm>
#include <sstream>
#include <filesystem>
#include <set>
using namespace std;
Row::Row(vector<TableColAttribute>& col_info,vector<byte> data){
    //根据表头信息和数据,构造一行数据
    //数据格式为:每个字段的数据类型+每个字段的数据
    uint16 shift = 0;
    for(auto &i:col_info){
        ColValue col_value(i.col_name_,i.data_type_,data.begin()+shift,i.length_);
        shift += i.length_;
        col_value_.push_back(col_value);
    }
}
void Row::Update(vector<pair<string,string>> &col_item){
    //更新一条记录,参数为列名和列值
    for(auto &i:col_item){
        for(auto &j:col_value_){
            if(j.getColName() == i.first){
                j.setValue(i.second);
            }
        }
    }
}
bool Row::isSatisfied(const SQLWhere& where) const{
    //先判断Where语句顶层是AND还是OR,如果是AND则需要都满足,如果是OR则只需要有一个满足
    if(where.getOperator() == ClauseOperator::AND){
        for(auto &i:col_value_){
            if(!where.Filter(i)){
                return false;
            }
        }
        return true;
    }else{
        for(auto &i:col_value_){
            if(!where.Filter(i)){
                return true;
            }
        }
        return false;
    }
}
string Row::getRowJSON() const{
    //获取一条记录的JSON格式
    string res = "{";
    for(auto &i:col_value_){
        res += "\""+i.getColName()+"\":"+i.getValue()+",";
    }
    res.pop_back();
    res += "}";
    return res;
}
vector<byte> Row::toByte() const{
    //将一条记录转换为字节流
    vector<byte> res;
    for(auto &i:col_value_){
        vector<byte> tmp = i.getBytes();
        res.insert(res.end(),tmp.begin(),tmp.end());
    }
    return res;
}
ColValue Row::getValue(const string& col_name) const{
    //根据列名获取列值
    for(auto &i:col_value_){
        if(i.getColName() == col_name){
            return i;
        }
    }
    throw ColValueError("列名不存在");
}



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
    //遍历表的数据文件,将数据文件中的数据插入到索引中
    //直接按顺序读取数据文件,然后插入到索引中
    tb_data_->ReadFirstChunk();
    while(tb_data_->isBufLeaf()){
        //读取这一页的数据
        auto data = tb_data_->GetAllElemInChunk();
        for(auto &it:data){
            //将数据插入到索引中
            Row row(col_info_,it);
            tb_index_[index_name]->Insert(row.getValue(col_name).toKey(),it);
        }
    }
}

vector<byte> Table::serialize(vector<pair<string,string>> &col_item){
    //遍历,如果有列名不在表中,则抛出异常
    for(auto &it:col_item){
        if(col2id_.find(it.first)==col2id_.end()){
            throw TableDataError("Data Serialize Error,Column "+it.first+" not exists");
        }
    }
    // 随后根据列的顺序(即TableAttribute),将数据写入data中
    vector<byte> data(record_length_,std::byte(0));
    for(auto &it:col_item){
        //先利用col2id_找到列的id
        int id = col2id_[it.first];
        //再利用col_shift_找到列的偏移量
        uint16 shift = col_shift_[id];
        //根据数据类型,将string转换为对应的数据类型
        istringstream iss(it.second);
        switch(col_info_[id].data_type_){
            //1.数值类型:整型(符号型)
            case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
                {
                    int64 val_int64;
                    iss>>val_int64;
                    if(iss.fail()){
                        throw TableDataError("Data Serialize Error,can't convert to int64,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    std::copy(reinterpret_cast<std::byte*>(&val_int64),reinterpret_cast<std::byte*>(&val_int64)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            //2.数值类型:浮点型(单精度)
            case T_FLOAT:
                {
                    float val_float;
                    iss>>val_float;
                    if(iss.fail()){
                        throw TableDataError("Data Serialize Error,can't convert to float,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    std::copy(reinterpret_cast<std::byte*>(&val_float),reinterpret_cast<std::byte*>(&val_float)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            //3.数值类型:浮点型(双精度)
            case T_DOUBLE:
                {
                    double val_double;
                    iss>>val_double;
                    if(iss.fail()){
                        throw TableDataError("Data Serialize Error,can't convert to double,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    std::copy(reinterpret_cast<std::byte*>(&val_double),reinterpret_cast<std::byte*>(&val_double)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            //4.数值类型:无符号类型(日期和时间类型),需要将字符串转换为uint64
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
                        throw TableDataError("Data Serialize Error,Date format error,cannot convert to uint64,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    //判断是否在范围里,不在则抛出异常
                    if(year<1000||year>9999||month<1||month>12||day<1||day>31){
                        throw TableDataError("Data Serialize Error,Date out of range,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    val_uint64 = (year<<9ULL)|(month<<5ULL)|day;
                    std::copy(reinterpret_cast<std::byte*>(&val_uint64),reinterpret_cast<std::byte*>(&val_uint64)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            case T_TIME:
                //格式为HH:MM:SS
                //范围为-838:59:59到838:59:59
                {
                    int32 val_int32;
                    int32 hour,minute,second;
                    char c;
                    iss>>hour>>c>>minute>>c>>second;
                    //判断输入格式是否正确
                    if(iss.fail()){
                        throw TableDataError("Data Serialize Error,Time format error,cannot convert to int32,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    //判断是否在范围里,不在则抛出异常
                    if(hour<-838||hour>838||minute<0||minute>59||second<0||second>59){
                        throw TableDataError("Data Serialize Error,Time out of range,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    val_int32 = hour*3600+minute*60+second;
                    std::copy(reinterpret_cast<std::byte*>(&val_int32),reinterpret_cast<std::byte*>(&val_int32)+col_info_[id].length_,data.begin()+shift);
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
                        throw TableDataError("Data Serialize Error,year format error,cannot convert to uint8,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    //判断是否在范围里,不在则抛出异常
                    if(year<1901||year>2155){
                        throw TableDataError("Data Serialize Error,year out of range,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    val_uint8 = year-1901;
                    std::copy(reinterpret_cast<std::byte*>(&val_uint8),reinterpret_cast<std::byte*>(&val_uint8)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            case T_TIMESTAMP:
                //格式为YYYY-MM-DD HH:MM:SS
                //以1970-01-01 00:00:00为起始值
                {
                    std::tm timestamp = {};
                    iss>> std::get_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                    if(iss.fail()){
                        throw TableDataError("Data Serialize Error,timestamp out of range or format error,cannot convert to time_t,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    std::time_t val_int64;//存储时间戳,时间戳的数据类型为int64
                    auto tp = std::chrono::system_clock::from_time_t(std::mktime(&timestamp));
                    val_int64 = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
                    std::copy(reinterpret_cast<std::byte*>(&val_int64),reinterpret_cast<std::byte*>(&val_int64)+col_info_[id].length_,data.begin()+shift);
                }
                break;
            //5.字符类型
            default:
                {
                    //如果是字符串类型,先判断长度是否超过
                    if(it.second.length()>col_info_[id].length_){
                        throw TableDataError("Data Serialize Error,varchar length out of range,COLUMN:"+it.first+" VALUE:"+it.second);
                    }
                    //如果没有超过,则直接拷贝
                    std::copy(reinterpret_cast<std::byte*>(it.second.data()),reinterpret_cast<std::byte*>(it.second.data())+col_info_[id].length_,data.begin()+shift);
                } 
                break;
        }
    }
    return data;
}
Key Table::getValue(vector<byte> &data,uint16 col_id){
    //根据col_id获取列的类型
    uint8 type = col_info_[col_id].data_type_;
    //获取偏移量
    uint16 shift = col_shift_[col_id];
    //获取大小
    uint16 size = col_info_[col_id].length_;
    //获取对应字节流
    vector<byte> value(data.begin()+shift,data.begin()+shift+size);
    //根据类型,将data中的数据转换为Key
    return Key(value.begin(),size,type);
}
void Table::SelectRecord(SQLWhere &where){
    //用Where类的成员函数获取最应该使用的索引
    string indexName = where.GetBestIndex(index_col_name_,primary_key_);
    //随后根据索引的类型,调用不同的函数
    //先判断这玩意是不是主键,再判断有没有这样的索引
    vector<Row> rows_result;
    if(indexName == primary_key_){
        //如果是主键,则直接调用主键查找函数
        if(where.GetQueryType(indexName) == QueryType::QUERY_EQ){
            //如果是等值查询,则调用主键查找函数
            Row row(col_info_,tb_data_->Search(where.GetQueryKey(indexName,tb_data_->getKeyType())));
            rows_result.push_back(row);
            //result.push_back(tb_data_->Search(where.GetQueryKey(indexName)));
        
        }else{
            //如果是范围查询,则调用主键范围查找函数
            vector<vector<byte>> record = tb_data_->SearchRange(where.GetQueryLeftKey(indexName,tb_data_->getKeyType()),where.GetQueryRightKey(indexName,tb_data_->getKeyType()));
            for(auto &it:record){
                Row row(col_info_,it);
                if(where.getTermNum() == 1||row.isSatisfied(where)){
                    rows_result.push_back(row);
                }
            }
        }
    }else if(col2index_.find(indexName)==col2index_.end()){
        //如果没有索引且不为主键,顺序查找过滤
        vector<vector<byte>> record;
        tb_data_->ReadFirstChunk();
        while(tb_data_->isBufLeaf()){
            record = tb_data_->GetAllElemInChunk();
            for(auto &it:record){
                Row row(col_info_,it);
                if(row.isSatisfied(where)){
                    //result.push_back(it);
                    rows_result.push_back(row);
                }
            }
            tb_data_->ReadNextChunk();
        }
    }else{
        //如果有,则调用索引的查找函数
        //先将Where类中的条件转换为索引的键值
        //先检测查询类型,判断是范围查询还是等值查询
        if(where.GetQueryType(indexName) == QueryType::QUERY_EQ){
            //如果是等值查询,则直接调用索引的查找函数
            //要注意,索引查找返回的值是一个块的地址,需要把这个块读取出来,再进行过滤
            vector<byte> addr = tb_index_[indexName]->Search(where.GetQueryKey(indexName,tb_data_->getKeyType()));
            //将这个字节流转换为streampos类型
            streampos pos;
            std::copy(reinterpret_cast<char*>(addr.data()),reinterpret_cast<char*>(addr.data())+sizeof(streampos),reinterpret_cast<char*>(&pos));
            //将这个块读取出来
            tb_data_->ReadChunk(pos);
            //将这个块中的所有元素读取出来,并过滤
            vector<vector<byte>> record = tb_data_->GetAllElemInChunk();
            for(auto &it:record){
                Row row(col_info_,it);
                if(row.isSatisfied(where)){
                    rows_result.push_back(row);
                }
            }
        }else{
            //如果是范围查询,则调用索引的范围查找函数
            //当然,返回值为块地址,需要把块读出来,再进行过滤
            vector<vector<byte>> addr_list = tb_index_[indexName]->SearchRange(where.GetQueryLeftKey(indexName,tb_data_->getKeyType()),where.GetQueryRightKey(indexName,tb_data_->getKeyType()));
            //接下来转换成streampos并去重
            set<streampos> pos_set;
            for(auto &it:addr_list){
                streampos pos;
                std::copy(reinterpret_cast<char*>(it.data()),reinterpret_cast<char*>(it.data())+sizeof(streampos),reinterpret_cast<char*>(&pos));
                pos_set.insert(pos);
            }
            //随后将这些块读取出来,并过滤
            for(auto &it:pos_set){
                tb_data_->ReadChunk(it);
                vector<vector<byte>> record = tb_data_->GetAllElemInChunk();
                for(auto &it:record){
                    Row row(col_info_,it);
                    if(row.isSatisfied(where)){
                        rows_result.push_back(row);
                    }
                }
            }
        }
    }
    //最后将结果转换为字符串输出
    PrintToStream("Result Found",rows_result);
}

void Table::InsertRecord(vector<pair<string,string>> &col_item){
    //一条一条插入记录,先将记录序列化
    vector<byte> data = serialize(col_item);
    //获取主键的值
    Key primary_key = getValue(data,col2id_[primary_key_]);
    //随后调用Table的插入函数
    tb_data_->Insert(primary_key,data);
    //然后查询插入的位置
    streampos pos = tb_data_->SearchPos(primary_key);
    //最后调用索引的插入函数
    for(auto &it:col2index_){
        //先获取索引的键值
        Key index_key = getValue(data,col2id_[it.first]);
        //随后调用索引的插入函数
        //将pos转换为字节流
        vector<byte> index_data(sizeof(streampos));
        std::copy(reinterpret_cast<char*>(&pos),reinterpret_cast<char*>(&pos)+sizeof(streampos),reinterpret_cast<char*>(index_data.data()));
        tb_index_[it.first]->Insert(index_key,index_data);
    }
    saySuccess();
}
void Table::UpdateRecord(vector<pair<string,string>> &col_item,SQLWhere &where){
    //先根据Where类的成员函数获取最应该使用的索引
    string indexName = where.GetBestIndex(index_col_name_,primary_key_);
    //随后根据索引的类型,调用不同的函数
    //目前只支持更新的不是主键,所以不用考虑主键更新,主键更新直接丢异常
    //col_item中保存的是要更新的列的名字和值,所以需要先将这些值转换为字节流
    //更新在某种层度上是删除加插入,如果更改涉及到主键或者索引,则需要先删除,再插入
    vector<Row> rows_result;
    if(indexName == primary_key_){
        //如果是主键,则直接调用主键查找函数
        if(where.GetQueryType(indexName) == QueryType::QUERY_EQ){
            //如果是等值查询,则调用主键查找函数
            Row row(col_info_,tb_data_->Search(where.GetQueryKey(indexName,tb_data_->getKeyType())));
            rows_result.push_back(row);
            //result.push_back(tb_data_->Search(where.GetQueryKey(indexName)));
        
        }else{
            //如果是范围查询,则调用主键范围查找函数
            vector<vector<byte>> record = tb_data_->SearchRange(where.GetQueryLeftKey(indexName,tb_data_->getKeyType()),where.GetQueryRightKey(indexName,tb_data_->getKeyType()));
            for(auto &it:record){
                Row row(col_info_,it);
                if(where.getTermNum() == 1||row.isSatisfied(where)){
                    rows_result.push_back(row);
                }
            }
        }
    }else if(col2index_.find(indexName)==col2index_.end()){
        //如果没有索引且不为主键,顺序查找过滤
        vector<vector<byte>> record;
        tb_data_->ReadFirstChunk();
        while(tb_data_->isBufLeaf()){
            record = tb_data_->GetAllElemInChunk();
            for(auto &it:record){
                Row row(col_info_,it);
                if(row.isSatisfied(where)){
                    //result.push_back(it);
                    rows_result.push_back(row);
                }
            }
            tb_data_->ReadNextChunk();
        }
    }else{
        //如果有,则调用索引的查找函数
        //先将Where类中的条件转换为索引的键值
        //先检测查询类型,判断是范围查询还是等值查询
        if(where.GetQueryType(indexName) == QueryType::QUERY_EQ){
            //如果是等值查询,则直接调用索引的查找函数
            //要注意,索引查找返回的值是一个块的地址,需要把这个块读取出来,再进行过滤
            vector<byte> addr = tb_index_[indexName]->Search(where.GetQueryKey(indexName,tb_data_->getKeyType()));
            //将这个字节流转换为streampos类型
            streampos pos;
            std::copy(reinterpret_cast<char*>(addr.data()),reinterpret_cast<char*>(addr.data())+sizeof(streampos),reinterpret_cast<char*>(&pos));
            //将这个块读取出来
            tb_data_->ReadChunk(pos);
            //将这个块中的所有元素读取出来,并过滤
            vector<vector<byte>> record = tb_data_->GetAllElemInChunk();
            for(auto &it:record){
                Row row(col_info_,it);
                if(row.isSatisfied(where)){
                    rows_result.push_back(row);
                }
            }
        }else{
            //如果是范围查询,则调用索引的范围查找函数
            //当然,返回值为块地址,需要把块读出来,再进行过滤
            vector<vector<byte>> addr_list = tb_index_[indexName]->SearchRange(where.GetQueryLeftKey(indexName,tb_data_->getKeyType()),where.GetQueryRightKey(indexName,tb_data_->getKeyType()));
            //接下来转换成streampos并去重
            set<streampos> pos_set;
            for(auto &it:addr_list){
                streampos pos;
                std::copy(reinterpret_cast<char*>(it.data()),reinterpret_cast<char*>(it.data())+sizeof(streampos),reinterpret_cast<char*>(&pos));
                pos_set.insert(pos);
            }
            //随后将这些块读取出来,并过滤
            for(auto &it:pos_set){
                tb_data_->ReadChunk(it);
                vector<vector<byte>> record = tb_data_->GetAllElemInChunk();
                for(auto &it:record){
                    Row row(col_info_,it);
                    if(row.isSatisfied(where)){
                        rows_result.push_back(row);
                    }
                }
            }
        }
    }
    //随后,检查更新的列是否有索引或主键,如果有索引或主键,则需要先删除索引或主键,再更新
    vector<string> update_index;
    bool remove_primary = false;
    for(auto &it:col_item){
        if(it.first == primary_key_){
            //如果有主键,则需要删除主键和索引,再插入
            Key key(it.first,tb_data_->getKeyType());
            tb_data_->Remove(key);
            remove_primary = true;
        }else if(col2index_.find(it.first)!=col2index_.end()){
            //如果有索引,则需要先删除索引
            Key index(it.first,tb_index_[it.first]->getKeyType());
            tb_index_[it.first]->Remove(index);
            update_index.push_back(it.first);
        }
    }
    //然后更新这些记录
    for(auto &it:rows_result){
        it.Update(col_item);
    }
    //随后,将这些记录写回
    vector<streampos> pos_list;
    if(remove_primary){
        for(auto &it:rows_result){
            vector<byte> record = it.toByte();
            tb_data_->Insert(it.getValue(primary_key_).toKey(),record);
            pos_list.push_back(tb_data_->SearchPos(it.getValue(primary_key_).toKey()));
        }
    }else{
        for(auto &it:rows_result){
            vector<byte> record = it.toByte();
            tb_data_->Update(it.getValue(primary_key_).toKey(),record);
            pos_list.push_back(tb_data_->SearchPos(it.getValue(primary_key_).toKey()));
        }
    }
    //TODO 这里应该找到对应记录的pos,然后更新
    for(auto &it:update_index){
        for(int i=0;i<rows_result.size();i++){
            //将streampos转换为字节流
            vector<byte> pos_byte(sizeof(streampos));
            std::copy(reinterpret_cast<char*>(&pos_list[i]),reinterpret_cast<char*>(&pos_list[i])+sizeof(streampos),reinterpret_cast<char*>(pos_byte.data()));
            tb_index_[it]->Insert(rows_result[i].getValue(it).toKey(),pos_byte);
        }
    }
}
void Table::DeleteRecord(SQLWhere &where){
    //用Where类的成员函数获取最应该使用的索引
    string indexName = where.GetBestIndex(index_col_name_,primary_key_);
    //随后根据索引的类型,调用不同的函数
    //如果这个家伙是主键,则调用主键删除函数,然后同步更新索引(目前均是唯一索引)
    //如果这个家伙不是主键,则调用索引删除函数,同时拿到块地址,再调用删除块中某条记录的函数
    //目前只支持等值删除,所以不用考虑范围删除,范围删除直接丢异常
    //同时,删除函数会返回删除的记录,这些记录需要同步删除索引,同时也需要返回给用户
    vector<Row> rows_result;
    if(where.GetQueryType(indexName) != QueryType::QUERY_EQ){
        throw TableDeleteError("Range Delete is not supported now!");
    }
    if(indexName == primary_key_){
        //调用主键删除函数
        //先获取主键的值
        Key primary_key = where.GetQueryKey(indexName,tb_data_->getKeyType());
        //随后调用Table的删除函数
        vector<byte> remove = tb_data_->Remove(primary_key);
        //最后遍历索引,删除索引中的记录
        for(auto &it:tb_index_){
            //先获取索引的键值
            Key index_key = getValue(remove,col2id_[it.first]);
            //随后调用索引的删除函数
            tb_index_[it.first]->Remove(index_key);
        }
        Row row(col_info_,remove);
        rows_result.push_back(row);
    }else if(col2index_.find(indexName)!=col2index_.end()){
        //在索引中有这个值,则调用索引删除函数
        //先获取索引的键值
        Key index_key = where.GetQueryKey(indexName,tb_data_->getKeyType());
        //随后调用索引的删除函数
        vector<byte> remove = tb_index_[indexName]->Remove(index_key);
        //最后调用Table的删除函数
        Key primary_key = getValue(remove,col2id_[primary_key_]);
        Row row(col_info_,tb_data_->Remove(primary_key));
        //result.push_back(tb_data_->Remove(std::any_cast<uint64>(primary_key)));
        //最后遍历索引,删除索引中的记录
        for(auto &it:tb_index_){
            if(it.first == indexName){
                //删除过了,继续
                continue;
            }
            //先获取索引的键值
            Key index_key = getValue(remove,col2id_[it.first]);
            //随后调用索引的删除函数
            tb_index_[it.first]->Remove(index_key);
        }
    }else{
        //既不是主键,也不是索引,则全表扫描过滤删除
        //让Table顺序读取叶子节点,然后过滤
        tb_data_->ReadFirstChunk();
        while(tb_data_->isBufLeaf()){
            vector<vector<byte>> record = tb_data_->GetAllElemInChunk();
            for(auto &it:record){
                Row row(col_info_,it);
                if(row.isSatisfied(where)){
                    //如果满足条件,则删除
                    rows_result.push_back(row);
                    //先获取主键的值
                    Key primary_key = getValue(it,col2id_[primary_key_]);
                    tb_data_->Remove(primary_key);
                    //最后遍历索引,删除索引中的记录
                    for(auto &it_:tb_index_){
                        //先获取索引的键值
                        Key index_key = getValue(it,col2id_[it_.first]);
                        //随后调用索引的删除函数
                        tb_index_[it_.first]->Remove(index_key);
                    }
                }
            }
            tb_data_->ReadNextChunk();
        }
    }
    //最后输出结果
    PrintToStream("Delete Success!",rows_result);
}
void Table::saySuccess(){
    cout<<"{\"status_code\":0,\"status_msg\":\"OK\",\"data\":[]}"<<endl;
}
void Table::PrintToStream(string msg,vector<Row> result){
    //返回的JSON示例如下
    /**
     * {
     *  "status_code":0,
     *  "status_msg":"OK",
     *  "data":[]
     * }
     * 
     */
    //其中,data为多条deserialized后的记录

    //先构造status_code和status_msg
    cout<<"{\"status_code\":0,\"status_msg\":\""<<msg<<"\",\"data\":[";
    //遍历输出result
    for(auto it=result.begin();it!=result.end();it++){
        cout<<it->getRowJSON();
        if(it+1!=result.end()){
            cout<<",";
        }
    }

    cout<<"]}"<<endl;
}

