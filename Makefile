CXX=g++
CXXFLAGS = -std=c++17
LDFLAGS = -lSDL2 -lSDL2_ttf

TARGET = ./build/rotating_balls.out

SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) examples/rotating_balls.cpp $(LDFLAGS) -o ./build/rotating_ball.out

clean:
	rm -rf $(OBJS) $(TARGET)
