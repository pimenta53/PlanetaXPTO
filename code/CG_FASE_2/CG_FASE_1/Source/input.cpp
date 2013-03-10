#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <windows.h>
#include "input.h"
#include "terrain.h"
#include "output.h"

#include "Vec3.h"

#define _USE_MATH_DEFINES

extern bool wrap;								//utilizado no posicionamento do cursor
extern float alpha , beta ;						//Angulos utilizados para calculo da posicao da camara e para onde ela aponta
extern int dead;
extern int help;
extern int view ;								//Variavel que define o modo FPS ou TPS

extern Vec3 cam, look, player, direc, direita;

extern int frustumOn;
extern bool frente, traz, left, right, rotateEsq, rotateDir, rotateU, rotateD; //difinem um estado associado às teclas que podem estar(UP/DOWN)
extern float *alturas;
extern int largura, comprimento;

extern float velJogador;
extern float velJogadorMaxima;
extern float velJogadorMinima;
extern float factorMetro;
extern float fps;

extern float arvores[nArvores][3];
extern float raioArvore;
extern float endX, endZ;
extern int end;
extern int keysPlayer;

extern int fullscreen;

extern int move;
float vel;
//Calculo das posiçÁes da camara e do player

void calcPos(float a, float b) {
	float height=0;

	if(view == FPS) {
		cam = player;
		cam.y += 1.2;
		look.x = player.x + 5 * (cos(b) * sin(a));
		look.z = player.z + 5 * (cos(b) * cos(a));
		look.y = player.y + 20 * (sin(b));
	}   
	else {
		look.x = player.x + 5 * (cos(b) * sin(a));
		look.z = player.z + 5 * (cos(b) * cos(a));
		look.y = player.y + 5 * (sin(b));
		
		cam.x = player.x + (5 * cos((M_PI/10)-b) * sin(M_PI + a));
		cam.z = player.z + (5 * cos((M_PI/10)-b) * cos (M_PI + a));
		cam.y = player.y + (7 * sin((M_PI/10)-b));

		if (abs(cam.x) < (largura/2) && abs(cam.z) < (comprimento/2)) {
			height = getAlturaSmoothWalk(alturas, largura, cam.x+(largura/2), cam.z+(largura/2));
			if(cam.y <= height+1) cam.y = height+1;
		}
	}
}

/*Funcao que calcula, para cada frame, os vectores directores utilizados para a movimentacao*/
void updateDir() {
	
	float velocidade = ((velJogador/fps)*factorMetro);
	vel = velocidade*3;
	direc.set(look.x - player.x, 
			look.y - player.y, 
			look.z - player.z);
	
	direc.normalize();
	direc = direc*velocidade;
	
	direita.x = -direc.z;		
	direita.z = direc.x;
	direita = direita*velocidade*2;
}

//Calculo dos novos angulos 'alpha' e 'beta' baseados na distancia percorrida pelo rato
void fpassiveMouse(int xx, int yy) {
	
	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	float meioW = w/2;
	float meioH = h/2;
	float diffX, diffY;
	float limit;
	
	
	if (!wrap) {
		diffX = xx - meioW;		//distancia percorrida pelo rato no eixo do xx
		alpha -= 0.002*diffX;
		
		diffY = (yy - meioH)*0.002; //distancia percorrida pelo rato no eixo do xx
		
		if (view == FPS) limit=M_PI/4;	 //limitar o angulo 'beta' consoante o modo de vista
		else limit = M_PI/8;
		
		if ((beta+diffY) > -limit && (beta+diffY) <limit)  
			beta += diffY;
		
		wrap=true;
		glutWarpPointer(meioW,meioH);	//posicionar o cursor no centro
		
	} else {
		wrap=false;
	}
		
}

// escrever funÁ„o de processamento do teclado
void pressKey(unsigned char key, int xx, int yy) {   
	
	switch(key) {
		//activar teclas
		case 'A':
		case 'a': left=1;move++; break;
		case 'D':
		case 'd': right=1;move++; break;
		case 'W':
		case 'w': frente=1;move++; break;
		case 'S':
		case 's': traz=1;move++; break;
		//Aumentar ou diminuir velocidade do jogador
		case '+': if (velJogador < velJogadorMaxima) velJogador += 1; break; 
		case '-': if (velJogador > velJogadorMinima) velJogador -= 1; break;
		//fazer a troca de FPS para TPS e vice versa e calcular posições do jogador
		case 'V':			
		case 'v' :                           //Passa do modo FPS para TPS e vice versa
			if(view == FPS) {
				player.x = cam.x; player.y = cam.y; player.z = cam.z;
				view = TPS;
				calcPos(alpha, beta);
			}
			else {
				cam.x = player.x; cam.y = player.y; cam.z = player.z;
				view = FPS;
				calcPos(alpha, beta);
			}
			break; 
		case 'F':
		case 'f':{ 
			fullscreen = (fullscreen+1)%2;
		}break;
		case 'C':
		case 'c':{ 
			frustumOn = (frustumOn+1)%2;
			}break;
		//ESC pa sair
		case 27 :                           
			exit(0);
		case '1' :
			if(dead == 1 || end == 1){
				reiniciar();
			}
			break;
		case '2':
			if(dead == 1 || end == 1){
				exit(0);
			}
			break;
		case '0':
			if(help == 0){
				ajuda();
				help = 1;
			}
			else{
				help = 0;
			}
	}
}
	
//desativar teclas
void releaseKey(unsigned char key, int x, int y) {
	switch(key) {
		case 'a': left = 0; move--; break;
		case 'd': right = 0; move--; break;
		case 'w': frente = 0; move--; break;
		case 's': traz = 0; move--; break;
	}
}

void pressSpKey(int key, int xx, int yy) {
}

void releaseSpKey(int key, int x, int y) {
}

bool inside() {
	for(int i=0; i < nArvores; i++){
		if((pow((player.x-arvores[i][XX]),2)+pow((player.y-arvores[i][YY]),2)+pow((player.z-arvores[i][ZZ]),2)) <= pow(raioArvore,2)){
		   return true;
		}
	}
	return false;
}

void funcaoMove(int valor) {

	endGame();

//	direc = direc*(velJogador);
//	direita = direita*(velJogador);

	if (frente) {
		if (abs(player.x + direc.x) < ((largura/2)-1) && abs(player.z + direc.z) < ((comprimento/2)-1)) {
			player.x += direc.x;               //Estruturado para o modo FPS e TPS
			player.z += direc.z;
			if (inside()) {
				player.x -= direc.x;               //Estruturado para o modo FPS e TPS
				player.z -= direc.z;
			}
		}
	}
	
	if (traz) {
		if (abs(player.x - direc.x) < ((largura/2)-2) && abs(player.z - direc.z) < ((comprimento/2)-2)) {
			player.x -= direc.x;
			player.z -= direc.z;
			if (inside()) {
				player.x += direc.x;               //Estruturado para o modo FPS e TPS
				player.z += direc.z;
			}
		}
	}
	
	if (left) {
		if (abs(player.x - direita.x) < ((largura/2)-1) && abs(player.z - direita.z) < ((comprimento/2)-1)) {
			if(view == FPS) {                  //Utiliza o vecto diretor orientado a direita para
				look.x -= direita.x;
				look.z -= direita.z;
			}
			player.x -= direita.x;
			player.z -= direita.z;
		if (inside()) {
				player.x += direita.x;               //Estruturado para o modo FPS e TPS
				player.z += direita.z;
			}
		}
	}
	
	if (right) {
		if (abs(player.x + direita.x) < ((largura/2)-1) && abs(player.z + direita.z) < ((comprimento/2)-1)) {
			if(view == FPS) {
				look.x += direita.x;
				look.z += direita.z;
			}
			player.x += direita.x;
			player.z += direita.z;
		if (inside()) {
				player.x -= direita.x;               //Estruturado para o modo FPS e TPS
				player.z -= direita.z;
			}
		}
	}	
	
	calcPos(alpha, beta);

}

void funcaoMenu(unsigned char key, int xx, int yy){
}

void endGame() {
	if((player.x > endX-5 && player.x < endX+5) && (player.z > endZ-5 && player.z < endZ+5) && nChaves == keysPlayer) end = 1;
	
}