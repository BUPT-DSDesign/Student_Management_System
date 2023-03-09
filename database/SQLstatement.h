#include "datatype.h"
//创建数据库需要传入的参数
typedef struct
{
    char *db_name;//数据库的名字

}SQLCreateDatabase;
//每一列的属性
typedef struct
{
    uint8 data_type;//数据类型
    bool is_primary;//是否为主键
    uint16 length;//该数据类型长度
    char* col_name;//列名     
}ColAttribute;

//创建表需要的参数
typedef struct
{
    //char *db_name;//数据库的名字
    char *tb_name;//表的名字
    uint8 col_cnt;//列数
    ColAttribute *attr;//表的属性(即每一列的属性)
}SQLCreateTable;

//创建主键需要的参数
typedef struct
{
    //char *db_name;//数据库的名字
    char *tb_name;//表的名字
    char *col_name;//列的名字
    char *index_name;//索引名
}SQLCreateIndex;

//删除数据库需要的参数
typedef struct
{
    char *db_name;//数据库名字
}SQLDropDatabase;

//删除某张表索引所需要的参数
typedef struct
{
    /* data */
    char *index_name;//索引名
}SQLDropIndex;


//链接数据库
typedef struct
{
    char *db_name;//数据库名称
}SQLUse;

//SQL插入的对象
typedef struct 
{
    uint8 data_type;//数据类型
    char *val;//值
}SQLInsertValue;

//增加
typedef struct
{
    char *tb_name;//表名
    uint8 length;//准备新增的数据数量
    SQLInsertValue *values;//待新增的数据
}SQLInsert;

//Where约束
typedef struct
{
	uint8 op_type;//Where约束的运算符类型
	char* key;//对应键值
	char* value;//约束值
}SQLWhere;

typedef struct
{
    
}SQLDelete;
typedef struct
{
    
}SQLUpdate;

typedef struct
{
    /* data */
}SQLSelect;



