CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Iinclude

SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)
BIN := atm

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
