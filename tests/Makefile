GXX=g++
CXXFLAGS=-std=c++20
LDFLAGS=-lSDL2 -lSDL2_ttf

TARGET=test

SRCS= $(wildcard ../src/*.cpp) $(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)


all: $(TARGET)

$(TARGET): $(OBJS)
	$(GXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.cpp
	$(GXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean

