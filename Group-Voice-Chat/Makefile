BUILD_DIR   = ./build
BIN_DIR     = ./bin
INCLUDE_DIR = ./include
SRC_DIR     = ./src

CC 		  = gcc
LD        = gcc

CLIENT = client
SERVER = server

CFLAG = -pthread -lpulse -lpulse-simple

run: $(BIN_DIR)/$(CLIENT) $(BIN_DIR)/$(SERVER)

client: $(BIN_DIR)/$(CLIENT)
						./$(BIN_DIR)/$(CLIENT)

server: $(BIN_DIR)/$(SERVER)
						./$(BIN_DIR)/$(SERVER)

$(BIN_DIR)/$(CLIENT): $(BUILD_DIR)/client.o
						$(LD) $(BUILD_DIR)/client.o -o $(BIN_DIR)/$(CLIENT) -I $(INCLUDE_DIR) $(CFLAG)

$(BIN_DIR)/$(SERVER): $(BUILD_DIR)/server.o
						$(LD) $(BUILD_DIR)/server.o -o $(BIN_DIR)/$(SERVER) -I $(INCLUDE_DIR) $(CFLAG)

$(BUILD_DIR)/client.o: $(SRC_DIR)/client.c 
						$(CC) -c $(SRC_DIR)/client.c -o $(BUILD_DIR)/client.o -I $(INCLUDE_DIR)

$(BUILD_DIR)/server.o: $(SRC_DIR)/server.c 
						$(CC) -c $(SRC_DIR)/server.c -o $(BUILD_DIR)/server.o -I $(INCLUDE_DIR)

build: $(BIN_DIR)/$(PROG_NAME)

clean: 
		rm -f $(BIN_DIR)/* $(BUILD_DIR)/*.o