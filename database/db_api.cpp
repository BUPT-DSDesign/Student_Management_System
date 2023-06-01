#include "db_api.hpp"
#include <filesystem>
DB_API::DB_API(const DB_API &api):path_(api.path_),db_name_(api.db_name_){
    //设置数据库所在根目录
    db_manager_ = make_unique<DBmanager>(DBmanager(path_));
}
DB_API::DB_API(const string& path):path_(path){
    //设置数据库所在根目录
    db_manager_ = make_unique<DBmanager>(DBmanager(path));
}
DB_API::~DB_API(){

}
void DB_API::Quit(){
    //退出数据库(与数据库断开链接)
    db_manager_->CloseDatabase();
}
void DB_API::CreateDatabase(SQLCreateDatabase &statement){
    //利用SQLCreateDatabase中的db_name_创建一个文件夹
    db_manager_->CreateDatabase(statement.get_db_name());
}

void DB_API::CreateTable(SQLCreateTable &statement){
    //利用SQLCreateTable中的tb_name_创建一个表文件
    db_manager_->CreateTable(statement.get_tb_name(),statement.get_attr());
    //然后创建索引
    //TODO 索引呢？？？
    vector<pair<string,string>> index_sequence = statement.get_index();
    cerr <<"index_sequence.size() = "<<index_sequence.size()<<endl;
    for(auto &index : index_sequence){
        cerr << "index.first = "<<index.first<<" index.second = "<<index.second<<endl;

        db_manager_->CreateIndex(statement.get_tb_name(),index.first,index.second);
    }
}

void DB_API::CreateIndex(SQLCreateIndex &statement){
    //利用SQLCreateIndex中的tb_name_和col_name_创建一个索引文件
    db_manager_->CreateIndex(statement.get_tb_name(),statement.get_col_name(),statement.get_index_name());
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
    //通过调用DBmanager的ShowTables函数来展示数据库中的表
    //表列表以JSON格式输出到标准输入输出流中
    //JSON格式如下
    // { "tables": [ "tb1", "tb2", "tb3" ] }
    db_manager_->ShowTables();
}

void DB_API::DropDatabase(SQLDropDatabase &statement){
    //利用SQLDropDatabase中的db_name_删除一个文件夹
    db_manager_->DropDatabase(statement.get_db_name());
}

void DB_API::DropTable(SQLDropTable &statement){
    //利用SQLDropTable中的tb_name_删除一个表文件
    db_manager_->DropTable(statement.get_tb_name());
}

void DB_API::DropIndex(SQLDropIndex &statement){
    //利用SQLDropIndex中的tb_name_和col_name_删除一个索引文件
    db_manager_->DropIndex(statement.get_index_name());
}

void DB_API::Use(SQLUse &statement){
    //利用SQLUse中的db_name_连接上某个数据库
    db_manager_->UseDatabase(statement.get_db_name());
}

void DB_API::Insert(SQLInsert &statement){
    //利用SQLInsert中的tb_name_和col_name_插入一条记录
    //需要将其加工为字节流,然后调用Table的插入函数
    //最后将其写入表文件
    vector<pair<string,string>> col_info_ = statement.get_values();
    db_manager_->InsertData(statement.get_tb_name(),col_info_);
}

void DB_API::Select(SQLSelect &statement){
    //利用SQLSelect中的SQLWhere查询记录
    db_manager_->SelectData(statement.get_tb_name(),statement.get_col_name(),statement.get_condition());
}

void DB_API::Delete(SQLDelete &statement){
    //利用SQLDelete中的tb_name_和col_name_删除一条记录
    db_manager_->DeleteData(statement.get_tb_name(),statement.get_condition());
}

void DB_API::Update(SQLUpdate &statement){
    //利用SQLUpdate中的tb_name_和col_name_更新一条记录
    vector<pair<string,string>> col_info_ = statement.get_values();
    db_manager_->UpdateData(statement.get_tb_name(),col_info_,statement.get_condition());
}