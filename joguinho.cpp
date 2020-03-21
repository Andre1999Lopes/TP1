#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <vector>
#include <unistd.h>
#define vetorInimigos 60

using namespace std;
void desenhaTiro();
GLuint carregaTexturas(const char *arquivo);
void verificaPosicao();
GLboolean checarColisao();
void iniciarJogador();
void iniciarInimigos();
int pausa=0;
int direita;
int esquerda;
float larguraTela=1920;
float alturaTela=1080;
float movimentoJogador=10;
float movimentoInimigos=4;
int atirou = 0;
float incrementoX;
float decrementoY;

typedef struct Player{
	float posicaoX;
	float posicaoY;
	int vida;
	int larg;
	int alt;
	GLuint textura;
} Player;

typedef struct Enemies{
	float posicaoX;
	float posicaoY;
	int larg=50;
	int alt=50;
	GLuint textura;
} Enemies;

typedef struct Bullet{
	float x;
	float y;
}Bullet;

vector<Bullet> bullets;
Player jogador;
vector<Enemies> inimigos;

void resetar(){
	inimigos.clear();
	bullets.clear();
	iniciarJogador();
	iniciarInimigos();
	if(movimentoInimigos<0)
		movimentoInimigos*=-1;
}

void verificaPosicao(){
	for(int i=0;i<inimigos.size();i++){
		if(inimigos[i].posicaoX>=1720){
			movimentoInimigos*=-1;
			for(int j=0;j<inimigos.size();j++){
				inimigos[j].posicaoY-=20;
			}
			break;
		}
		else if(inimigos[i].posicaoX<=200){
			movimentoInimigos*=-1;
			for(int j=0;j<inimigos.size();j++){
				inimigos[j].posicaoY-=20;
			}
			break;
		}
	}
}

void moverNaves(){
	for(int i=0;i<inimigos.size();i++){
		inimigos[i].posicaoX+=movimentoInimigos;
	}
}

void iniciarInimigos(){
	incrementoX=0;
	decrementoY=0;
	int cont=0;
	for(int i=0;i<vetorInimigos;i++){
		Enemies inimigo;
		inimigo.posicaoX=500+incrementoX;
		inimigo.posicaoY=980-decrementoY;
		inimigo.textura = carregaTexturas("nave1.png");
		incrementoX+=60;
		cont++;
		inimigos.push_back(inimigo);
		if(cont==15){
			decrementoY+=60;
			incrementoX=0;
			cont=0;
		}
	}
}

GLboolean checarColisao(Enemies enemy, Bullet bala){
	bool colisaoX = (enemy.posicaoX + enemy.larg >= bala.x) && (bala.x + 10 >= enemy.posicaoX);
    bool colisaoY = (enemy.posicaoY + enemy.alt >= bala.y) && (bala.y + 60 >= enemy.posicaoY);
	return colisaoX && colisaoY;
}

void mover(){
	if(direita==1 && jogador.posicaoX<1700)
		jogador.posicaoX+=movimentoJogador;
	if(esquerda==1 && jogador.posicaoX>200)
		jogador.posicaoX-=movimentoJogador;
}

GLboolean checarVitoria(){
	if(inimigos.size()==0)
		return true;
	return false;
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
    glFlush();
}

void iniciarTexturas(){
	//iniciar a textura do fundo, do menu (se tiver) e do restante
}

void checarPause(){

}

void setup(){
	glClearColor(0,0,0,1);
	iniciarJogador();
	iniciarInimigos();
	//iniciarTexturas();
	glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		for(int i=0;i<inimigos.size();i++){
		desenharRetanguloTextura(inimigos[i].posicaoX,inimigos[i].posicaoY,inimigos[i].larg,inimigos[i].alt,inimigos[i].textura);
	}
	desenhaTiro();
	
	glutSwapBuffers();
}

void atira(int x, int y){
	Bullet bullet;
	bullet.x = x;
	bullet.y = y;
	bullets.push_back(bullet);
}

void desenhaTiro(){
	for(int i = 0; i < bullets.size();i++){
		if(bullets[i].y>= 1080)
			bullets.erase(bullets.begin()+i);
	}
	for(int i = 0; i < bullets.size(); i++){
		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef (bullets[i].x, bullets[i].y, 0.0);
	    glBegin(GL_POLYGON); 
	        glVertex3f(-2, 15, 0);
	        glVertex3f(2, 15, 0);
	        glVertex3f(2, -15, 0);
	        glVertex3f(-2, -15, 0);
	        glEnd();
	   glPopMatrix();
	   bullets[i].y+=25;
	}
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
			atira(jogador.posicaoX, jogador.posicaoY);
			break;
		case 'r':
		case 'R':
			resetar();
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
	for(int i=0;i<inimigos.size();i++){
		for(int j=0;j<bullets.size();j++){
			if(checarColisao(inimigos[i],bullets[j])){
				bullets.erase(bullets.begin()+j);
				inimigos.erase(inimigos.begin()+i);
			}
		}
	}
	if(checarVitoria())
		exit(0); // aqui vai ficar a tela de vitória e td mais
	mover();
	//checarPause();
	verificaPosicao();
	moverNaves();
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