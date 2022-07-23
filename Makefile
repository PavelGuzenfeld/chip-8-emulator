#chip-8 emlulator makefile
CXX = g++
CC = $(CXX)

WARNINGS = -pedantic
WARNINGS += -Wall -Werror
DEBUG = -g3
CXX_STD = c++17

PROG_NAME   = chip-8-emlulator
TEST_NAME	= utest

BIN_DIR     = ./bin
BUILD_DIR   = ./build
INC_DIR		= ./inc
SRC_DIR     = ./src

MAIN_SRC_PATH = ./src/main/main.cpp
MAIN_BIN_PATH = ./build/main.o

TEST_SRC_PATH = ./test/utest.cpp
TEST_BIN_PATH = ./build/utest.o

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD)
CPPFLAGS = -I $(INC_DIR)
SRC_LIST = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_LIST = $(BUILD_DIR)/$(notdir $(SRC_LIST:.cpp=.o))
BUILD_LIST = $(BUILD_DIR)/*.o
LDLIBS 	 = -lSDL2 -lpthread

all:
	$(foreach SRC_FILE, $(SRC_LIST) ,$(CC) -DNDEBUG -O2 -c $(CPPFLAGS) $(CXXFLAGS) $(SRC_FILE) -o $(BUILD_DIR)/$(notdir $(SRC_FILE:.cpp=.o));)
	$(CC) -DNDEBUG -O2 -c  $(CPPFLAGS) $(CXXFLAGS) $(MAIN_SRC_PATH) -o $(MAIN_BIN_PATH)
	$(CXX) $(BUILD_LIST) -o $(BIN_DIR)/$(PROG_NAME) $(LDLIBS) 
	$(BIN_DIR)/./$(PROG_NAME)

check:
	$(foreach SRC_FILE, $(SRC_LIST) ,$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(SRC_FILE) -o $(BUILD_DIR)/$(notdir $(SRC_FILE:.cpp=.o));)
	$(CC) -c $(CPPFLAGS) $(CXXFLAGS) $(TEST_SRC_PATH) -o $(TEST_BIN_PATH)
	$(CXX) $(BUILD_LIST) -o $(BIN_DIR)/$(TEST_NAME) $(LDLIBS)
	$(BIN_DIR)/$(TEST_NAME) -v

clean:
	rm -f $(BIN_DIR)/$(PROG_NAME) $(BUILD_DIR)/*.o

.PHONY: prod test all clean $(PROG_NAME) compile main