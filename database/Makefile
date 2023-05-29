#编译器和选项
CC = g++
CFLAGS = -O2 -Wall -Wextra -W -Wpedantic
#宏定义
DEBUG = -DMY_MACRO -D TEST_SQL

database: bplustree.o table.o SQLWhere.o DBManager.o db_api.o Interpreter.o SQLstatement.o main.o 
	${CC} ${CFLAGS} -o database bplustree.o table.o SQLWhere.o DBManager.o db_api.o Interpreter.o SQLstatement.o main.o -lm
testsql: Interpreter.o.test SQLstatement.o.test main.o
	${CC} ${CFLAGS} ${DEBUG} -o testsql Interpreter.o SQLstatement.o main.o -lm
Interpreter.o.test: Interpreter.cpp
	${CC} ${CFLAGS} ${DEBUG} -c Interpreter.cpp
SQLstatement.o.test: SQLstatement.cpp
	${CC} ${CFLAGS} ${DEBUG} -c SQLstatement.cpp
SQLstatement.o: SQLstatement.cpp
	${CC} ${CFLAGS} -c SQLstatement.cpp
main.o: main.cpp
	${CC} ${CFLAGS} -c main.cpp
bplustree.o: bplustree.cpp
	${CC} ${CFLAGS} -c bplustree.cpp
table.o: table.cpp
	${CC} ${CFLAGS} -c table.cpp
db_api.o: db_api.cpp
	${CC} ${CFLAGS} -c db_api.cpp
Interpreter.o: Interpreter.cpp
	${CC} ${CFLAGS} -c Interpreter.cpp
clean:
	${RM} *.o datalink *.log