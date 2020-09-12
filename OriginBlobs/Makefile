CXX=g++
CPPFLAGS=-std=c++1y -Wall -Wextra -pedantic -Wno-unused-parameter -Wno-c++98-compat

OUT=OriginBlobs
SRC_DIR=src
BIN_DIR=bin
TEST_DIR=test
INCLUDE_DIR=include

SRC=$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/*.hpp *.ino)
TESTS=$(wildcard $(TEST_DIR)/*.cpp)
TEST_TARGETS=$(patsubst $(TEST_DIR)/%.cpp,$(BIN_DIR)/%,$(TESTS))
OUTPUT_FILES=$(wildcard $(BIN_DIR)/*)

.PHONY: all test clean

all: $(SRC)
	@arduino-cli compile \
		--fqbn esp32:esp32:esp32:PartitionScheme=min_spiffs \
		--output-dir "$(BIN_DIR)/$(OUT)" \
		--warnings all \
		"$(PWD)" \
		2>&1 | sed -E 's%(/.+)+/sketch/%%' 1>&2

up: $(BIN_DIR)/$(OUT)
	screen -X quit; ./upload.sh

test: $(TEST_TARGETS)

$(TEST_TARGETS): $(BIN_DIR)/%: $(TEST_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -D TEST -g -I $(SRC_DIR) -I $(INCLUDE_DIR) $(TEST_DIR)/$*.cpp -o $@

clean:
	rm -rf -v $(OUTPUT_FILES) "$(SRC_DIR)/*.gch"
