MAKEFLAGS=--warn-undefined-variables

SOURCE_DIR := src
BUILD := debug
BUILD_DIR := build/$(BUILD)

CXX := g++
CXXFLAGS_MAIN := -Wall -Werror -lglfw -lGL -lpthread -lXrandr -lXi -ldl -std=c++20 -L./lib -lassimp -Wl,-rpath,'$$ORIGIN/../../lib'
CXXFLAGS.debug := -g -O0
CXXFLAGS.release := -O2 -DNDEBUG

CXXFLAGS := -Wall -Werror -std=c++20 -DGLFW_INCLUDE_NONE -MMD $(CXXFLAGS.$(BUILD))

INC_DIRS := $(shell find include -type d)
INC_FLAGS := $(addprefix -I./,$(INC_DIRS))

CXXFLAGS += $(INC_FLAGS)

SOURCE_FILES := $(shell find src -type f -name "*.cpp")

OBJS := $(patsubst $(SOURCE_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCE_FILES))

DEP := $(OBJS:%.o=%.d)

INCLUDES := $(wildcard include/*.hpp)
INCLUDES += $(wildcard include/*.h)

main: build_dir main.cpp $(OBJS) $(INCLUDES) makefile 
	@printf "[Compiling] $@\n"
	@$(CXX) main.cpp -o $(BUILD_DIR)/main $(OBJS) $(CXXFLAGS_MAIN) $(CXXFLAGS)
	@printf "\r[OK] $@\n"

-include $(DEP)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@printf "[Compiling] $<\n"
	@mkdir -p $(dir $@)
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS)
	@printf "[OK] $@\n"

build_dir:
	@mkdir -p $(BUILD_DIR)

clean:
	rm -rf build/*

print:
	echo $(SOURCE_FILES)
	echo $(OBJS)
	echo $(INCLUDES)
	echo $(SOURCE_DIRS)
