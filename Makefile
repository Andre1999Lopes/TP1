all:
	sudo apt-get update
	sudo apt-get install build-essential -y
	sudo apt-get install freeglut3-dev -y
	sudo apt-get install libglew-dev -y
	sudo apt-get install libsoil-dev -y
	sudo apt-get install libsdl-mixer1.2-dev -y
	sudo apt install libsdl2-dev libsdl2-2.0-0 -y
	g++ joguinho.cpp -o jogo -lGL -lglut -lGLEW -lGLU -lSOIL -lSDL -lSDL_mixer
	
run:
	./jogo

clean:
	rm jogo