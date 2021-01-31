BUILD_DIR = ./build
BIN_DIR   = ./bin
SRC_DIR_HALF_DUPLEX   = ./src/half-duplex
SRC_DIR_FULL_DUPLEX   = ./src/full-duplex
SRC_DIR_FULL_DUPLEX_THREAD   = ./src/full-duplex-thread

CC 		  = gcc
LD        = gcc

CLIENT_HALF_DUPLEX = client_half_duplex
CLIENT_FULL_DUPLEX = client_full_duplex
CLIENT_FULL_DUPLEX_THREAD = client_full_duplex_thread

SERVER_HALF_DUPLEX = server_half_duplex
SERVER_FULL_DUPLEX = server_full_duplex
SERVER_FULL_DUPLEX_THREAD = server_full_duplex_thread

CFLAG = -pthread

run: $(BIN_DIR)/$(CLIENT_HALF_DUPLEX) $(BIN_DIR)/$(CLIENT_FULL_DUPLEX) $(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD) $(BIN_DIR)/$(SERVER_HALF_DUPLEX) $(BIN_DIR)/$(SERVER_FULL_DUPLEX) $(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD)

client_half_duplex: $(BIN_DIR)/$(CLIENT_HALF_DUPLEX)
						./$(BIN_DIR)/$(CLIENT_HALF_DUPLEX)

client_full_duplex: $(BIN_DIR)/$(CLIENT_FULL_DUPLEX)
						./$(BIN_DIR)/$(CLIENT_FULL_DUPLEX)

client_full_duplex_thread: $(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD)
						./$(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD)

server_half_duplex: $(BIN_DIR)/$(SERVER_HALF_DUPLEX)
						./$(BIN_DIR)/$(SERVER_HALF_DUPLEX)

server_full_duplex: $(BIN_DIR)/$(SERVER_FULL_DUPLEX)
						./$(BIN_DIR)/$(SERVER_FULL_DUPLEX)

server_full_duplex_thread: $(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD)
						./$(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD)

$(BIN_DIR)/$(CLIENT_HALF_DUPLEX): $(BUILD_DIR)/client_half_duplex.o
						$(LD) $(BUILD_DIR)/client_half_duplex.o -o $(BIN_DIR)/$(CLIENT_HALF_DUPLEX)

$(BIN_DIR)/$(CLIENT_FULL_DUPLEX): $(BUILD_DIR)/client_full_duplex.o
						$(LD) $(BUILD_DIR)/client_full_duplex.o -o $(BIN_DIR)/$(CLIENT_FULL_DUPLEX)

$(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD): $(BUILD_DIR)/client_full_duplex_thread.o
						$(LD) $(CFLAG) $(BUILD_DIR)/client_full_duplex_thread.o -o $(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD)

$(BIN_DIR)/$(SERVER_HALF_DUPLEX): $(BUILD_DIR)/server_half_duplex.o
						$(LD) $(BUILD_DIR)/server_half_duplex.o -o $(BIN_DIR)/$(SERVER_HALF_DUPLEX)

$(BIN_DIR)/$(SERVER_FULL_DUPLEX): $(BUILD_DIR)/server_full_duplex.o
						$(LD) $(BUILD_DIR)/server_full_duplex.o -o $(BIN_DIR)/$(SERVER_FULL_DUPLEX)

$(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD): $(BUILD_DIR)/server_full_duplex_thread.o
						$(LD) $(CFLAG) $(BUILD_DIR)/server_full_duplex_thread.o -o $(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD)

$(BUILD_DIR)/client_half_duplex.o: $(SRC_DIR_HALF_DUPLEX)/client.c 
						$(CC) -c $(SRC_DIR_HALF_DUPLEX)/client.c -o $(BUILD_DIR)/client_half_duplex.o

$(BUILD_DIR)/client_full_duplex.o: $(SRC_DIR_FULL_DUPLEX)/client.c 
						$(CC) -c $(SRC_DIR_FULL_DUPLEX)/client.c -o $(BUILD_DIR)/client_full_duplex.o

$(BUILD_DIR)/client_full_duplex_thread.o: $(SRC_DIR_FULL_DUPLEX_THREAD)/client.c 
						$(CC) -c $(SRC_DIR_FULL_DUPLEX_THREAD)/client.c -o $(BUILD_DIR)/client_full_duplex_thread.o

$(BUILD_DIR)/server_half_duplex.o: $(SRC_DIR_HALF_DUPLEX)/server.c 
						$(CC) -c $(SRC_DIR_HALF_DUPLEX)/server.c -o $(BUILD_DIR)/server_half_duplex.o

$(BUILD_DIR)/server_full_duplex.o: $(SRC_DIR_FULL_DUPLEX)/server.c 
						$(CC) -c $(SRC_DIR_FULL_DUPLEX)/server.c -o $(BUILD_DIR)/server_full_duplex.o

$(BUILD_DIR)/server_full_duplex_thread.o: $(SRC_DIR_FULL_DUPLEX_THREAD)/server.c 
						$(CC) -c $(SRC_DIR_FULL_DUPLEX_THREAD)/server.c -o $(BUILD_DIR)/server_full_duplex_thread.o

build: $(BIN_DIR)/$(PROG_NAME)

clean: 
		rm -f $(BIN_DIR)/$(CLIENT_HALF_DUPLEX) $(BIN_DIR)/$(CLIENT_FULL_DUPLEX) $(BIN_DIR)/$(CLIENT_FULL_DUPLEX_THREAD) $(BIN_DIR)/$(SERVER_HALF_DUPLEX) $(BIN_DIR)/$(SERVER_FULL_DUPLEX) $(BIN_DIR)/$(SERVER_FULL_DUPLEX_THREAD) $(BUILD_DIR)/*.o