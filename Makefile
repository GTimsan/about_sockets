.PHONY: all clean build rebuild client server

CC=g++
CFLAGS= -std=c++17
# LDFLAGS= -Wall -Wextra -Werror 
LIB_NAME = lib
COMMON_PATH = ./common/
BUILD_DIR = build
BUILD_PATH = ./$(BUILD_DIR)/
PROGRAM_1_PATH = ./program1/
PROGRAM_2_PATH = ./program2/


all: build

build: $(LIB_NAME).a build_client build_server
# @rm -rf $(BUILD_PATH)*.a $(BUILD_PATH)*.o

$(LIB_NAME).a:
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(COMMON_PATH)my_socket_lib.cc -o $(BUILD_PATH)$(LIB_NAME).o
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(PROGRAM_1_PATH)client.cc -o $(BUILD_PATH)client.o
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(COMMON_PATH)socketinterface.cc -o $(BUILD_PATH)socketinterface.o
	@ar -rcs $(BUILD_PATH)$@ $(BUILD_PATH)$(LIB_NAME).o $(BUILD_PATH)client.o $(BUILD_PATH)socketinterface.o

build_client:
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(PROGRAM_1_PATH)input.cc -o $(BUILD_PATH)input.o
	@$(CC) $(CFLAGS) -pthread $(BUILD_PATH)input.o $(BUILD_PATH)$(LIB_NAME).a  -o $(BUILD_PATH)client
	
build_server:
	@$(CC) $(CFLAGS) $(LDFLAGS) -c $(PROGRAM_2_PATH)server.cc -o $(BUILD_PATH)server.o
	@$(CC) $(CFLAGS) $(BUILD_PATH)server.o $(BUILD_PATH)$(LIB_NAME).a -o $(BUILD_PATH)server

client:
	$(BUILD_PATH)client

server:
	$(BUILD_PATH)server

clean:
	rm -rf $(BUILD_PATH)*

rebuild: clean all


# ps -a -o pid,ppid,tty,stat,args,wchan
