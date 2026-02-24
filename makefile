main: src/main.cpp glad.o
	g++ src/main.cpp -o main build/glad.o -Wall -lglfw3 -lGL -lwayland-client -lpthread -lXrandr -lXi -ldl -I./include

glad.o:
	g++ -c src/glad.c -I./include -o build/glad.o

clean:
	rm -f build/*.o main
