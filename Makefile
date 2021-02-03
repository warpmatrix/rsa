BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

TARGET = $(BIN_DIR)/demo
SRC = $(wildcard $(SRC_DIR)/*.c)
INCLUDE = -I $(INC_DIR)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
CC = gcc

proj: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $^ -o $(TARGET) -lgmp

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(INCLUDE) -c $< -o $@


TEST_BIN_DIR = test/bin
TEST_SRC_DIR = test/.
TEST_BUILD_DIR = test/build

TEST_TARGET = $(TEST_BIN_DIR)/gtest
TEST_SRC = $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJ = $(TEST_SRC:$(TEST_SRC_DIR)/%.cpp=$(TEST_BUILD_DIR)/%.o)
TEST_CC = g++
TEST_FLAGS = -lgtest -lpthread

all: $(TARGET) $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJ) $(filter-out $(BUILD_DIR)/demo.o, $(OBJ))
	@mkdir -p $(TEST_BIN_DIR)
	$(TEST_CC) $^ -o $@ $(TEST_FLAGS) -lgcov

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	@mkdir -p $(TEST_BUILD_DIR)
	$(TEST_CC) $(INCLUDE) $(FLAGS) -c $< -o $@



.PHONY: clean run-test

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@rm -rf $(TEST_BUILD_DIR) $(TEST_BIN_DIR)

run-test: all
	@./test/bin/gtest
