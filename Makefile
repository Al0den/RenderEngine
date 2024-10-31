GXX=g++
CXXFLAGS=-std=c++20 -fPIC
LDFLAGS=-lSDL2 -lSDL2_ttf

TARGET=build/librenderengine.so

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(GXX) -shared $(OBJS) -o $@ $(LDFLAGS)

%.o: $.cpp
	$(GXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean



