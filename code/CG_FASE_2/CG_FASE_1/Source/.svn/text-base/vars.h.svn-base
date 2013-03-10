#include "Vec3.h"
#include "FrustumG.h"

//	Variáveis
int windowSize[2];							// tamanho da janela ao longo da aplicação
int windowSizeInitial[2] = {800, 600};		// tamanho incial da janela
int fullscreen=0;


GLfloat lightPos[4] = {50.0, 30.0 ,0.0, 0.0};
GLfloat lightAmb[3] = {0.5, 0.1, 0.1};
GLfloat lightDiff[3] = {1.0, 1.0, 1.0};

float FogCol[3]={0.3f, 0.3f, 0.3f};					// define a nice light grey
float FogStart = 100;
float FogStop = 400;

GLMmodel *key,*tesouro;	
Md2Player *boneco, *torreMD, *tree, *bullet;

float larguraTerrreno = 2000;				// largura do terreno

int width = glutGet(GLUT_SCREEN_WIDTH);					
int height = glutGet(GLUT_SCREEN_HEIGHT);

Vec3 cam(0.0,0.0,0.0), look(0,0,0), up(0,1,0);
Vec3 direc(0,0,0), direita(0,0,0);
Vec3 player(0,0,0);

float nearP = 1.0f, farP = 1000.0f;
float angle = 45, ratio=1;

float alpha = 0, beta = 0;					     //Angulos utilizados para calculo da posicao da camara e para onde ela aponta
int mouseX = 0, mouseY = 0;		                     //Posicoes do rato para a movimentacao
int view = TPS;								         //Variavel que define o modo FPS ou TPS
int help = 0;

float factorMetro;
float velJogador = 2.78;				//Velocidade do jogador em m/s
float velJogadorMaxima = 40;			//Velocidade máxima do jogador
float velJogadorMinima = 1;			//velocidade mínima do jogador-

int keysPlayer=0;
int keys[nChaves];	// chaves apanhadas
float arvores[nArvores][3];				//Matriz que armazena as coordenadas das árvores
float posChave[nChaves][3];				//Matriz que armazena as coordenadas das chaves
float posTorre[nTorres][4];				//Matriz que armazena as coordenadas das torres de protecção
bool frente, traz, left, right, rotateEsq, rotateDir, rotateU, rotateD;	//Indicam o movimento correspondente à tecla
bool wrap = true;
int time1, timebase = 0, frame;
float fps;



int terrainTexture=1;
// variáveis para a geração do terreno
ILubyte* image;							// imagem usada para a criação do terreno
int comprimento;						// comprimento do terreno
int largura;							// largura do terreno
float* alturas = NULL;					// alturas do terreno

int bordaTerreno = 30;

int nAtingido;
float balas[nBalas][4];

int displayList;

float raioArvore=2;

int vbo = 0;//gera terreno através de vbo ou não

// dados para geracao do terreno - VBOs
GLuint buffers[3];
int* posicoes = NULL;
float *vertexB, *normalB, *textB;

char test[] = TEXTURE;

bool debug = true;

int end = 0;
int dead = 0;

float endX, endZ;

FrustumG frustum;
int frustumOn = 1;

int move = 0;
int castel = 0;
