CXX = g++
CXXFLAGS := -Wall -Wextra -std=c++23 -Iinclude # -O3
SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:.cpp=.o)

tiko: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f src/*.o tiko
