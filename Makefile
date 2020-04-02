all:
	g++ joguinho.cpp -o jogo -lGL -lglut -lGLEW -lGLU -lSOIL -lSDL -lSDL_mixer

run:
	./jogo

clean:
	rm jogo