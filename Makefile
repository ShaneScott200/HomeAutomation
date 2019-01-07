# Defines the compiler to use
CXX = g++
# What flags to pass to the compiler
CXXFLAGS = -Wall -g


# Target (usually start with the main file)
#main: main.o Point.o Rectangle.o
#    $(CXX) $(CXXFLAGS) -o main main.o Point.o Rectangle.o
timeServer: timeServer.o 
	$(CXX) $(CXXFLAGS) -o timeServer timeServer.o  -lmosquittopp


# The next target is each of the object files (main.o, Point.o, Rectangle.o)
#main.o: main.cpp Point.h Rectangle.h
#    $(CXX) $(CXXFLAGS) -c main.cpp
timeServer.o: timeServer.cpp 
	$(CXX) $(CXXFLAGS) -c timeServer.cpp -lmosquittopp
