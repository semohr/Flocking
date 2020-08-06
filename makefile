LIBARY += -lGL -ldl -lglfw 
FLAGS += -std=c++11 -pthread
CPPFLAGS += -Iinclude/ -I/usr/include/
CC = g++
all: main.exec

main.exec: main.o src/glad.o src/callback.o
	$(CC) $(CPPFLAGS) -Wall -o main.exec src/glad.o src/callback.o main.o $(LIBARY) $(FLAGS)

glad.o: glad.c
	$(CC) $(CPPFLAGS) -c -Wall -v glad.c $(FLAGS)

callback.o: callback.c
	$(CC) $(CPPFLAGS) -c -Wall -v callback.cpp $(FLAGS)

main.o: main.cpp Shader.h src/Boid.h
	$(CC) $(CPPFLAGS) -c -Wall -v main.cpp $(FLAGS)

clean:
	rm main.o main.exec src/glad.o src/callback.o
