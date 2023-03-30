#include "datatype.hpp"
#include "bplustree.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>
using namespace std;
BPTree::BPTree(string tbname)
{
    string_view table_path = tbname + ".db";
    table_ = make_unique<fstream>(table_path.data(),ios::binary|ios::out|ios::in);
    if (!table_->good()) {
        cout << "Table does not exist." << endl;
        exit(1);
    }
    //读取列数量
    int col_num = 0;
    table_->read((char*)&col_num,sizeof(col_num));
    //cout<<"有"<<col_num<<"列\n";
    //(*table_item_)>>col_num;
    //开始读取表属性
    TableColAttribute buf_atr;
    for(int i=0;i<col_num;i++){
        table_->read((char*)&buf_atr,sizeof(buf_atr));
        col_info_.push_back(buf_atr);
    }
    //记录B+树根节点位置
    root_pos_ = table_->tellg();
}
BPTree::BPTree(string tbname,vector<TableColAttribute> &col_info)
{
    string_view table_path = tbname + ".db";
    table_ = (make_unique<fstream>(table_path.data(),ios::binary|ios::out|ios::in));
    //写入列长度
    int col_num = col_info.size();
    table_->write((char*)&col_num,sizeof(col_num));
    //写入每一列的属性
    for(auto &t:col_info){
        table_->write((char*)&t,sizeof(t));
        col_info_.push_back(t);
    }
    //记录B+树根节点位置
    root_pos_ = table_->tellg();
}
BPTree::~BPTree(){
    table_->close();
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