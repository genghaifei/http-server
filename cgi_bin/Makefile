WORK_PATH=$(shell pwd)
CC=gcc
Cx=g++
INCLUDE=-I../ -I../sql_connect/mysql_lib/include -I../sql_connect/
LIB=-L../sql_connect/mysql_lib/lib

.PHONY:all
all:debug math_cgi register show_data_out
debug:debug.c
	@$(CC) -o $@ $^ $(INCLUDE) -gdwarf-2
math_cgi:math_cgi.c
	@$(CC) -o $@ $^ ${INCLUDE} -gdwarf-2
register:register.cpp ../sql_connect/sql_connect.cpp
	@$(Cx) -o $@ $^ $(INCLUDE) $(LIB) -lmysqlclient -gdwarf-2
show_data_out:show_data_out.cpp ../sql_connect/sql_connect.cpp
	@$(Cx) -o $@ $^ $(INCLUDE) $(LIB) -lmysqlclient -gdwarf-2
.PHONY:output
output:
	@cp debug ../htdocs/cgi_bin
	@cp math_cgi ../htdocs/cgi_bin
	@cp register ../htdocs/cgi_bin
	@cp show_data_out ../htdocs/cgi_bin
.PHONY:clean
clean:
	@rm -rf debug math_cgi register show_data_out
