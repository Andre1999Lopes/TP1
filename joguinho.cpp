#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#define vetorInimigos 60
using namespace std;
#include "global.h"
#include "utility.h"
#include "callbacks.cpp"
#include "gaming.cpp"

/*
	Fontes das imagens e músicas:

	*Música de batalha: https://www.youtube.com/watch?v=FvG4ZjHbkLg
	*Música do menu: https://www.youtube.com/watch?v=bOYdk1UY5o8
	*Efeito sonoro do tiro: https://youtu.be/Dav5kn_8BmY
	*Imagem logo: https://www.deviantart.com/left4cake008/art/Star-Wars-Episode-8-bit-582007343
	*Fonte das imagens de opções e instruções: joystix monospace
*/

int main(int argc, char **argv){
	glutInit(&argc, argv);
	SDL_Init(SDL_INIT_AUDIO);
	Mix_Init(MIX_INIT_MP3);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920,1080);
	glutCreateWindow("Star Wars - the game");
	setup();
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialUpFunc(specialKeyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutTimerFunc(0,atualizaCena,33);
	glutMainLoop();
}