CXX = g++
CFLAGS = -std=c++11 -o2 -Wall -g  -I/usr/include/mysql/ -L/usr/lib/x86_64-linux-gnu 
TARGET = Poll
OBJS = ../code/*.cpp ../code/connpool/*.cpp  ../code/mysqlconn/*.cpp
all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS)  -lmysqlclient  -fsanitize=address -fsanitize=leak -fsanitize=undefined -o ../bin/$(TARGET) 
clean:
	rm -rf ../bin/*.o $(TARGET)