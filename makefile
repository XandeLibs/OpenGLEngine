MAKEFLAGS=--warn-undefined-variables

SOURCE_DIR := src
RELEASE_DIR := build/release
DEBUG_DIR := build/debug
CXX := g++
CXXFLAGS := -Wall -Werror -std=c++20 -DGLFW_INCLUDE_NONE
CXXFLAGS_MAIN := -Wall -Werror -lglfw -lGL -lpthread -lXrandr -lXi -ldl -std=c++20 -L./lib -lassimp -Wl,-rpath,'$$ORIGIN/../../lib'
CXXFLAGS_DEBUG := -g -O0
CXXFLAGS_RELEASE := -O2

INC_DIRS := $(shell find include -type d)
INC_FLAGS := $(addprefix -I./,$(INC_DIRS))

CXXFLAGS += $(INC_FLAGS)

SOURCE_FILES := $(shell find src -type f -name "*.cpp")

OBJS_RELEASE := $(patsubst $(SOURCE_DIR)/%.cpp, $(RELEASE_DIR)/%.o, $(SOURCE_FILES))
OBJS_DEBUG := $(patsubst $(SOURCE_DIR)/%.cpp, $(DEBUG_DIR)/%.o, $(SOURCE_FILES))

INCLUDES := $(wildcard include/*.hpp)
INCLUDES += $(wildcard include/*.h)

ifneq ($(shell [ -t 1 ] && echo 1),1)
	GREEN :=
	YELLOW :=
	NC :=
	CLEAR := 
else
	GREEN := \033[0;32m
	YELLOW := \033[0;33m
	NC := \033[0m
	CLEAR := \033[2K
endif

main: release_folder main.cpp $(OBJS_RELEASE) $(INCLUDES) makefile 
	@printf "$(YELLOW)[Compiling] $@$(NC)\n"
	@$(CXX) main.cpp -o $(RELEASE_DIR)/main $(OBJS_RELEASE) $(CXXFLAGS_MAIN) $(CXXFLAGS_RELEASE) $(CXXFLAGS)
	@printf "\r$(CLEAR)$(GREEN)[OK]$(NC) $@\n"

$(RELEASE_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@printf "$(YELLOW)[Compiling]$(NC) $<\n"
	@mkdir -p $(dir $@)
	@$(CXX) -c $< -o $@ $(CXXFLAGS) $(CXXFLAGS_RELEASE)
	@printf "$(GREEN)[OK]$(NC) $@\n"

release_folder:
	@mkdir -p $(RELEASE_DIR)

debug: debug_folder main.cpp $(OBJS_DEBUG) $(INCLUDES) makefile
	@printf "$(YELLOW)[Compiling]$(NC) $@\n"
	@mkdir -p $(dir $@)
	@$(CXX) main.cpp -g -o $(DEBUG_DIR)/main $(OBJS_DEBUG) $(CXXFLAGS_MAIN) $(CXXFLAGS_DEBUG) $(CXXFLAGS)
	@printf "$(GREEN)[OK]$(NC) $@\n"

$(DEBUG_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@printf "$(YELLOW)[Compiling]$(NC) $<\n"
	@mkdir -p $(dir $@)
	@$(CXX) -c $< -g -o $@ $(CXXFLAGS) $(CXXFLAGS_DEBUG)
	@printf "$(GREEN)[OK]$(NC) $@\n"

debug_folder:
	@mkdir -p $(DEBUG_DIR)

clean:
	rm -rf $(DEBUG_DIR)/* $(RELEASE_DIR)/*

print:
	echo $(SOURCE_FILES)
	echo $(OBJS_RELEASE)
	echo $(INCLUDES)
	echo $(SOURCE_DIRS)
	echo $(RELEASE_DIRS)
