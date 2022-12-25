#!/bin/bash

echo -e "First run dos2unix makefile"

declare -x LINUX_SBIN=".sbin_ln"
mkdir $LINUX_SBIN

declare -x CC="g++"
declare -x CFLAGS="-g -Wall"
declare -x ADDITIONAL_FLAGS="-Wno-unused-variable -Wno-unused-but-set-variable"
declare -x TARGET="sample"
all: $(TARGET)
$(TARGET): $(TARGET).cpp
	echo -e $(CC) $(CFLAGS) $(ADDITIONAL_FLAGS) -o $(LINUX_SBIN)/$(TARGET) main.cpp $(TARGET).cpp -I.
	$(CC) $(CFLAGS) $(ADDITIONAL_FLAGS) -o $(LINUX_SBIN)/$(TARGET) main.cpp $(TARGET).cpp -I.
clean:
	echo -e $(RM) $(LINUX_SBIN)/$(TARGET)
	$(RM) $(LINUX_SBIN)/$(TARGET)
	
# Single Line g++ command
# $(CC) $(CFLAGS) $(ADDITIONAL_FLAGS) -o $(LINUX_SBIN)/$(TARGET) main.cpp $(TARGET).cpp -I.
# g++ $(CFLAGS) $(ADDITIONAL_FLAGS) -o $(LINUX_SBIN)/sample main.cpp sample.cpp -I.
# g++ -g -Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-but-set-variable -o .sbin_ln/sample main.cpp sample.cpp -I.

# g++ clean $(LINUX_SBIN)/$(TARGET)
# g++ clean .sbin_ln/sample
