#pragma once
#include "SQLstatement.hpp"
#include <memory>
// 提供了数据库增删改查操作所需的API
class DB_API
{
private:
    // 我们使用如下两个变量确定数据库位置
    string path_; // 数据库所在文件夹
    string db_name_; // 数据库名字
public:
    DB_API(const string &path);
    ~DB_API();
    // 退出数据库(与数据库断开链接)
    void Quit();

    // 创建数据库(返回值待定)
    void CreateDatabase(SQLCreateDatabase &statement);

    // 创建表(应该传入一个能描述表结构的结构体)
    void CreateTable(SQLCreateTable &statement);

    // 为表创建一个索引,应该传入表和相fA关的信息
    void CreateIndex(SQLCreateIndex &statement);

    // 显示数据库条目
    void ShowDatabases();

    // 展示数据库中表的详细信息
    void ShowTables(string &db_name);

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
    操作对象:条目
    在数据库中,增加(Insert Into)语句仅仅只和条目的增加有关
    主要有以下三种增加模式:
    -- 写全所有列名
    insert into 表名(列名1,列名2,...列名n) values(值1,值2,...值n);
    -- 不写列名（所有列全部添加）
    insert into 表名 values(值1,值2,...值n);
    -- 插入部分数据
    insert into 表名(列名1,列名2) values(值1,值2);
    故传入参数主要应该为表名，设计到的列，所需要传入数值
    同时这种方式只能一条一条添加
    */
    void Insert(SQLInsert &statement);

    /*
    删除
    操作对象:条目
    限定条件: Where
    在实际数据库操作中,多会使用WHERE来限制所删除条目的范围
    我们主要实现对多个比较运算符的解析
    */
    void Delete(SQLDelete &statement);

    /*
    更改
    操作对象:条目
    限定条件: Where
    在实际数据库操作中,更改和删除所要解析的条件子句相似,故两者可以互相参考
    */
    void Update(SQLUpdate &statement);
    /*
    查询
    操作对象:条目
    限定条件:WHERE
    操作:排序ORDER BY,分组GROUP BY
    查询设计到的限制条件同上，但会对返回数据有要求，这将是一大难点
    */
    void Select(SQLSelect &statement);
};
