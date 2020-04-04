#include "utility.h"

/*
	Este arquivo inclui as implementações das callbacks.
*/
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

void reshape(int width, int height){
  	glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
   	glOrtho(0, larguraMundo, 0, alturaMundo, -1, 1);
   	float aspectoJanela = ((float)width)/height;
    float aspectoMundo = ((float) larguraMundo)/ alturaMundo;
    // se a janela está menos larga do que o mundo (16:9)...
    if (aspectoJanela < aspectoMundo) {
        // vamos colocar barras verticais (acima e abaixo)
        float hViewport = width / aspectoMundo;
        float yViewport = (height - hViewport)/2;
        glViewport(0, yViewport, width, hViewport);
    }
    // se a janela está mais larga (achatada) do que o mundo (16:9)...
    else if (aspectoJanela > aspectoMundo) {
        // vamos colocar barras horizontais (esquerda e direita)
        float wViewport = ((float)height) * aspectoMundo;
        float xViewport = (width - wViewport)/2;
        glViewport(xViewport, 0, wViewport, height);
    }
    else
        glViewport(0, 0, width, height);
   	glMatrixMode(GL_MODELVIEW);
   	glLoadIdentity();
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
				enemyBullets.clear();
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
		case 'c':
		case 'C':
			texturaAtual++;
			trocaTextura();
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
