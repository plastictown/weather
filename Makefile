CXX:=g++
CXXFLAGS:=-std=c++17 -O3 -fstack-protector-strong -Wall -Weffc++ -pthread
INCLUDES:=-I.
LIBS:=-lm -lrt -lboost_system -lboost_thread -lpthread
RM:=rm -f
BIN:=weather

all: compile link

compile: weather.o main.o

weather.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) -o weather.o weather.cpp

main.o:
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) -o main.o main.cpp

link:
	$(CXX) $(CXXFLAGS) weather.o main.o $(LIBS) -o $(BIN)

clean:
	$(RM) $(BIN) *.o

