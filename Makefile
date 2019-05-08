CC = g++
CFLAGS = -g -Wall -std=c++17
SRCS = log.cpp main.cpp fframe.cpp contours.cpp contour.cpp processor.cpp contour3d.cpp contours3d.cpp tests.cpp disparity.cpp crtree.cpp height.cpp
PROG = centers

OPENCV = `pkg-config opencv --cflags --libs`
BOOST = -I/usr/include/boost   
LIBS = $(OPENCV) $(BOOST)

all: $(PROG)
	

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)

