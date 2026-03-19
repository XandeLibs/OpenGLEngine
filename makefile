SRC = src
BUILD = build
CXX = g++
CMAINFLAGS = -Wall -Werror -lglfw -lGL -lpthread -lXrandr -lXi -ldl -I./include -std=c++20
CDEBUGFLAGS = -D_GLIBCXX_DEBUG -g -O0
CRELEASEFLAGS = -O2
COBJFLAGS = -I./include -Wall -Werror -std=c++20

SRCS = $(wildcard $(SRC)/*.cpp)
BUILDS = $(SRCS:$(SRC)/%.cpp=$(BUILD)/%.o)
BUILDSDBG = $(SRCS:$(SRC)/%.cpp=$(BUILD)/%dbg.o)

INCLUDES = $(wildcard include/*.hpp)
INCLUDES += $(wildcard include/*.h)

main: main.cpp $(BUILDS) $(INCLUDES) makefile
	$(CXX) main.cpp -o bin/main $(BUILDS) $(CMAINFLAGS) $(CRELEASEFLAGS) -L./bin -lassimp -Wl,-rpath,'$$ORIGIN'

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CXX) -c $< -o $@ $(COBJFLAGS) $(CRELEASEFLAGS)

debug: main.cpp $(BUILDSDBG)
	$(CXX) main.cpp -g -o bin/main $(BUILDSDBG) $(CMAINFLAGS) $(CDEBUGFLAGS) -L./bin -lassimp -Wl,-rpath,'$$ORIGIN'

$(BUILD)/%dbg.o: $(SRC)/%.cpp
	$(CXX) -c $< -g -o $@ $(COBJFLAGS) $(CDEBUGFLAGS)

clean:
	rm -f build/*.o bin/main
