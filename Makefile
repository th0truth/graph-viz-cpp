TARGET = main

CC = g++
BUILD_DIR = ./build
INC_DIRS = ./include ./include/graph
SRC_DIRS = ./src

CPP_SRCS = $(shell find $(SRC_DIRS) -name "*.cpp")
CPP_OBJS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CPP_SRCS))

INC_FLAGS = $(addprefix -I,$(INC_DIRS))
CXXFLAGS = $(INC_FLAGS) -O2 -Wall -std=c++17

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(CPP_OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CPP_OBJS) -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
