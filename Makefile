CC = g++
FLAGS = -std=c++17 -g -Wall

LIBS = -LC:/vcpkg/installed/x86-windows/lib
INCLUDES = -IC:/vcpkg/installed/x86-windows/include
LDLIBS = -lmingw32 -lSDL2

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: build

build: $(OBJS)
	$(CC) $(OBJS) -o bin/main $(LIBS) $(LDLIBS) && start bin/main

%.o: %.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS)
