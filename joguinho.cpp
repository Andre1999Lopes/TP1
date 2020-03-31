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
bool pausa=false;
bool sair=false;
bool reset=false;
bool direita;
bool esquerda;
int dificuldade=1;
float larguraTela=1920;
float alturaTela=1080;
float movimentoJogador=10;
float movimentoInimigos=4;
bool podeAtirar = true;
bool atirou = false;
float alfa=0;
float incrementoX;
float decrementoY;
const float delta1 = 5;
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
GLuint vidas;
GLuint fundo1,fundo2,fundo3;
Mix_Chunk *tiro;
Mix_Chunk *tirotf;
Mix_Music *musicaBatalha;
Mix_Music *musicaMenu;
enum telas {SPLASH, MENU, OPCOES, CREDITOS, JOJINHO, INSTRUCOES, DIFICULDADES} TELAS;
int telaAtual=SPLASH;
int aux=1;
bool venceu=false;
bool perdeu=false;
bool mute=false;
int selecao=1;
float logoYMenu=800;
float logoXtamanho=640;
float logoYtamanho=320;
static GLfloat deslocaFundo1=0;
vector<float> biri;
long long int pontuacao=0;
GLuint n0,n1,n2,n3,n4,n5,n6,n7,n8,n9;

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
//Em teoria, esta é a função que desenha um fundo animado. Não faço ideia do pq não tá funcionando, mas acontece
void desenhaFundo(){
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fundo1);
	glTexParameterf(fundo1, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(fundo1, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(fundo1, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPushMatrix();
	    glTranslatef(960,540/*deslocaFundo1*/,0);
        glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0,0); glVertex2f(-1920/2, -1080/2);
            glTexCoord2f(1,0); glVertex2f( 1920/2, -1080/2);
            glTexCoord2f(1,1); glVertex2f( 1920/2,  1080/2);
            glTexCoord2f(0,1); glVertex2f(-1920/2,  1080/2);
        glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glFlush();
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
	if(direita==true && jogador.posicaoX<1700)
		jogador.posicaoX+=movimentoJogador;
	if(esquerda==true && jogador.posicaoX>200)
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
	if(telaAtual == MENU && !Mix_PlayingMusic()){
		Mix_PlayMusic(musicaMenu,-1);
		Mix_VolumeMusic(32);
	}
	else if(telaAtual==JOJINHO && !Mix_PlayingMusic()){
		Mix_PlayMusic(musicaBatalha,-1);
		Mix_VolumeMusic(96);
	}
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
	fundo1 = carregaTexturas("imgs/fundo1-transparente.png");
	fundo2 = carregaTexturas("imgs/fundo2-transparente.png");
	fundo3 = carregaTexturas("imgs/fundo3-transparente.png");
	n0 = carregaTexturas("imgs/0.png");
	n1 = carregaTexturas("imgs/1.png");
	n2 = carregaTexturas("imgs/2.png");
	n3 = carregaTexturas("imgs/3.png");
	n4 = carregaTexturas("imgs/4.png");
	n5 = carregaTexturas("imgs/5.png");
	n6 = carregaTexturas("imgs/6.png");
	n7 = carregaTexturas("imgs/7.png");
	n8 = carregaTexturas("imgs/8.png");
	n9 = carregaTexturas("imgs/9.png");
	vidas = carregaTexturas("imgs/simbolo-resistencia.png");
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
	tirotf=Mix_LoadWAV("songs/tirotf.wav");
	iniciarJogador();
	iniciarInimigos();
	iniciarTexturas();
	glEnable(GL_BLEND);
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
	if(telaAtual==SPLASH){
		desenhaTexturaEstatica(960,540,800,412,longTime);
	}
	if(telaAtual==MENU){
		if(sair)
			desenhaTexturaEstatica(960,540,960,540,imagemSair);
		else{
			desenhaTexturaEstatica(960,540,1920,1080,fundo1);
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
	else if(telaAtual==OPCOES){
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
	else if(telaAtual==CREDITOS){
		desenhaTexturaEstatica(960,540,1820,1020,imagemOsCreditos);
	}
	else if(telaAtual==JOJINHO){
		if(!sair && !reset && !pausa && !venceu && !perdeu){
			desenhaFundo();
			desenhaTexturaAnimada(jogador.posicaoX,jogador.posicaoY,jogador.larg,jogador.alt,jogador.textura,jogador.tamanho,jogador.estado);
			for(int i=0;i<inimigos.size();i++){
				desenhaTexturaEstatica(inimigos[i].posicaoX,inimigos[i].posicaoY,inimigos[i].larg,inimigos[i].alt,inimigos[i].textura);
			}
			desenhaTiro();
			desenhaTiroInimigo();
			int x=100;
			glColor3f(1,1,1);
			for(int j=0;j<jogador.vida;j++){
				desenhaTexturaEstatica(x,100,60,60, vidas);
				x+=60;
			}

			//desenhaTexturaEstatica(960,540,1920,1080,fundo1);
			long long int guardaPonto = pontuacao;
			int posX=1088;
			for(int i=0;i<9;i++){
				if(guardaPonto%10==0)
					desenhaTexturaEstatica(posX,80,30,60,n0);
				else if(guardaPonto%10==1)
					desenhaTexturaEstatica(posX,80,30,60,n1);
				else if(guardaPonto%10==2)
					desenhaTexturaEstatica(posX,80,30,60,n2);
				else if(guardaPonto%10==3)
					desenhaTexturaEstatica(posX,80,30,60,n3);
				else if(guardaPonto%10==4)
					desenhaTexturaEstatica(posX,80,30,60,n4);
				else if(guardaPonto%10==5)
					desenhaTexturaEstatica(posX,80,30,60,n5);
				else if(guardaPonto%10==6)
					desenhaTexturaEstatica(posX,80,30,60,n6);
				else if(guardaPonto%10==7)
					desenhaTexturaEstatica(posX,80,30,60,n7);
				else if(guardaPonto%10==8)
					desenhaTexturaEstatica(posX,80,30,60,n8);
				else if(guardaPonto%10==9)
					desenhaTexturaEstatica(posX,80,30,60,n9);
				guardaPonto/=10;
				posX-=32;
			}
		}	
		if(sair && !reset && !pausa && !venceu && !perdeu){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,540,imagemSair);
		}
		else if(!sair && reset && !pausa && !venceu && !perdeu){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,540,imagemReset);
		}
		else if(!sair && !reset && pausa && !venceu && !perdeu){
			Mix_PauseMusic();
			desenhaTexturaEstatica(960,540,960,180,imagemPause);
		}
		else if(venceu)
			desenhaTexturaEstatica(960,540,960,540,vitoria);
		else if(perdeu)
			desenhaTexturaEstatica(960,540,960,540,derrota);
	}
	else if(telaAtual==INSTRUCOES){
		desenhaTexturaEstatica(960,540,1820,1020,imagemInstrucoes);
	}
	else if(telaAtual==DIFICULDADES){
		desenhaTexturaEstatica(960,biri[2],biri[0],biri[1],imagemLogo);
		if(selecao==1)
			desenhaTexturaEstatica(960,540,230,33,facilSel);
		else
			desenhaTexturaEstatica(960,540,230,33,facil);
		if(selecao==2)
			desenhaTexturaEstatica(960,490,150,33,medioSel);
		else
			desenhaTexturaEstatica(960,490,150,33,medio);
		if(selecao==3)
			desenhaTexturaEstatica(960,440,100,33,dificilSel);
		else
			desenhaTexturaEstatica(960,440,100,33,dificil);
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
	if(telaAtual==JOJINHO && !venceu && !perdeu){
		if(!pausa && !reset && !sair){
			srand(time(0));
			Bullet enemyBullet;
			valor = rand()%inimigos.size();
			enemyBullet.x = inimigos[valor].posicaoX;
			enemyBullet.y = inimigos[valor].posicaoY;
			enemyBullets.push_back(enemyBullet);
			Mix_PlayChannel(-1,tirotf,0);
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
		glColor3f(0.0, 1.0, 0.0);
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
			esquerda=true;
			jogador.estado=0;
			break;
		case 'd':
		case 'D':
			direita=true;
			jogador.estado=2;
			break;
		case 's':
		case 'S':
			if(sair && !reset){
				if(telaAtual==JOJINHO){
					Mix_HaltMusic();
					trocaTela(MENU);
					tocaMusica();
					sair=false;
				}
				else if(telaAtual==MENU){
					exit(0);
				}
			}
			else if(!sair && reset){
				reset=false;
				Mix_RewindMusic();
				resetar();
				Mix_ResumeMusic();
			}
			else if(venceu){
				resetar();
				Mix_RewindMusic();
				venceu=false;
			}
			else if(perdeu){
				resetar();
				Mix_RewindMusic();
				pontuacao=0;
				perdeu=false;
			}
			break;
		case 'n':
		case 'N':
			if(sair && !reset && !pausa){
				sair=false;
				Mix_ResumeMusic();
			}
			else if(!sair && reset && !pausa){
				reset=false;
				Mix_ResumeMusic();
			}
			else if(venceu){
				Mix_HaltMusic();
				trocaTela(MENU);
				tocaMusica();
			}
			else if(perdeu){
				Mix_HaltMusic();
				trocaTela(MENU);
				tocaMusica();
			}
			break;
		case 13: //enter
			if(telaAtual==MENU && selecao==1 && aux==1){
				Mix_HaltMusic();
				trocaTela(JOJINHO);
				pontuacao=0;
				glutTimerFunc(5000,navesAtirar,0);
				resetar();
				tocaMusica();
			}
			else if(telaAtual==MENU && selecao==2 && aux==1){
				trocaTela(OPCOES);
				selecao=1;
			}
			else if(telaAtual==MENU && selecao==3 && aux==1){
				trocaTela(INSTRUCOES);
				selecao=1;
			}
			else if(telaAtual==MENU && selecao==4 && aux==1){
				sair=true;
			}
			else if(telaAtual==OPCOES && selecao==1){
				trocaTela(DIFICULDADES);
			}
			else if(telaAtual==OPCOES && selecao==2){
				trocaTela(CREDITOS);
				selecao=1;
			}
			else if(telaAtual==OPCOES && selecao==3){
				trocaTela(MENU);
				selecao=1;
			}
			else if(telaAtual==DIFICULDADES && selecao==1){
				dificuldade=1;
				trocaTela(OPCOES);
				selecao=1;	
			}
			else if(telaAtual==DIFICULDADES && selecao==2){
				dificuldade=2;
				trocaTela(OPCOES);
				selecao=1;
			}
			else if(telaAtual==DIFICULDADES && selecao==3){
				dificuldade=3;
				trocaTela(OPCOES);
				selecao=1;
			}
			else if(telaAtual==DIFICULDADES && selecao==4){
				trocaTela(OPCOES);
				selecao=1;
			}
			break;
		case 27:
			if(telaAtual==JOJINHO && !sair && !reset && !pausa)
				sair=true;
			else if(telaAtual==MENU)
				sair=true;
			else if(telaAtual==OPCOES)
				trocaTela(MENU);
			else if(telaAtual==INSTRUCOES)
				trocaTela(MENU);
			else if(telaAtual==CREDITOS)
				trocaTela(OPCOES);
			else if(telaAtual==DIFICULDADES)
				trocaTela(OPCOES);
			break;
		case 32:
			if(!pausa && !reset && !sair && telaAtual == JOJINHO){
				if(podeAtirar){
					atira(jogador.posicaoX, jogador.posicaoY);
					Mix_PlayChannel(-1,tiro,0);
			    	trocaValorAtira(0);
					glutTimerFunc(dificuldade*1000,trocaValorAtira,0);
				}
			}

			break;
		case 'r':
		case 'R':
			if(!pausa && !sair)
				reset=true;
			break;
		case 'p':
		case 'P':
			if(!sair && !reset && !pausa)
				pausa=true;
			else{
				pausa=false;
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
			esquerda=false;
			jogador.estado=1;
			break;
		case 'd':
		case 'D':
			direita=false;
			jogador.estado=1;
			break;
		default:
			break;
	}
}

void specialKeyboard(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=true;
			jogador.estado=0;
			break;
		case GLUT_KEY_RIGHT:
			direita=true;
			jogador.estado=2;
			break;
		case GLUT_KEY_UP:
			if(telaAtual==MENU && selecao>1)
				selecao--;
			else if(telaAtual==OPCOES && selecao>1)
				selecao--;
			else if(telaAtual==DIFICULDADES && selecao>1)
				selecao--;
			break;
		case GLUT_KEY_DOWN:
			if(telaAtual==MENU && selecao<4)
				selecao++;
			else if(telaAtual==OPCOES && selecao<3)
				selecao++;
			else if(telaAtual==DIFICULDADES && selecao<4)
				selecao++;
			break;
	}
}

void specialKeyboardUp(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT:
			esquerda=false;
			jogador.estado=1;
			break;
		case GLUT_KEY_RIGHT:
			direita=false;
			jogador.estado=1;
			break;
	}
}

void atualizaCena(int tempo){
	if(telaAtual==SPLASH){
		if(aux!=0){
			if(alfa<1)
				alfa+=0.01;
		}
		else{
			if(alfa>0)
				alfa-=0.01;
			else{
				alfa=1;
				trocaTela(MENU);
			}
		}
		if(alfa>=1){
			aux=0;
			alfa=0.9999;
			sleep(2);
		}
	}
	//Aqui é verificado se a logo já ficou no tamanho e na posição já predefinidas para o menu
	if(telaAtual==MENU){
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
	if(telaAtual==JOJINHO){
		tocaMusica();
		deslocaFundo1-=delta1;
		for(int i=0;i<inimigos.size();i++){
			if(checarColisaoPlayerNaves(jogador,inimigos[i]))
				exit(0);
			for(int j=0;j<bullets.size();j++){
				if(checarColisao(inimigos[i],bullets[j])){
					bullets.erase(bullets.begin()+j);
					inimigos.erase(inimigos.begin()+i);
					pontuacao+=50;
				}
			}
		}
		for(int i=0;i<enemyBullets.size();i++){
			if(checarColisaoPlayerBala(jogador,enemyBullets[i])){
				enemyBullets.erase(enemyBullets.begin()+i);
				jogador.vida--;
			}
			if(jogador.vida==0){
				perdeu=true;
			}
		}
		if(checarVitoria()){
			 venceu=true;
		}
		if(!sair && !reset && !pausa && !venceu && !perdeu){
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