PWD=${shell pwd}
SER_BIN=${PWD}/my_httpd
CLI_BIN=${PWD}/demo_client
SER_SRC=${PWD}/httpd.c
CLI_SRC=${PWD}/demo_client.c
INCLUDE=${PWD}/.
BIN_DIR=${PWD}/cgi_bin
CC=gcc
FLAGS=-w -o
LDFLAGS=-lpthread#-static
LIB=


.PHONY:all
all:${SER_BIN} ${CLI_BIN} cgi

${CLI_BIN}:${CLI_SRC}
	@${CC} ${FLAGS} $@ $^ ${LDFLAGS} #-gdwarf-2

${SER_BIN}:${SER_SRC}
	@${CC} ${FLAGS} $@ $^ ${LDFLAGS} -gdwarf-2
.PHONY:cgi
MAKE=make
cgi:
	cd $(BIN_DIR) && $(MAKE)   
#at first I use for loop to do this thing but I find use it is more simple 
#@for name in `echo $(BIN_DIR)`;\
		do\
			cd $$name && $(MAKE);\
			cd -;\
		done


.PHONY:clean
clean:MAKE=make clean
clean:
	@rm -rf ${CLI_BIN} ${SER_BIN}
	cd $(BIN_DIR) && $(MAKE)
#@for name in `echo ${BIN_DIR}`;\
		do\
			cd $$name;\
			make clean;\
			cd -;\
		done

