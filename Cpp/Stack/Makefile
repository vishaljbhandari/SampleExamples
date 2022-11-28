CC=g++
CFLAGS=-g -Wall
TARGET=stack
all: $(TARGET)
$(TARGET): src/$(TARGET).cc src/main.cc src/node.cc
	$(CC) $(CFLAGS) -o $(TARGET) src/$(TARGET).cc src/main.cc src/node.cc -I.
clean:
	$(RM) $(TARGET)
