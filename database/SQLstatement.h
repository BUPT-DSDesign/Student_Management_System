#include "datatype.h"
//创建数据库需要传入的参数
typedef struct CreateDataBaseStatement
{
    char *db_name;//数据库的名字

}SQLCreateDatabase;
//每一列的属性
typedef struct AttributeOfTableColumn
{
    uint8 data_type;//数据类型
    bool is_primary;//是否为主键
    uint16 length;//该数据类型长度
    char* col_name;//列名     
}ColAttribute;

//创建表需要的参数
typedef struct CreateTableStatement
{
    char *db_name;//数据库的名字
    char *tb_name;//表的名字
    uint8 col_cnt;//列数
    ColAttribute *attr;//表的属性(即每一列的属性)
}SQLCreateTable;

//创建主键需要的参数
typedef struct CreateIndexStatement
{
    char *db_name;//数据库的名字
    char *tb_name;//表的名字
    char *col_name;//列的名字
    char *index_name;//索引名
}SQLCreateIndex;