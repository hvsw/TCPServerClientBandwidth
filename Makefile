# Constants
CC = g++
SRC_DIR = ./src
DIST_DIR = ./dist
LOG_DIR = ./log

# Rules
# target: dependencies
#     system command(s)
all: server client

server: $(SRC_DIR)/server.cpp
	$(CC) -o $(DIST_DIR)/server $(SRC_DIR)/server.cpp -pthread

client:	$(SRC_DIR)/client.cpp
	$(CC) -o $(DIST_DIR)/client $(SRC_DIR)/client.cpp -pthread

clean:
	rm $(DIST_DIR)/*
	rm $(LOG_DIR)/*.csv