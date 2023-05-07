#include "db_api.hpp"
#include <filesystem>
DB_API::DB_API(const string& path):path_(path){

}
DB_API::~DB_API(){

}
void DB_API::Quit(){

}
void DB_API::CreateDatabase(SQLCreateDatabase &statement){
    //利用SQLCreateDatabase中的db_name_创建一个文件夹
    filesystem::path db_path(path_ + "/" + statement.get_db_name());
    if(!filesystem::exists(db_path)){
        filesystem::create_directory(db_path);
    }
}

void DB_API::CreateTable(SQLCreateTable &statement){
    //利用SQLCreateTable中的tb_name_创建一个表文件,此处调用bplustree的构造函数
    //同时将表的属性写入表文件
    //TODO

}

void DB_API::CreateIndex(SQLCreateIndex &statement){
    //利用SQLCreateIndex中的tb_name_和col_name_创建一个索引文件
    //索引文件通过调用bplustree的构造函数创建
    //TODO
}

void DB_API::ShowDatabases(){
    //通过搜索path_下的文件夹来展示数据库
    //数据库列表以JSON格式输出到标准输入输出流中
    //JSON格式如下
    // { "databases": [ "db1", "db2", "db3" ] }
    
    cout << "{ \"databases\": [ ";
    for (auto &p : filesystem::directory_iterator(path_))
    {
        cout << "\"" << p.path().filename().string() << "\", ";
    }
    cout << "] }" << endl;
}

void DB_API::ShowTables(){
    //通过搜索path_/db_name下的文件来展示表
    //表列表以JSON格式输出到标准输入输出流中
    //JSON格式如下
    // { "tables": [ "tb1", "tb2", "tb3" ] }
    filesystem::path db_path(path_ + "/" + db_name_);
    cout << "{ \"tables\": [ ";
    for (auto &p : filesystem::directory_iterator(db_path))
    {
        cout << "\"" << p.path().filename().string() << "\", ";
    }
    cout << "] }" << endl;
}

void DB_API::DropDatabase(SQLDropDatabase &statement){
    //利用SQLDropDatabase中的db_name_删除一个文件夹
    filesystem::path db_path(path_ + "/" + statement.get_db_name());
    if(filesystem::exists(db_path)){
        filesystem::remove_all(db_path);
    }
}

void DB_API::DropTable(SQLDropTable &statement){
    //利用SQLDropTable中的tb_name_删除一个表文件
    filesystem::path tb_path(path_ + "/" + statement.get_tb_name());
    if(filesystem::exists(tb_path)){
        filesystem::remove(tb_path);
    }
    //TODO 删除索引文件
}

void DB_API::DropIndex(SQLDropIndex &statement){
    //利用SQLDropIndex中的tb_name_和col_name_删除一个索引文件
    filesystem::path index_path(path_ + "/" + db_name_ + "/" + statement.get_index_name());
    if(filesystem::exists(index_path)){
        filesystem::remove(index_path);
    }
}

void DB_API::Use(SQLUse &statement){
    //利用SQLUse中的db_name_连接上某个数据库
    db_name_ = statement.get_db_name();
}

void DB_API::Insert(SQLInsert &statement){
    //利用SQLInsert中的tb_name_和col_name_插入一条记录
    //需要将其加工为字节流,然后调用BPTree的插入函数
    //最后将其写入表文件
    //TODO
}

void DB_API::Select(SQLSelect &statement){
    //利用SQLSelect中的tb_name_和col_name_查询一条记录
    //需要将其加工为字节流,然后调用BPTree的查询函数
    //最后将返回的json格式的数据输出到标准输入输出流中
    //TODO
}

void DB_API::Delete(SQLDelete &statement){
    //利用SQLDelete中的tb_name_和col_name_删除一条记录
    //需要将其加工为字节流,然后调用BPTree的删除函数
    //最后将其写入表文件
    //TODO
}

void DB_API::Update(SQLUpdate &statement){
    //利用SQLUpdate中的tb_name_和col_name_更新一条记录
    //需要将其加工为字节流,然后调用BPTree的更新函数
    //最后将其写入表文件
    //TODO
}