NAME = tsp

CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O3

S = src
I = include
O = bin

SRCS = $S/graph.cpp $S/main.cpp
INCLUDES = $I/graph.hpp

OBJS = $(patsubst $S/%.cpp,$O/%.o,$(SRCS))

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$O/%.o: $S/%.cpp $(INCLUDES)
	@mkdir -p $O
	$(CXX) $(CXXFLAGS) -I$I -c -o $@ $<

clean:
	@rm -rf $(OBJS) $(NAME)