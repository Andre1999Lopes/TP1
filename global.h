/*
	Este arquivo inclui as variáveis globais usadas no jogo.
*/
#define larguraMundo 1920
#define alturaMundo 1080

bool pausa=false;
bool sair=false;
bool reset=false;
bool direita;
bool esquerda;
int dificuldade=1;
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
GLuint mfalcon,xwing;
Mix_Chunk *tiro;
Mix_Chunk *tirotf;
Mix_Music *musicaBatalha;
Mix_Music *musicaMenu;
int texturaAtual=1;
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