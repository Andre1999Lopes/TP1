#include "utility.h"

/*
	Este arquivo implementa as funções de jogo.
*/


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
	trocaTextura();
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
	mfalcon = carregaTexturas("imgs/mfalconAnimado.png");
	xwing = carregaTexturas("imgs/x-wingAnimado.png");
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
			glutTimerFunc(5000,navesAtirar,0);
		}
	}
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

void trocaTextura(){
	if(texturaAtual==3)
		texturaAtual=1;
	if(texturaAtual==1)
		jogador.textura=mfalcon;
	else if(texturaAtual==2)
		jogador.textura=xwing;
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
