#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <GL/glut.h>
#include "output.h"
#include "scene.h"
#include "Vec3.h"

extern float fps;
extern int keysPlayer;

extern Vec3 player;

extern bool debug;
extern int vbo;
extern int end;
extern int windowSizeInitial[2];
extern int dead;
extern int KeysPlayer;
extern int keys[nChaves];
extern int castel;
extern float factorMetro;
extern float* alturas;
extern int largura;
extern int frustumOn;

extern float endZ,endX;

/*Função responsável por desenhar para o ecrã*/
void write(float x, float y, void* tipo, char *s){

	char *aux;
	glRasterPos2f(x,y);
	for(aux = s; *aux != '\0'; aux++) glutBitmapCharacter(tipo,*aux);
}

/*Função principal para a projecção ortogonal no topo e do lado esquerdo*/ 
void texto(){
	
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1024, 0, 800);
	glScalef(1,-1,1);
	glTranslatef(0,-800,0);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0,1.0,1.0);
	char txt[100];
	detectaChaves(txt);

	write(30,30,(void *) GLUT_BITMAP_9_BY_15,txt);
	char text[100]; 
	sprintf_s(text,100, "fps: %4.2f", fps);
	write(30,50,(void *) GLUT_BITMAP_9_BY_15,text);
	sprintf_s(text,100, "chaves jogador: %d/%d", keysPlayer,nChaves);
	write(30,70,(void *) GLUT_BITMAP_9_BY_15,text);
	if(debug){
		sprintf(text, "x,y,z = (%.0f,%.0f,%.0f)", (player.x/factorMetro), (player.y/factorMetro), (player.z/factorMetro));
		write(30,90,(void *) GLUT_BITMAP_9_BY_15,text);
		sprintf(text,"View Frustum Culling: %s",frustumOn?"yes":"no");
		write(30,110,(void*) GLUT_BITMAP_9_BY_15,text);
		sprintf(text, "vbo = %s",vbo?"yes":"no");
		write(30,130,(void *) GLUT_BITMAP_9_BY_15,text);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_LIGHTING);
}

void menuFim(int estado){
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1024, 0, 800);
	glScalef(1,-1,1);
	glTranslatef(0,-800,0);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0,1.0,1.0);
	char txt[25];
	char txt1[25];
	char txt2[25];
	
	if(estado == 0){
		sprintf_s(txt,25,"Parabéns voce ganhou!!");
	}
	else{
		sprintf_s(txt,25,"Game Over!");
	}
	sprintf_s(txt1,25,"1 - Jogar outra vez.");
	sprintf_s(txt2,25,"2 - Sair do jogo.");
	
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2)-20,(void *) GLUT_BITMAP_TIMES_ROMAN_24,txt);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +30,(void *) GLUT_BITMAP_9_BY_15,txt1);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +60,(void *) GLUT_BITMAP_9_BY_15,txt2);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_LIGHTING);


}

void reiniciar(){
	end = 0;
	dead = 0;
	keysPlayer = 0;
	castel = 0;
	for(int i=0; i < nChaves; i++){keys[i] = 0;}
	player.x = 0;
	player.z = -(largura/2)+6;
}

void ajuda(){
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1024, 0, 800);
	glScalef(1,-1,1);
	glTranslatef(0,-800,0);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0,1.0,1.0);
	char txt3[10];
	char txt4[10];
	char txt5[18];
	char txt6[18];
	char txt7[18];
	char txt8[18];
	char txt9[25];
	char txt10[40];
	char txt11[40];
	sprintf_s(txt3,10,"AJUDA");
	sprintf_s(txt4,10,"Teclas:");
	sprintf_s(txt5,18,"A -> Direita");
	sprintf_s(txt6,18,"D -> Esquerda");
	sprintf_s(txt7,18,"S -> Tras");
	sprintf_s(txt8,18,"W -> Frente");
	sprintf_s(txt9,25,"V -> Mudar vista");
	sprintf_s(txt10,40,"+ -> Aumenta Velocidade do Jogador");
	sprintf_s(txt11,40,"- -> Diminui Velocidade do Jogador");
	
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2)-20,(void *) GLUT_BITMAP_9_BY_15,txt3);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +30,(void *) GLUT_BITMAP_9_BY_15,txt4);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +50,(void *) GLUT_BITMAP_9_BY_15,txt5);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +70,(void *) GLUT_BITMAP_9_BY_15,txt6);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +90,(void *) GLUT_BITMAP_9_BY_15,txt7);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +110,(void *) GLUT_BITMAP_9_BY_15,txt8);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +130,(void *) GLUT_BITMAP_9_BY_15,txt9);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +150,(void *) GLUT_BITMAP_9_BY_15,txt10);
	write(windowSizeInitial[0]/2,(windowSizeInitial[1]/2) +170,(void *) GLUT_BITMAP_9_BY_15,txt11);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_LIGHTING);
	
}

void menuAjuda(){
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1024, 0, 800);
	glScalef(1,-1,1);
	glTranslatef(0,-800,0);
	glMatrixMode(GL_MODELVIEW);

	glColor3f(1.0,1.0,1.0);
	char txt10[10];
		sprintf_s(txt10,10,"0 - AJUDA");
		write(windowSizeInitial[0] - 100,(windowSizeInitial[1])-10,(void *) GLUT_BITMAP_9_BY_15,txt10);
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();
	glEnable(GL_LIGHTING);
}