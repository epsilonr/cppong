CC = g++
FLAGS = -std=c++17 -g -Wall

LIBS = -LC:/vcpkg/installed/x86-windows/lib
INCLUDES = -IC:/vcpkg/installed/x86-windows/include
LDLIBS = -lmingw32 -lSDL2

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: build

run:
	bin/main

build: $(OBJS)
	$(CC) $(OBJS) -o bin/main $(LIBS) $(LDLIBS) && start bin/main && rm -f $(OBJS)

%.o: %.cpp
	$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

test: test.cpp
	$(CC) $(FLAGS) test.cpp -o out && ./out && rm -f out

clean:
	rm -f $(OBJS)
