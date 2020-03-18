#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>

int pausa=0;
int direita;
int esquerda;
float larguraTela=1920;
float alturaTela=1080;
float movimento=10;

typedef struct Player{
	float posicaoX;
	float posicaoY;
	int vida;
	int larg;
	int alt;
	GLuint textura;
} Player;

typedef struct Enemies{
	float posicao;
	int larg;
	int alt;
	GLuint textura;
} Enemies;

Player jogador;
Enemies inimigos[40];

void mover(){
	if(direita==1 && jogador.posicaoX<1700)
		jogador.posicaoX+=movimento;
	if(esquerda==1 && jogador.posicaoX>200)
		jogador.posicaoX-=movimento;
}

GLuint carregaTexturas(const char *arquivo){
	GLuint idTextura = SOIL_load_OGL_texture(arquivo, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if(idTextura==0)
		printf("Erro ao carregar a textura: %s\n", SOIL_last_result());
	return idTextura;
}

void iniciarJogador(){
	jogador.posicaoX=960;
	jogador.posicaoY=200;
	jogador.larg=100;
	jogador.alt=100;
	jogador.vida=2;
	jogador.textura = carregaTexturas("mfalcon.png");
}

void desenharRetanguloTextura(float x, float y, float larg, float alt, GLuint textura){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glPushMatrix();
        glTranslatef(x, y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0,0); glVertex2f(-larg/2, -alt/2);
            glTexCoord2f(1,0); glVertex2f( larg/2, -alt/2);
            glTexCoord2f(1,1); glVertex2f( larg/2,  alt/2);
            glTexCoord2f(0,1); glVertex2f(-larg/2,  alt/2);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void iniciarTexturas(){
	//iniciar a textura do fundo, do menu (se tiver) e do restante
}

void setup(){
	glClearColor(1,1,1,1);
	iniciarJogador();
	//iniciarTexturas();
}

void reshape(int width, int height){
	glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
	desenharRetanguloTextura(jogador.posicaoX,jogador.posicaoY,jogador.larg,jogador.alt,jogador.textura);
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'a':
		case 'A':
			esquerda=1;
			break;
		case 'd':
		case 'D':
			direita=1;
			break;
		case 27:
			//printf("Deseja mesmo sair?\n");
			//if()
				exit(0);
			break;
		case 32: //barra de espaço
			//atirar
		case 'p':
		case 'P':
			if(pausa==0)
				pausa=1;
			else
				pausa=0;
			break;
		default:
			break;
	}
}

void keyboardUp(unsigned char key, int x, int y){
	switch(key){
		case 'a':
		case 'A':
			esquerda=0;
			break;
		case 'd':
		case 'D':
			direita=0;
			break;
		default:
			break;
	}
}

void specialKeyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=1;
			break;
		case GLUT_KEY_RIGHT:
			direita=1;
			break;
	}
}

void specialKeyboardUp(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=0;
			break;
		case GLUT_KEY_RIGHT:
			direita=0;
			break;
	}
}

void atualizaCena(int tempo){
	mover();
	glutPostRedisplay();
	glutTimerFunc(33, atualizaCena, 0);
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920,1080);
	glutCreateWindow("Galaxian");
	setup();
	glutDisplayFunc(draw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialUpFunc(specialKeyboardUp);
	glutSpecialFunc(specialKeyboard);
	glutTimerFunc(0,atualizaCena,33);
	glutMainLoop();
}