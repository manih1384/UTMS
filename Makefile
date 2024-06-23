
CXX = g++


CXXFLAGS = -Wall -g -std=c++20


SRC_DIRS = src server utils
OBJ_DIR = obj
BIN_DIR = bin


TARGET = utms.out


SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))


OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)


all: $(TARGET)


$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<


clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)/$(TARGET)


.PHONY: all clean