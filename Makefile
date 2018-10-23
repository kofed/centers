CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = main.cpp centers.cpp
PROG = centers

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

all: $(PROG)
	

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

