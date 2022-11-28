CC=g++
RM=rm
CFLAGS=-g -Wall
CLIENT_TARGET=client
SERVER_TARGET=server
all: $(SERVER_TARGET) $(CLIENT_TARGET)
$(SERVER_TARGET): src/$(SERVER_TARGET)_main.cc src/file.cc
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) src/$(SERVER_TARGET)_main.cc src/file.cc -I.
$(CLIENT_TARGET): src/$(CLIENT_TARGET)_main.cc src/file.cc
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) src/$(CLIENT_TARGET)_main.cc src/file.cc -I.
clean:
	$(RM) $(SERVER_TARGET) $(CLIENT_TARGET)
