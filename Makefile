CXX=g++
CXXFLAGS = -std=c++17
LDFLAGS = -lrenderer -lSDL2 -lSDL2_ttf

TARGET = ./lib/librenderer.a

SRCS = $(wildcard ./src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^
clean:
	rm -rf $(OBJS) $(TARGET)

rotating_ball: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) examples/rotating_ball.cpp -o ./build/rotating_ball.out
