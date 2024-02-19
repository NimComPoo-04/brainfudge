CXX = g++
# CXXFLAGS = -Wall -Wextra -std=c++98 -O2 -pedantic
CXXFLAGS = -Wall -Wextra -ggdb -std=c++98 -O0 -pedantic

OBJ = obj/compiler.o obj/controller.o obj/lexer.o obj/main.o obj/vm.o

all: build bf

build:
	@mkdir -p obj

bf: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ):
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf obj bf

.PHONY: all build clean

obj/compiler.o: compiler.cc compiler.hh lexer.hh Vector.hh vm.hh controller.hh
obj/controller.o: controller.cc controller.hh vm.hh
obj/lexer.o: lexer.cc lexer.hh Vector.hh
obj/main.o: main.cc compiler.hh lexer.hh Vector.hh vm.hh controller.hh
obj/vm.o: vm.cc vm.hh controller.hh
