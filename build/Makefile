CXX = g++
CFLAGS = -std=c++11 -o2 -Wall -g -lmysqlclient
TARGET = Poll
OBJS = ../code/*.cpp ../code/connpool/*.cpp  ../code/mysqlconn/*.cpp
all: $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o ../bin/$(TARGET) 
clean:
	rm -rf ../bin/*.o $(TARGET)