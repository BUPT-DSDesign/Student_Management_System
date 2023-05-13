CC = g++
CFLAGS = -O2 -Wall -Wextra -W -Wpedantic

database: bplustree.o table.o db_api.o Interpreter.o SQLstatement.o table.o main.o
	${CC} ${CFLAGS} -o database bplustree.o table.o db_api.o Interpreter.o SQLstatement.o main.o -lm
testsql:db_api.o Interpreter.o SQLstatement.o main.o
	${CC} ${CFLAGS} -o testsql db_api.o Interpreter.o SQLstatement.o main.o -lm
clean:
	${RM} *.o datalink *.log