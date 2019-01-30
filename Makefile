CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = log.cpp main.cpp fframe.cpp contours.cpp contour.cpp processor.cpp contour3d.cpp contours3d.cpp 
PROG = contours

OPENCV = `pkg-config opencv --cflags --libs`
LIBS = $(OPENCV)

all: $(PROG)
	

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

