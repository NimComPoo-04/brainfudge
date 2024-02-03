CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -pedantic
# CXXFLAGS = -Wall -Wextra -std=c++17 -O2 -pedantic

SRC = $(wildcard *.cc)
OBJ = $(patsubst %.cc, obj/%.o, $(SRC))

all: build bf

build:
	@mkdir -p obj

bf: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

obj/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $^

clean:
	rm -rf obj bf

.PHONY: all build clean
