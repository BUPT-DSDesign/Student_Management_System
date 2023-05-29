#include "value.hpp"
#include "MyException.hpp"
#include <ctime>
#include <iomanip>
#include <sstream>
Key::Key(const string& key,uint8 data_type){
    //根据数据类型,将key转换为对应的类型
    switch(data_type){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoi(key);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_float64_ = stod(key);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = stoul(key);
        default:
            value_str_ = key;
    }
    data_type_ = data_type;

}
Key::Key(vector<byte>::iterator begin,int len,uint8 data_type){
    switch(data_type){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = *(int64*)&(*begin);
            break;
        case T_FLOAT:
            value_float64_ = *(float32*)&(*begin);
            break;
        case T_DOUBLE:
            value_float64_ = *(float64*)&(*begin);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = *(uint64*)&(*begin);
        default:
            value_str_ = string(begin,begin+len);
    }
    data_type_ = data_type;
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
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            //根据SIZE_OF_T[data_type_]来确定bytes的大小
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_int_,(byte*)&value_int_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_FLOAT:
            bytes.resize(SIZE_OF_T[data_type_]);
            float32 tmp = value_float64_;
            std::copy((byte*)&tmp,(byte*)&tmp+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DOUBLE:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_float64_,(byte*)&value_float64_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_uint_,(byte*)&value_uint_+SIZE_OF_T[data_type_],bytes.begin());
        default:
            bytes.resize(value_str_.size());
            copy(value_str_.begin(),value_str_.end(),bytes.begin());
    }
    return bytes;
}
bool Key::operator<(const Key& key) const{
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


ColValue::ColValue(const string& col_name,const string& value,uint8 data_type)
:col_name_(col_name),data_type_(data_type)
{
    //根据数据类型,将value转换为对应的类型
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            value_int_ = stoi(value);
            break;
        case T_FLOAT:case T_DOUBLE:
            value_float64_ = stod(value);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = stoul(value);
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
            value_int_ = *(int64*)&(*begin);
            break;
        case T_FLOAT:
            value_float64_ = *(float32*)&(*begin);
            break;
        case T_DOUBLE:
            value_float64_ = *(float64*)&(*begin);
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = *(uint64*)&(*begin);
        default:
            value_str_ = string(begin,begin+len);
    }
}
vector<byte> ColValue::getBytes() const{
    vector<byte> bytes;
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
        case T_TIME:
            //根据SIZE_OF_T[data_type_]来确定bytes的大小
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_int_,(byte*)&value_int_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_FLOAT:
            bytes.resize(SIZE_OF_T[data_type_]);
            float32 tmp = value_float64_;
            std::copy((byte*)&tmp,(byte*)&tmp+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DOUBLE:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_float64_,(byte*)&value_float64_+SIZE_OF_T[data_type_],bytes.begin());
            break;
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            bytes.resize(SIZE_OF_T[data_type_]);
            std::copy((byte*)&value_uint_,(byte*)&value_uint_+SIZE_OF_T[data_type_],bytes.begin());
        default:
            bytes.resize(value_str_.size());
            copy(value_str_.begin(),value_str_.end(),bytes.begin());
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
        case T_DATE:case T_YEAR:case T_TIMESTAMP:
            value_uint_ = stoul(value);
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
                return to_string(year)+"-"+to_string(month)+"-"+to_string(day);
            }
        case T_TIME:
            //格式为HH:MM:SS
            //范围为-838:59:59到838:59:59
            {
                int64 hour = value_int_/3600;
                int64 minute = (value_int_%3600)/60;
                int64 second = value_int_%60;
                return to_string(hour)+":"+to_string(minute)+":"+to_string(second);
            }
        case T_YEAR:
            //格式为YYYY
            //范围为1901到2155
            return to_string(value_uint_);
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                std::time_t val_time_t = value_uint_;
                std::tm timestamp = *std::localtime(&val_time_t);
                stringstream ss;
                ss << std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                return ss.str();
            }
        default:
            return value_str_;
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
    switch(data_type_){
        case T_TINY_INT:case T_SMALL_INT:case T_INT:case T_BIG_INT:
            return make_pair(col_name_,to_string(value_int_));
        case T_FLOAT:case T_DOUBLE:
            return make_pair(col_name_,to_string(value_float64_));
        case T_DATE:
            //格式为YYYY-MM-DD
            //范围为1000-01-01到9999-12-31
            {
                int64 year = value_int_/10000;
                int64 month = (value_int_%10000)/100;
                int64 day = value_int_%100;
                return make_pair(col_name_,to_string(year)+"-"+to_string(month)+"-"+to_string(day));
            }
        case T_TIME:
            //格式为HH:MM:SS
            //范围为-838:59:59到838:59:59
            {
                int64 hour = value_int_/3600;
                int64 minute = (value_int_%3600)/60;
                int64 second = value_int_%60;
                return make_pair(col_name_,to_string(hour)+":"+to_string(minute)+":"+to_string(second));
            }
        case T_YEAR:
            //格式为YYYY
            //范围为1901到2155
            return make_pair(col_name_,to_string(value_uint_));
        case T_TIMESTAMP:
            //格式为YYYY-MM-DD HH:MM:SS
            //以1970-01-01 00:00:00为起始值
            {
                std::time_t val_time_t = value_uint_;
                std::tm timestamp = *std::localtime(&val_time_t);
                stringstream ss;
                ss << std::put_time(&timestamp,"%Y-%m-%d %H:%M:%S");
                return make_pair(col_name_,ss.str());
            }
        default:
            return make_pair(col_name_,value_str_);
    }
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
Key ColValue::toKey() const{
    return Key(this->getValue(),data_type_);
}