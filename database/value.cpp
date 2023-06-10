#include "value.hpp"
#include "MyException.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <chrono>
using namespace std;
Key::Key(bool is_maximun){
    is_maximun_ = is_maximun;
    is_minimun_ = !is_maximun;
    data_type_ = T_INT;
    value_int_ = -2147483648;
    str_len_ = 0;
}
Key::Key(int64 value)
:is_maximun_(false),is_minimun_(false),data_type_(T_BIG_INT),value_int_(value),str_len_(0)
{
}
Key::Key(const string& key,uint8 data_type)
:is_maximun_(false),is_minimun_(false),data_type_(data_type),str_len_(key.length())
{
    //根据数据类型,将key转换为对应的类型
    switch(data_type){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoll(key);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_float64_ = stod(key);
            break;
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                istringstream iss(key);
                uint64 year,month,day;
                char c;
                iss>>year>>c>>month>>c>>day;
                //判断输入格式是否正确
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,Date format error,cannot convert to uint64");
                }
                    //判断是否在范围里,不在则抛出异常
                if(year<1000||year>9999||month<1||month>12||day<1||day>31){
                    throw ColValueError("Data Serialize Error,Date out of range");
                }
                value_uint_ = (year<<9ULL)|(month<<5ULL)|day;
            }
            break;
        case T_YEAR:
            //格式为YYYY
            //范围为1901-2155
            {
                istringstream iss(key);
                uint64 year;
                iss>>year;
                //判断输入格式是否正确
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,Year format error,cannot convert to uint64");
                }
                //判断是否在范围里,不在则抛出异常
                if(year<1901||year>2155){
                    throw ColValueError("Data Serialize Error,Year out of range");
                }
                value_uint_ = year;
            }
            break;
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                istringstream iss(key);
                std::tm timestamp = {};
                iss>> std::get_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,timestamp out of range or format error,cannot convert to time_t");
                }
                std::time_t val_int64;//存储时间戳,时间戳的数据类型为int64
                auto tp = std::chrono::system_clock::from_time_t(std::mktime(&timestamp));
                value_uint_ = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
            }
            break;
        default:
            value_str_ = key;
    }
}
Key::Key(vector<byte>::iterator begin,int len,uint8 data_type)
:is_maximun_(false),is_minimun_(false),data_type_(data_type),str_len_(len)
{
    switch(data_type){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = 0;
            std::copy(begin,begin+len,(byte*)&value_int_);
            break;
        case T_FLOAT:
            value_float64_ = *(float32*)&(*begin);
            break;
        case T_DOUBLE:
            value_float64_ = *(float64*)&(*begin);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = 0;
            std::copy(begin,begin+len,(byte*)&value_uint_);
            break;
        default:
            value_str_.assign((char*)&(*begin));
            //value_str_ = std::string(reinterpret_cast<char*>(&(*begin)), len);
    }
}
string Key::getKey() const{
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            return to_string(value_int_);
        case T_FLOAT:case T_DOUBLE:
            return to_string(value_float64_);
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return to_string(value_uint_);
        default:
            return value_str_;
    }
}
int Key::size() const{
    return SIZE_OF_T[data_type_];
}
vector<byte> Key::getBytes() const{
    vector<byte> bytes;
    float32 tmp;
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            //根据SIZE_OF_T[data_type_]来确定bytes的大小
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_int_,(byte*)&value_int_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_FLOAT:
            bytes.resize(SIZE_OF_T[data_type_]);
            tmp = value_float64_;
            std::copy((byte*)&tmp,(byte*)&tmp+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DOUBLE:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_float64_,(byte*)&value_float64_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_uint_,(byte*)&value_uint_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        default:
            //补0
            bytes = vector<byte>(str_len_,std::byte(0));
            std::copy((byte*)value_str_.data(),(byte*)value_str_.data()+value_str_.size(),bytes.begin());
    }
    return bytes;
}
bool Key::operator<(const Key& key) const{
    //看看有没有最小/最大
    if(is_minimun_ || key.IsMaximun())
        return true;
    if(is_maximun_ || key.IsMinimun())
        return false;
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            return value_int_ < key.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_float64_ < key.value_float64_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ < key.value_uint_;
        default:
            return value_str_ < key.value_str_;
    }
    
}
bool Key::operator==(const Key& key) const{
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            return value_int_ == key.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_float64_ == key.value_float64_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ == key.value_uint_;
        default:
            return value_str_ == key.value_str_;
    }
}
bool Key::operator!=(const Key& key) const{
    return !(*this == key);
}
bool Key::operator>(const Key& key) const{
    return !(*this < key || *this == key);
}
bool Key::operator<=(const Key& key) const{
    return !(*this > key);
}
bool Key::operator>=(const Key& key) const{
    return !(*this < key);
}
bool Key::IsMinimun() const{
    return is_minimun_;
}
bool Key::IsMaximun() const{
    return is_maximun_;
}

ColValue::ColValue(const string& col_name,const string& value,uint8 data_type)
:col_name_(col_name),data_type_(data_type)
{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoll(value);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_float64_ = stod(value);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = stoull(value);
            break;
        default:
            value_str_ = value;
    }
}
ColValue::ColValue(const string& col_name,float64 value):col_name_(col_name),value_float64_(value),data_type_(T_DOUBLE){}
ColValue::ColValue(const string& col_name,int64 value):col_name_(col_name),value_int_(value),data_type_(T_BIG_INT){}
ColValue::ColValue(const string& col_name,const string& value):col_name_(col_name),value_str_(value),data_type_(T_CHAR){}
ColValue::ColValue(const string& col_name,uint8 data_type,vector<byte>::iterator begin,int len)
:col_name_(col_name),data_type_(data_type)
{
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = 0;
            std::copy(begin,begin+len,(byte*)&value_int_);
            break;
        case T_FLOAT:
            value_float64_ = *(float32*)&(*begin);
            break;
        case T_DOUBLE:
            value_float64_ = *(float64*)&(*begin);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = 0;
            std::copy(begin,begin+len,(byte*)&value_uint_);
            break;
        default:
            //注意截断
            value_str_.assign((char*)&(*begin));
            //value_str_ = std::string(reinterpret_cast<char*>(&(*begin)), len);
            //cerr << "Get value_str_"<< value_str_ << endl;
    }
}
vector<byte> ColValue::getBytes() const{
    vector<byte> bytes;
    float32 tmp;
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            //根据SIZE_OF_T[data_type_]来确定bytes的大小
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_int_,(byte*)&value_int_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_FLOAT:
            bytes.resize(SIZE_OF_T[data_type_]);
            tmp = value_float64_;
            std::copy((byte*)&tmp,(byte*)&tmp+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DOUBLE:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_float64_,(byte*)&value_float64_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_uint_,(byte*)&value_uint_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        default:
            bytes.resize(value_str_.size());
            std::copy((byte*)value_str_.data(),(byte*)value_str_.data()+value_str_.size(),bytes.begin());
    }
    return bytes;
}
string ColValue::getColName() const{
    return col_name_;
}
void ColValue::setValue(const string& value){
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoi(value);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_float64_ = stod(value);
            break;
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                istringstream iss(value);
                uint64 year,month,day;
                char c;
                iss>>year>>c>>month>>c>>day;
                //判断输入格式是否正确
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,Date format error,cannot convert to uint64");
                }
                    //判断是否在范围里,不在则抛出异常
                if(year<1000||year>9999||month<1||month>12||day<1||day>31){
                    throw ColValueError("Data Serialize Error,Date out of range");
                }
                value_uint_ = (year<<9ULL)|(month<<5ULL)|day;
            }
            break;
        case T_YEAR:
            //格式为YYYY
            //范围为1901-2155
            {
                istringstream iss(value);
                uint64 year;
                iss>>year;
                //判断输入格式是否正确
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,Year format error,cannot convert to uint64");
                }
                //判断是否在范围里,不在则抛出异常
                if(year<1901||year>2155){
                    throw ColValueError("Data Serialize Error,Year out of range");
                }
                value_uint_ = year;
            }
            break;
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                istringstream iss(value);
                std::tm timestamp = {};
                iss>> std::get_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                if(iss.fail()){
                    throw ColValueError("Data Serialize Error,timestamp out of range or format error,cannot convert to time_t");
                }
                std::time_t val_int64;//存储时间戳,时间戳的数据类型为int64
                auto tp = std::chrono::system_clock::from_time_t(std::mktime(&timestamp));
                value_uint_ = std::chrono::duration_cast<std::chrono::seconds>(tp.time_since_epoch()).count();
            }
            break;
        default:
            value_str_ = value;
    }
}
string ColValue::getValue() const{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return to_string(value_int_);
        case T_FLOAT:case T_DOUBLE:
            return to_string(value_float64_);
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                int64 year = value_int_/10000;
                int64 month = (value_int_%10000)/100;
                int64 day = value_int_%100;
                return "\\\""+to_string(year)+"-"+to_string(month)+"-"+to_string(day)+"\\\"";
            }
        case T_TIME:
            //格式为HH:MM:SS
            //范围为-838:59:59到838:59:59
            {
                int64 hour = value_int_/3600;
                int64 minute = (value_int_%3600)/60;
                int64 second = value_int_%60;
                return "\\\""+to_string(hour)+":"+to_string(minute)+":"+to_string(second)+"\\\"";
            }
        case T_YEAR:
            //格式为YYYY
            //范围为1901到2155
            return "\\\""+to_string(value_uint_)+"\\\"";
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                std::time_t val_time_t = value_uint_;
                std::tm timestamp = *std::localtime(&val_time_t);
                stringstream ss;
                ss << std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                return "\\\""+ss.str()+"\\\"";
            }
        default:
            return "\\\""+value_str_+"\\\"";
    }
}
uint8 ColValue::getDataType() const{
    return data_type_;
}
int ColValue::size() const{
    //如果data_type_为T_CHAR,则返回value_str_的长度
    if(data_type_ == T_CHAR || data_type_ == T_VARCHAR)
        return value_str_.size();
    //否则返回SIZE_OF_T[data_type_]
    return SIZE_OF_T[data_type_];
}
pair<string,string> ColValue::getColValue() const{
    //根据数据类型,将value转换为对应的类型
    return make_pair(col_name_,getValue());
    
}
bool ColValue::isSameType(const ColValue& other) const{
    return data_type_ == other.data_type_;
}
bool ColValue::operator==(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        return false;
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ == other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_float64_ == other.value_float64_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ == other.value_uint_;
        default:
            return value_str_ == other.value_str_;
    }
}
bool ColValue::operator!=(const ColValue& other) const{
    return !(*this == other);
}
bool ColValue::operator<(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        throw ColValueError("ColValue::operator<:类型不匹配");
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ < other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_float64_ < other.value_float64_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ < other.value_uint_;
        default:
            return value_str_ < other.value_str_;
    }
}
bool ColValue::operator<=(const ColValue& other) const{
    if(data_type_ != other.data_type_){
        throw ColValueError("ColValue::operator<=:类型不匹配");
    }
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return value_int_ <= other.value_int_;
        case T_FLOAT:case T_DOUBLE:
            return value_float64_ <= other.value_float64_;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            return value_uint_ <= other.value_uint_;
        default:
            return value_str_ <= other.value_str_;
    }
}
bool ColValue::operator>(const ColValue& other) const{
    return !(*this <= other);
}
bool ColValue::operator>=(const ColValue& other) const{
    return !(*this < other);
}
Key ColValue::toKey(uint16 len) const{
    vector<byte> data = this->getBytes();
    if(data_type_ == T_CHAR || data_type_ == T_VARCHAR){
        //如果是字符串,则在末尾补0
        while(data.size() < len){
            data.push_back(std::byte(0));
        }
    }
    return Key(data.begin(),data.size(),data_type_);
}