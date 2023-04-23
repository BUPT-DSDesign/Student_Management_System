#pragma once
//一些数据类型的重命名,便于移植/对接golang
//以下为按照Golang习惯,对数据类型进行的重命名
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef float float32;
typedef double float64;
//typedef unsigned char byte;
typedef int rune;

//接下来引入数据库中常见的数据类型
//MySQL中数据类型编号
//数值类型
#define T_TINY_INT 0x0
#define T_SMALL_INT 0x1
#define T_INT 0x2
#define T_BIG_INT 0x3
#define T_FLOAT 0x4
#define T_DOUBLE 0x5
//日期和时间类型
#define T_DATE 0x10
#define T_TIME 0x11
#define T_YEAR 0x12
#define T_DATE_TIME 0x13
#define T_TIMESTAMP 0x14
//字符串类型
#define T_CHAR 0x20
#define T_VARCHAR 0x21

//接下来给出这些数据的大小
const int SIZE_OF_T[]=
{
    /*      0     1 2 3 4 5 6 7 8 9 a b c d e f*/
    /*0x*/  1,    2,4,8,4,8,0,0,0,0,0,0,0,0,0,0,
    /*1x*/  3,    3,1,8,4,0,0,0,0,0,0,0,0,0,0,0,
    /*2x*/255,65535,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

//以下为SQL语句关键字的声明
//数据定义语言DDL
#define SQL_CREATE 0x00
#define SQL_CREATE_DATABASE 0x01
#define SQL_CREATE_TABLE 0x02
#define SQL_CREATE_INDEX 0x03
#define SQL_DROP 0x10
#define SQL_DROP_DATABASE 0x11
#define SQL_DROP_TABLE 0x12
#define SQL_DROP_INDEX 0x13
#define SQL_ALTER 0x20
//数据查询语言DQL
#define SQL_SELECT 0x30

//数据操纵语言DML
#define SQL_INSERT 0x40
#define SQL_UPDATE 0x50
#define SQL_DELETE 0x60

//其它操作
#define SQL_SHOW_DATABASES 0x71
#define SQL_SHOW_TABLES 0x72
#define SQL_QUIT 0x80
#define SQL_USE  0x90

//错误处理
#define SQL_ERROR 0xFF

//条件(Condition)运算符
#define OP_EQUAL 0x00
#define OP_GREATER 0x01
#define OP_GREATER_EQUAL 0x02
#define OP_LESS 0x03
#define OP_LESS_EQUAL 0x04
#define OP_NOT_EQUAL 0x05
#define OP_AND 0x06
#define OP_OR 0x07
