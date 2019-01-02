LIBARY += -lGL -ldl -lglfw 
FLAGS += -std=c++11 -pthread

all: main.exec

main.exec: main.o src/glad.o src/callback.o
	g++ -o main.exec src/glad.o src/callback.o main.o $(LIBARY) $(FLAGS)

glad.o: glad.c
	g++ -c -Wall -v glad.c $(FLAGS)

callback.o: callback.c
	g++ -c -Wall -v callback.cpp $(FLAGS)

main.o: main.cpp Shader.h src/Boid.h
	g++ -c -Wall -v main.cpp $(FLAGS)

clean:
	rm main.o main.exec src/glad.o src/callback.o
