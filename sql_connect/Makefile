WORK_PATH=$(shell pwd)

BIN=sql_connect
CC=g++
INCLUDE= -I${WORK_PATH}/mysql_lib/include -I.
LIB= -L${WORK_PATH}/mysql_lib/lib
FLAG=#-static
SRC=sql_connect.cpp

${BIN}:${SRC}
	${CC} -o $@ $^ ${INCLUDE} ${LIB} -lmysqlclient -gdwarf-2
.PHONY:clean
clean:
	rm -rf ${BIN}
