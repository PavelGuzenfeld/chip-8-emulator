#chip-8 emlulator makefile
CXX = g++
CC = $(CXX)

WARNINGS = -pedantic
WARNINGS += -Wall -Werror
DEBUG = -g3
CXX_STD = c++17

PROG_NAME   = chip-8-emlulator

BIN_DIR     = ./bin
BUILD_DIR   = ./build
INC_DIR		= ./inc
SRC_DIR     = ./src

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD)
CPPFLAGS = -I $(INC_DIR)
SRC_LIST = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST = $(BUILD_DIR)/$(notdir $(SRC_LIST:.cpp=.o))
LDLIBS 	 = -lSDL2

.PHONY: all clean $(PROG_NAME) compile

all: $(PROG_NAME)
	$(BIN_DIR)/./$(PROG_NAME)

compile:
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(SRC_LIST) -o $(OBJ_LIST)

$(PROG_NAME): compile
	$(CXX) $(OBJ_LIST) -o $(BIN_DIR)/$@ $(LDLIBS)

clean:
	rm -f $(BIN_DIR)/$(PROG_NAME) $(BUILD_DIR)/*.o