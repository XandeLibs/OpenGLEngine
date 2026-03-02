SRC = src
BUILD = build
CC = g++
CMAINFLAGS = -Wall -Werror -lglfw3 -lGL -lwayland-client -lpthread -lXrandr -lXi -ldl -I./include
COBJFLAGS = -I./include -Wall -Werror

SRCS = $(wildcard $(SRC)/*.cpp)
BUILDS = $(SRCS:$(SRC)/%.cpp=$(BUILD)/%.o)
BUILDSDBG = $(SRCS:$(SRC)/%.cpp=$(BUILD)/%dbg.o)

INCLUDES = $(wildcard include/*.hpp)
INCLUDES += $(wildcard include/*.h)

main: main.cpp $(BUILDS) $(INCLUDES)
	g++ main.cpp -o main $(BUILDS) $(CMAINFLAGS)

$(BUILD)/%.o: $(SRC)/%.cpp
	g++ -c $< -o $@ $(COBJFLAGS)

debug: main.cpp $(BUILDSDBG)
	g++ main.cpp -g -o main $(BUILDSDBG) $(CMAINFLAGS)

$(BUILD)/%dbg.o: $(SRC)/%.cpp
	g++ -c $< -g -o $@ $(COBJFLAGS)

clean:
	rm -f build/*.o main
