#pragma once
#include "datatype.hpp"
#include <string>
#include <vector>
using namespace std;
//键值,用于B+树
class Key{
public:
    Key(bool is_maximun);//设置其为最大值,否为最小值,用于B+树
    Key(int64 value);//从整数构造
    Key(const string& key,uint8 data_type);//从字符串构造
    Key(vector<byte>::iterator begin,int len,uint8 data_type);//从字节流构造
    string getKey() const;//获取键值的字符串形式
    int size() const;//获取键值的字节流长度
    vector<byte> getBytes() const;//获取键值的字节流形式
    bool operator==(const Key& other) const;
    bool operator!=(const Key& other) const;
    bool operator<(const Key& other) const;
    bool operator<=(const Key& other) const;
    bool operator>(const Key& other) const;
    bool operator>=(const Key& other) const;
    bool IsMaximun() const;//判断是否为最大值
    bool IsMinimun() const;//判断是否为最小值
private:
    bool is_maximun_;//设置其为最大值
    bool is_minimun_;//设置其为最小值
    union {
        int64 value_int_;
        float64 value_float64_;
        uint64 value_uint_;
    };
    string value_str_;//为字符类型时的存储
    uint16 str_len_;//如果键值为字符串,则记录其长度
    uint8 data_type_;//数据类型
};
//每行由多个列值组成
class ColValue{
public:
    ColValue(const string& col_name,const string& value,uint8 data_type);//从字符串构造
    ColValue(const string& col_name,uint8 data_type,vector<byte>::iterator begin,int len);//从字节流构造
    ColValue(const string& col_name,float64 value);//从浮点数构造
    ColValue(const string& col_name,int64 value);//从整数构造
    ColValue(const string& col_name,const string& value);//从字符串构造
    void setValue(const string& value);//设置列值
    string getColName() const;//获取列名
    string getValue() const;//获取列值的字符串形式
    int size() const;//获取列值的字节流长度
    pair<string,string> getColValue() const;
    vector<byte> getBytes() const;//获取列值的字节流形式
    uint8 getDataType() const;
    bool isSameType(const ColValue& other) const;
    bool operator==(const ColValue& other) const;
    bool operator!=(const ColValue& other) const;
    bool operator<(const ColValue& other) const;
    bool operator<=(const ColValue& other) const;
    bool operator>(const ColValue& other) const;
    bool operator>=(const ColValue& other) const;
    Key toKey(uint16 len) const;//将列值转换为键值
private:
    string col_name_;//名
    string value_str_;//为字符类型时的存储值
    float64 value_float64_;//为浮点类型时的存储值
    int64 value_int_;//为整型时的存储值
    uint64 value_uint_;//为无符号整型时的存储值
    uint8 data_type_;//数据类型
};