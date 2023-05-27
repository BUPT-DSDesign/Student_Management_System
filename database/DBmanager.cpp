#include "DBmanager.hpp"
#include "MyException.hpp"
#include <filesystem>
#include <algorithm>
DBmanager::DBmanager():path_(""){

}
DBmanager::DBmanager(const string& path):path_(path)
{
    //设置数据库所在根目录
}
void DBmanager::UseDatabase(const string& db_name)
{
    //打开数据库
    //先从磁盘中检查是否存在数据库,不存在则丢出异常
    filesystem::path db_path(path_ + "/" + db_name);
    if(!filesystem::exists(db_path)){
        throw DBManagerError("Database " + db_name + " does not exist!");
    }
    db_name_ = db_name;
    //打开数据库中的表,表的后缀名为.table
    for (auto &p : filesystem::directory_iterator(db_path))
    {
        if(p.path().extension() == ".table"){
            string tb_name = p.path().filename().string();
            tb_name_list_.push_back(tb_name);
            tb_list_[tb_name] = make_unique<Table>(Table(db_path.string(),tb_name));
        }
    }
}
void DBmanager::CreateDatabase(const string& db_name)
{
    //新建数据库
    filesystem::path db_path(path_ + "/" + db_name);
    if(!filesystem::exists(db_path)){
        filesystem::create_directory(db_path);
        db_name_ = db_name;
    }
    else{
        throw DBManagerError("Database " + db_name + " already exists!");
    }
}
void DBmanager::DropDatabase(const string& db_name)
{
    //删除数据库
    filesystem::path db_path(path_ + "/" + db_name);
    if(filesystem::exists(db_path)){
        filesystem::remove_all(db_path);
        db_name_ = "";
    }
    else{
        throw DBManagerError("Database " + db_name + " does not exist!");
    }
}
void DBmanager::CloseDatabase()
{
    //关闭数据库
    db_name_ = "";
}
void DBmanager::ShowTables()
{
    //展示数据库中的表
    //表列表以JSON格式输出到标准输入输出流中
    //JSON格式如下
    // { "tables": [ "tb1", "tb2", "tb3" ] }
    cout << "{ \"tables\": [ ";
    for (auto &p : tb_name_list_)
    {
        cout << "\"" << p << "\", ";
    }
    cout << "] }" << endl;
}
void DBmanager::CreateTable(const string& tb_name,vector<TableColAttribute> &col_info)
{
    //新建表
    //先检查表是否存在,若存在则抛出异常
    if(tb_list_.find(tb_name) != tb_list_.end()){
        throw DBManagerError("Table " + tb_name + " already exists!");
    }
    tb_name_list_.push_back(tb_name);
    tb_list_[tb_name] = make_unique<Table>(Table(path_ + "/" + db_name_,tb_name,col_info));
}
void DBmanager::DropTable(const string& tb_name)
{
    //删除表
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(tb_name) == tb_list_.end()){
        throw DBManagerError("Table " + tb_name + " does not exist!");
    }
    tb_name_list_.erase(find(tb_name_list_.begin(),tb_name_list_.end(),tb_name));
    //调用Table的删除函数,以删除表文件和对应索引文件
    tb_list_[tb_name]->DropTable();
    tb_list_.erase(tb_name);
}
void DBmanager::CreateIndex(const string& tb_name,const string& col_name,const string& index_name)
{
    //新建索引
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(tb_name) == tb_list_.end()){
        throw DBManagerError("Table " + tb_name + " does not exist!");
    }
    //调用Table的新建索引函数
    tb_list_[tb_name]->CreateIndex(col_name,index_name);
}
void DBmanager::DropIndex(const string& index_name)
{
    //删除索引
    //先去目录下检查索引是否存在,若不存在则抛出异常,后缀名为.idx
    filesystem::path index_path(path_ + "/" + db_name_ + "/" + index_name + ".idx");
    if(!filesystem::exists(index_path)){
        throw DBManagerError("Index " + index_name + " does not exist!");
    }
    //删除索引文件
    filesystem::remove(index_path);
    //索引信息的删除由Table类完成,这里不做处理
}
void DBmanager::InsertData(string table_name, vector<pair<string,string>> &col_item)
{
    //插入数据
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(table_name) == tb_list_.end()){
        throw DBManagerError("Table " + table_name + " does not exist!");
    }
    //调用Table的插入数据函数
    tb_list_[table_name]->InsertRecord(col_item);
}
void DBmanager::DeleteData(string table_name, SQLWhere &where)
{
    //删除数据
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(table_name) == tb_list_.end()){
        throw DBManagerError("Table " + table_name + " does not exist!");
    }
    //调用Table的删除数据函数
    tb_list_[table_name]->DeleteRecord(where);
}
void DBmanager::SelectData(string table_name, vector<string> &col_name, SQLWhere &where)
{
    //查询数据
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(table_name) == tb_list_.end()){
        throw DBManagerError("Table " + table_name + " does not exist!");
    }
    //调用Table的查询数据函数
    //TODO 除了SELECT * 还有其它的方式,后续有空支持下
    tb_list_[table_name]->SelectRecord(where);
}
void DBmanager::UpdateData(string table_name, vector<pair<string,string>> &col_item, SQLWhere &where)
{
    //更新数据
    //先检查表是否存在,若不存在则抛出异常
    if(tb_list_.find(table_name) == tb_list_.end()){
        throw DBManagerError("Table " + table_name + " does not exist!");
    }
    //调用Table的更新数据函数
    tb_list_[table_name]->UpdateRecord(col_item,where);
}