/*
	Este arquivo inclui as assinaturas de funções e procedimentos usados no jogo.
*/

void desenhaTiro();
void navesAtirar(int valor);
GLuint carregaTexturas(const char *arquivo);
void verificaPosicao();
GLboolean checarColisao();
void iniciarJogador();
void iniciarInimigos();
void desenhaTiroInimigo();
void draw();
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void specialKeyboardUp(int key, int x, int y);
void atualizaCena(int tempo);
void resetar();
void verificaPosicao();
void trocaTela(int valor);
void moverNaves();
void desenhaFundo();
void desenhaTexturaEstatica(float x, float y, float larg, float alt, GLuint textura);
void iniciarInimigos();
GLboolean checarColisao(Enemies enemy, Bullet bala);
GLboolean checarColisaoPlayerBala(Player player, Bullet bala);
GLboolean checarColisaoPlayerNaves(Player player, Enemies enemy);
void mover();
GLboolean checarVitoria();
GLuint carregaTexturas(const char *arquivo);
void iniciarJogador();
void desenhaTexturaAnimada(float x, float y, float larg, float alt, GLuint textura, float tamanho, float estado);
void tocaMusica();
void iniciarTexturas();
void setup();
void trocaValorAtira(int x);
void atira(int x, int y);
void navesAtirar(int valor);
void desenhaTiro();
void desenhaTiroInimigo();
void trocaTextura();
