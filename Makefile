CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = main.cpp fframe.cpp contours.cpp contour.cpp log.cpp processor.cpp 
PROG = centers

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

all: $(PROG)
	

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

