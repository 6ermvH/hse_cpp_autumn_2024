CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -I include
BUILD_DIR = build
SRC_DIR = src

SRCS = $(SRC_DIR)/bigint.cpp $(SRC_DIR)/main.cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: main

main: $(OBJS)
	$(CXX) $(OBJS) -o $@ -lgtest_main -lgtest -lpthread

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
