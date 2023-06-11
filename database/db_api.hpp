#pragma once
#include "SQLstatement.hpp"
#include "DBmanager.hpp"
#include <memory>
// 提供了数据库增删改查操作所需的API
class DB_API
{
private:
    // 我们使用如下两个变量确定数据库位置
    string path_; // 数据库所在文件夹
    string db_name_; // 数据库名字
    unique_ptr<DBmanager> db_manager_; // 数据库管理器
public:
    DB_API(const string &path);
    DB_API(const DB_API &api);
    ~DB_API();
    // 退出数据库(与数据库断开链接)
    void Quit();

    // 创建数据库(返回值待定)
    void CreateDatabase(SQLCreateDatabase &statement);

    // 创建表(应该传入一个能描述表结构的结构体)
    void CreateTable(SQLCreateTable &statement);

    // 为表创建一个索引,应该传入表和相关的信息
    void CreateIndex(SQLCreateIndex &statement);

    // 显示数据库条目
    void ShowDatabases();

    // 展示数据库中表的详细信息
    void ShowTables();

    // 删除数据库
    void DropDatabase(SQLDropDatabase &statement);

    // 删除表
    void DropTable(SQLDropTable &statement);

    // 删除某张表的索引，应该传入表和相关的信息
    void DropIndex(SQLDropIndex &statement);

    // 连接上某个数据库，应该返回一个可以操作数据库的指针
    void Use(SQLUse &statement);

    // 增删改查部分

    /*
    增加
   */
    void Insert(SQLInsert &statement);

    /*
    删除
    */
    void Delete(SQLDelete &statement);

    /*
    更改
    */
    void Update(SQLUpdate &statement);
    /*
    查询
    */
    void Select(SQLSelect &statement);
};
