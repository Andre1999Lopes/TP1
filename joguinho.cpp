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

/*
	Fontes das imagens e músicas:

	*Música de batalha: https://www.youtube.com/watch?v=FvG4ZjHbkLg
	*Música do menu: https://www.youtube.com/watch?v=bOYdk1UY5o8
	*Efeito sonoro do tiro: 
	*Imagem logo: 
	*Fonte das imagens de opções e instruções: joystix monospace
*/

using namespace std;
void desenhaTiro();
void navesAtirar(int valor);
GLuint carregaTexturas(const char *arquivo);
void verificaPosicao();
GLboolean checarColisao();
void iniciarJogador();
void iniciarInimigos();
void desenhaTiroInimigo();
int pausa=0;
int sair=0;
int reset=0;
int direita;
int esquerda;
int dificuldade=1;
float larguraTela=1920;
float alturaTela=1080;
float movimentoJogador=10;
float movimentoInimigos=4;
bool podeAtirar = true;
int atirou = 0;
float alfa=0;
float incrementoX;
float decrementoY;
GLuint imagemIniciar,imagemIniciarSel;
GLuint imagemSair;
GLuint imagemPause;
GLuint imagemReset;
GLuint vitoria;
GLuint derrota;
GLuint imagemSairMenu,imagemSairMenuSel;
GLuint imagemOpcoes,imagemOpcoesSel;
GLuint imagemDificuldade,imagemDificuldadeSel;
GLuint imagemInstrucao,imagemInstrucaoSel;
GLuint imagemCredito,imagemCreditoSel;
GLuint imagemVoltar,imagemVoltarSel;
GLuint imagemInstrucoes;
GLuint imagemOsCreditos;
GLuint imagemLogo;
GLuint longTime;
GLuint facil,facilSel,medio,medioSel,dificil,dificilSel;
Mix_Chunk *tiro;
Mix_Music *musicaBatalha;
Mix_Music *musicaMenu;
enum telas {SPLASH, MENU, OPCOES, CREDITOS, JOJINHO, INSTRUCOES, DIFICULDADES} TELAS;
int telaAtual=0;
int aux=1;
int selecao=1;
float logoYMenu=800;
float logoXtamanho=640;
float logoYtamanho=320;
vector<float> biri;

typedef struct Player{
	float posicaoX;
	float posicaoY;
	int vida;
	int larg;
	int alt;
	float estado;
	float tamanho;
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
vector<Bullet> enemyBullets;
Player jogador;
vector<Enemies> inimigos;
//Função que reinicia o jogador, os inimigos e os recoloca em suas posições iniciais
void resetar(){
	inimigos.clear();
	bullets.clear();
	iniciarJogador();
	iniciarInimigos();
	if(movimentoInimigos<0)
		movimentoInimigos*=-1;
}
//Função que verifica se os inimigos chegaram na borda da parte jogável da tela
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
//Função que troca o valor da tela pelo valor recebido
void trocaTela(int valor){
	telaAtual=valor;
}
//Função que muda a posição das naves inimigas
void moverNaves(){
	for(int i=0;i<inimigos.size();i++){
		inimigos[i].posicaoX+=movimentoInimigos;
	}
}
//Função que inicializa o vector de inimigos e dá uma posição para cada um
void iniciarInimigos(){
	incrementoX=0;
	decrementoY=0;
	int cont=0;
	for(int i=0;i<vetorInimigos;i++){
		Enemies inimigo;
		inimigo.posicaoX=500+incrementoX;
		inimigo.posicaoY=980-decrementoY;
		inimigo.textura = carregaTexturas("imgs/nave1.png");
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
//Função que checa colisão entre as balas atiradas pelo jogador e as naves inimigas. Método AABB
GLboolean checarColisao(Enemies enemy, Bullet bala){
	bool colisaoX = (enemy.posicaoX + (enemy.larg)/2 >= (bala.x - 2)) && (bala.x + 2 >= enemy.posicaoX - (enemy.larg)/2);
    bool colisaoY = (enemy.posicaoY + (enemy.alt)/2 >= (bala.y - 15)) && (bala.y + 15 >= enemy.posicaoY - (enemy.alt)/2);
	return colisaoX && colisaoY;
}
//Função que checa colisão entre as balas atiradas pelos inimigos e o jogador. Método AABB
GLboolean checarColisaoPlayerBala(Player player, Bullet bala){
	bool colisaoX = (player.posicaoX + (player.larg)/2 >= bala.x - 2) && (bala.x + 2 >= player.posicaoX - (player.larg/2));
    bool colisaoY = (player.posicaoY + (player.alt)/2 >= bala.y - 15) && (bala.y + 15 >= player.posicaoY - (player.alt/2));
	return colisaoX && colisaoY;
}
//Função que checa colisão entre as naves inimigas e o jogador. Método AABB
GLboolean checarColisaoPlayerNaves(Player player, Enemies enemy){
	bool colisaoX = (player.posicaoX + (player.larg)/2 >= enemy.posicaoX - (enemy.larg)/2) && (enemy.posicaoX + (enemy.larg)/2 >= player.posicaoX - (enemy.larg)/2);
    bool colisaoY = (player.posicaoY + (player.alt)/2 >= enemy.posicaoY - (enemy.alt)/2) && (enemy.posicaoY + (enemy.larg)/2 >= player.posicaoY - (enemy.alt)/2);
	return colisaoX && colisaoY;
}
//Função que muda a posição do jogador baseada em uma constante de movimento
void mover(){
	if(direita==1 && jogador.posicaoX<1700)
		jogador.posicaoX+=movimentoJogador;
	if(esquerda==1 && jogador.posicaoX>200)
		jogador.posicaoX-=movimentoJogador;
}
//Função de checagem de vitória
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
	jogador.larg=80;
	jogador.alt=100;
	jogador.vida=2;
	jogador.estado=1.0;
	jogador.tamanho=3.0;
	jogador.textura = carregaTexturas("imgs/mfalconAnimado.png");
}
//Função que desenha um retângulo dados a sua posição no eixo X e no eixo Y, sua largura, sua altura e a textura a ser utilizada. Textura estática
void desenhaTexturaEstatica(float x, float y, float larg, float alt, GLuint textura){
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

void desenhaTexturaAnimada(float x, float y, float larg, float alt, GLuint textura, float tamanho, float estado){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textura);
    glPushMatrix();
        glTranslatef(x, y, 0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(estado/tamanho,0); glVertex2f(-larg/2, -alt/2);
            glTexCoord2f((estado+1)/tamanho,0); glVertex2f( larg/2, -alt/2);
            glTexCoord2f((estado+1)/tamanho,1); glVertex2f( larg/2,  alt/2);
            glTexCoord2f(estado/tamanho,1); glVertex2f(-larg/2,  alt/2);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glFlush();

}
//Função que muda a música de acordo com a tela na qual o jogador se encontra
void tocaMusica(){
	if(telaAtual==1 && !Mix_PlayingMusic())
		Mix_PlayMusic(musicaMenu,-1);
	else if(telaAtual==4 && !Mix_PlayingMusic())
		Mix_PlayMusic(musicaBatalha,-1);
}
//Função que inicializa as texturas do menu
void iniciarTexturas(){
	longTime = carregaTexturas("imgs/a-long-time.png");
	imagemLogo = carregaTexturas("imgs/star-wars-logo.png");
	imagemInstrucao = carregaTexturas("imgs/instrucoes.png");
	imagemOpcoes = carregaTexturas("imgs/opcoes.png");
	imagemIniciar = carregaTexturas("imgs/iniciar.png");
	imagemSair = carregaTexturas("imgs/sair.png");
	imagemPause = carregaTexturas("imgs/pause.png");
	imagemReset = carregaTexturas("imgs/reset.png");
	imagemSairMenu = carregaTexturas("imgs/sairMenu.png");
	imagemCredito = carregaTexturas("imgs/creditos.png");
	imagemDificuldade = carregaTexturas("imgs/dificuldade.png");
	imagemVoltar = carregaTexturas("imgs/voltar.png");
	imagemInstrucoes = carregaTexturas("imgs/asInstrucoes.png");
	imagemOsCreditos = carregaTexturas("imgs/osCreditos.png");
	facil = carregaTexturas("imgs/facil.png");
	facilSel = carregaTexturas("imgs/facilSel.png");
	medio = carregaTexturas("imgs/medio.png");
	medioSel = carregaTexturas("imgs/medioSel.png");
	dificil = carregaTexturas("imgs/dificil.png");
	dificilSel = carregaTexturas("imgs/dificilSel.png");
	vitoria = carregaTexturas("imgs/telaVitoria.png");
	derrota = carregaTexturas("imgs/telaDerrota.png");
	imagemIniciarSel = carregaTexturas("imgs/iniciarSel.png");
	imagemOpcoesSel = carregaTexturas("imgs/opcoesSel.png");
	imagemDificuldadeSel = carregaTexturas("imgs/dificuldadeSel.png");
	imagemCreditoSel = carregaTexturas("imgs/creditosSel.png");
	imagemSairMenuSel = carregaTexturas("imgs/sairMenuSel.png");
	imagemInstrucaoSel = carregaTexturas("imgs/instrucoesSel.png");
	imagemVoltarSel = carregaTexturas("imgs/voltarSel.png");
}
//Função que inicializa o jogo no geral
void setup(){
	glClearColor(0,0,0,0);
	biri.push_back(2500);
	biri.push_back(1800);
	biri.push_back(400);
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,4096);
	musicaBatalha=Mix_LoadMUS("songs/BattleOfTheHeroes.mp3");
	musicaMenu=Mix_LoadMUS("songs/Abertura8bit.mp3");
	tiro=Mix_LoadWAV("songs/tiroMF.wav");
	iniciarJogador();
	iniciarInimigos();
	iniciarTexturas();
	glEnable(GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void reshape(int width, int height){
	glViewport(0, 0, width, height);
  	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(0, 1920, 0, 1080, -1, 1);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
}

void draw(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor4f(1,1,1,alfa);
	if(telaAtual==0){
		desenhaTexturaEstatica(960,540,800,412,longTime);
	}
	if(telaAtual==1){
		if(sair==1)
			desenhaTexturaEstatica(960,540,960,540,imagemSair);
		else{
			desenhaTexturaEstatica(960,biri[2],biri[0],biri[1],imagemLogo);
			/*A variável aux serve para verificar se a introdução do jogo já foi ou não finalizada para que se possa desenhar
			as opções do menu*/
			if(aux==1){
				if(selecao==1)
					desenhaTexturaEstatica(960,540,120,33,imagemIniciarSel);
				else
					desenhaTexturaEstatica(960,540,120,33,imagemIniciar);
				if(selecao==2)
					desenhaTexturaEstatica(960,485,115,55,imagemOpcoesSel);
				else
					desenhaTexturaEstatica(960,485,115,55,imagemOpcoes);
				if(selecao==3)
					desenhaTexturaEstatica(960,430,180,55,imagemInstrucaoSel);
				else
					desenhaTexturaEstatica(960,430,180,55,imagemInstrucao);
				if(selecao==4)
					desenhaTexturaEstatica(960,380,75,33,imagemSairMenuSel);
				else
					desenhaTexturaEstatica(960,380,75,33,imagemSairMenu);
			}
		}
	}
	else if(telaAtual==2){
		desenhaTexturaEstatica(960,biri[2],biri[0],biri[1],imagemLogo);
		if(selecao==1)
			desenhaTexturaEstatica(960,540,200,33,imagemDificuldadeSel);
		else
			desenhaTexturaEstatica(960,540,200,33,imagemDificuldade);
		if(selecao==2)
			desenhaTexturaEstatica(960,490,150,39,imagemCreditoSel);
		else
			desenhaTexturaEstatica(960,490,150,39,imagemCredito);
		if(selecao==3)
			desenhaTexturaEstatica(960,440,130,33,imagemVoltarSel);
		else
			desenhaTexturaEstatica(960,440,130,33,imagemVoltar);
	}
	else if(telaAtual==3){
		desenhaTexturaEstatica(960,540,1820,1020,imagemOsCreditos);
	}
	else if(telaAtual==4){
		if(sair==0 && reset==0 && pausa==0){
			desenhaTexturaAnimada(jogador.posicaoX,jogador.posicaoY,jogador.larg,jogador.alt,jogador.textura,jogador.tamanho,jogador.estado);
			for(int i=0;i<inimigos.size();i++){
				desenhaTexturaEstatica(inimigos[i].posicaoX,inimigos[i].posicaoY,inimigos[i].larg,inimigos[i].alt,inimigos[i].textura);
			}
			desenhaTiro();
			desenhaTiroInimigo();
		}	
		if(sair==1 && reset==0 && pausa==0){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,540,imagemSair);
		}
		if(sair==0 && reset==1 && pausa==0){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,540,imagemReset);
		}
		if(sair==0 && reset==0 && pausa==1){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,180,imagemPause);
		}
	}
	else if(telaAtual==5){
		desenhaTexturaEstatica(960,540,1820,1020,imagemInstrucoes);
	}
	else if(telaAtual==6){
		desenhaTexturaEstatica(960,biri[2],biri[0],biri[1],imagemLogo);
		if(selecao==1)
			desenhaTexturaEstatica(960,540,120,39,facilSel);
		else
			desenhaTexturaEstatica(960,540,120,39,facil);
		if(selecao==2)
			desenhaTexturaEstatica(960,490,120,39,medioSel);
		else
			desenhaTexturaEstatica(960,490,120,39,medio);
		if(selecao==3)
			desenhaTexturaEstatica(960,440,150,39,dificilSel);
		else
			desenhaTexturaEstatica(960,440,150,39,dificil);
		if(selecao==4)
			desenhaTexturaEstatica(960,390,130,33,imagemVoltarSel);
		else
			desenhaTexturaEstatica(960,390,130,33,imagemVoltar);
	}
	glutSwapBuffers();
}
//Função que verifica se o player pode ou não atirar. O tempo muda de acordo com a dificuldade
void trocaValorAtira(int x){
	if(podeAtirar)
		podeAtirar = false;
	else
		podeAtirar = true;
}
//Cria uma bala do jogador
void atira(int x, int y){
		Bullet bullet;
		bullet.x = x;
		bullet.y = y;
		bullets.push_back(bullet);
}
//Cria uma bala das naves inimigas
void navesAtirar(int valor){
	if(telaAtual==4){
		for(int i=0;i<dificuldade;i++){
			if(pausa==0 && reset==0 && sair==0){
				srand(time(0));
				Bullet enemyBullet;
				valor = rand()%inimigos.size();
				enemyBullet.x = inimigos[valor].posicaoX;
				enemyBullet.y = inimigos[valor].posicaoY;
				enemyBullets.push_back(enemyBullet);
			}
		}
	}
	glutTimerFunc(5000,navesAtirar,0);
}
//Desenha um tiro. Se a posição do tiro ultrapassar a altura máxima da tela, o tiro é apagado da memória
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
//Mesma coisa da função de cima, porém para as naves inimigas
void desenhaTiroInimigo(){
	for(int i = 0; i < enemyBullets.size();i++){
		if(enemyBullets[i].y<=0)
			enemyBullets.erase(enemyBullets.begin()+i);
	}
	for(int i = 0; i < enemyBullets.size(); i++){
		glColor3f(1.0, 0.0, 0.0);
		glPushMatrix();
		glTranslatef (enemyBullets[i].x, enemyBullets[i].y, 0.0);
	    glBegin(GL_POLYGON); 
	        glVertex3f(-2, 15, 0);
	        glVertex3f(2, 15, 0);
	        glVertex3f(2, -15, 0);
	        glVertex3f(-2, -15, 0);
	        glEnd();
	   glPopMatrix();
	   enemyBullets[i].y-=25;
	}
}

void keyboard(unsigned char key, int x, int y){
	switch(key){
		case 'a':
		case 'A':
			esquerda=1;
			jogador.estado=0;
			break;
		case 'd':
		case 'D':
			direita=1;
			jogador.estado=2;
			break;
		case 's':
		case 'S':
			if(sair==1 && reset==0){
				if(telaAtual==4){
					Mix_HaltMusic();
					trocaTela(1);
					tocaMusica();
					sair=0;
				}
				else if(telaAtual==1){
					exit(0);
				}
			}
			else if(sair==0 && reset==1){
				reset=0;
				Mix_RewindMusic();
				resetar();
				Mix_ResumeMusic();
			}
			break;
		case 'n':
		case 'N':
			if(sair==1 && reset==0 && pausa==0){
				sair=0;
				Mix_ResumeMusic();
			}
			else if(sair==0 && reset==1 && pausa==0){
				reset=0;
				Mix_ResumeMusic();
			}
			break;
		case 13: //enter
			if(telaAtual==1 && selecao==1 && aux==1){
				Mix_HaltMusic();
				trocaTela(4);
				glutTimerFunc(5000,navesAtirar,0);
				resetar();
				tocaMusica();
			}
			else if(telaAtual==1 && selecao==2 && aux==1){
				trocaTela(2);
				selecao=1;
			}
			else if(telaAtual==1 && selecao==3 && aux==1){
				trocaTela(5);
				selecao=1;
			}
			else if(telaAtual==1 && selecao==4 && aux==1){
				sair=1;
			}
			else if(telaAtual==2 && selecao==1){
				trocaTela(6);
			}
			else if(telaAtual==2 && selecao==2){
				trocaTela(3);
				selecao=1;
			}
			else if(telaAtual==2 && selecao==3){
				trocaTela(1);
				selecao=1;
			}
			else if(telaAtual==6 && selecao==1)
				dificuldade=1;
			else if(telaAtual==6 && selecao==2)
				dificuldade=2;
			else if(telaAtual==6 && selecao==3)
				dificuldade=3;
			else if(telaAtual==6 && selecao==4){
				trocaTela(2);
				selecao=1;
			}
			break;
		case 27:
			if(telaAtual==4 && sair==0 && reset==0 && pausa==0)
				sair=1;
			else if(telaAtual==1)
				sair=1;
			else if(telaAtual==2)
				trocaTela(1);
			else if(telaAtual==5)
				trocaTela(1);
			else if(telaAtual==3)
				trocaTela(2);
			else if(telaAtual==6)
				trocaTela(2);
			break;
		case 32:
			if(pausa==0 && reset==0 && sair==0){
				if(podeAtirar==true){
					atira(jogador.posicaoX, jogador.posicaoY);
					Mix_PlayChannel(-1,tiro,0);
			    	trocaValorAtira(0);
					glutTimerFunc(dificuldade*1000,trocaValorAtira,0);
				}
			}
			break;
		case 'r':
		case 'R':
			if(pausa==0 && sair==0)
				reset=1;
			break;
		case 'p':
		case 'P':
			if(sair==0 && reset==0 && pausa==0)
				pausa=1;
			else{
				pausa=0;
				Mix_ResumeMusic();
			}
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
			jogador.estado=1;
			break;
		case 'd':
		case 'D':
			direita=0;
			jogador.estado=1;
			break;
		default:
			break;
	}
}

void specialKeyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=1;
			jogador.estado=0;
			break;
		case GLUT_KEY_RIGHT:
			direita=1;
			jogador.estado=2;
			break;
		case GLUT_KEY_UP:
			if(telaAtual==1 && selecao>1)
				selecao--;
			else if(telaAtual==2 && selecao>1)
				selecao--;
			else if(telaAtual==6 && selecao>1)
				selecao--;
			break;
		case GLUT_KEY_DOWN:
			if(telaAtual==1 && selecao<4)
				selecao++;
			else if(telaAtual==2 && selecao<3)
				selecao++;
			else if(telaAtual==6 && selecao<4)
				selecao++;
			break;
	}
}

void specialKeyboardUp(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=0;
			jogador.estado=1;
			break;
		case GLUT_KEY_RIGHT:
			direita=0;
			jogador.estado=1;
			break;
	}
}

void atualizaCena(int tempo){
	if(telaAtual==0){
		if(aux!=0){
			if(alfa<1)
				alfa+=0.01;
		}
		else{
			if(alfa>0)
				alfa-=0.01;
			else{
				alfa=1;
				trocaTela(1);
			}
		}
		if(alfa>=1){
			aux=0;
			alfa=0.9999;
			sleep(2);
		}
	}
	//Aqui é verificado se a logo já ficou no tamanho e na posição já predefinidas para o menu
	if(telaAtual==1){
		tocaMusica();
		if(biri[2]<800)
			biri[2]+=1.1;
		if(biri[0]>640)
			biri[0]-=5.0;
		if(biri[1]>320)
			biri[1]-=4.0;
		else if(biri[2]>=800 && biri[0]<=640 && biri[1]<=320)
			aux=1;
	}
	//Aqui fica a lógica do jogo
	if(telaAtual==4){
		tocaMusica();
		for(int i=0;i<inimigos.size();i++){
			if(checarColisaoPlayerNaves(jogador,inimigos[i]))
				exit(0);
			for(int j=0;j<bullets.size();j++){
				if(checarColisao(inimigos[i],bullets[j])){
					bullets.erase(bullets.begin()+j);
					inimigos.erase(inimigos.begin()+i);
				}
			}
		}
		for(int i=0;i<enemyBullets.size();i++){
			if(checarColisaoPlayerBala(jogador,enemyBullets[i])){
				enemyBullets.erase(enemyBullets.begin()+i);
				jogador.vida--;
			}
			if(jogador.vida==0){
				printf("perdeu\n");
				exit(0);
			}
		}
		if(checarVitoria()){
			Mix_HaltMusic();
			trocaTela(1);
			tocaMusica();
			resetar(); // aqui vai ficar a tela de vitória e td mais
		}
		if(sair==0 && reset==0 && pausa==0){
			mover();
			verificaPosicao();
			moverNaves();
		}
	}
	glutPostRedisplay();
	glutTimerFunc(33, atualizaCena, 0);
}

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
	glutTimerFunc(0,atualizaCena,33);
	glutMainLoop();
}