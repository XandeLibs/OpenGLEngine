SRC = src
PATH_RELEASE = build/release
PATH_DEBUG = build/debug
CXX = g++
CMAINFLAGS = -Wall -Werror -lglfw -lGL -lpthread -lXrandr -lXi -ldl -I./include -std=c++20 -L./lib -lassimp -Wl,-rpath,'$$ORIGIN/../../lib'
CDEBUGFLAGS = -g -O0
CRELEASEFLAGS = -O2
COBJFLAGS = -I./include -Wall -Werror -std=c++20

SRCS = $(wildcard $(SRC)/*.cpp)
BUILDS = $(SRCS:$(SRC)/%.cpp=$(PATH_RELEASE)/%.o)
BUILDSDBG = $(SRCS:$(SRC)/%.cpp=$(PATH_DEBUG)/%.o)

INCLUDES = $(wildcard include/*.hpp)
INCLUDES += $(wildcard include/*.h)

main: main.cpp $(BUILDS) $(INCLUDES) makefile release_folder
	$(CXX) main.cpp -o $(PATH_RELEASE)/main $(BUILDS) $(CMAINFLAGS) $(CRELEASEFLAGS)

$(PATH_RELEASE)/%.o: $(SRC)/%.cpp
	$(CXX) -c $< -o $@ $(COBJFLAGS) $(CRELEASEFLAGS)

release_folder:
	mkdir -p $(PATH_RELEASE)

debug: main.cpp $(BUILDSDBG) $(INCLUDES) makefile debug_folder
	$(CXX) main.cpp -g -o $(PATH_DEBUG)/main $(BUILDSDBG) $(CMAINFLAGS) $(CDEBUGFLAGS)

$(PATH_DEBUG)/%.o: $(SRC)/%.cpp
	$(CXX) -c $< -g -o $@ $(COBJFLAGS) $(CDEBUGFLAGS)

debug_folder:
	mkdir -p $(PATH_DEBUG)

clean:
	rm -rf $(PATH_DEBUG)/* $(PATH_RELEASE)/*
