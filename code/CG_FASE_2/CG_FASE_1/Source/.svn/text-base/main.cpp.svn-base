#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
//#include <IL/ilu.h>
#include "glm.h"
#include "terrain.h"
#include "scene.h"
#include "input.h"
#include "output.h"
#include "md2/Md2Player.h"
#include "vars.h"
#define _USE_MATH_DEFINES

/*Função responsável por criar um foco de luz para  bbo jogo*/
void focoLuz() {
	glPushMatrix();
		glColor3f(0.972, 0.901, 0.118);
		glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	glPopMatrix();
}

void changeSize(int w, int h) {
	
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	
	ratio = w * 1.0/ h;
	
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);
	
	// Set the correct perspective.
	gluPerspective(angle,ratio,nearP,farP);
	glMatrixMode(GL_MODELVIEW);
	
	frustum.setCamInternals(angle,ratio,nearP,farP);
}

void renderScene(void) {
	
	glClearColor(0.3f,0.6f,0.9f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);						// luz ambiente
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiff);					// luz difusa
	
	funcaoMove(1);
	updateDir();
	
	glLoadIdentity();
	gluLookAt(cam.x,cam.y,cam.z, 
		      look.x,look.y,look.z,
			  up.x,up.y,up.z);

	frustum.setCamDef(cam,look,up);

	if(!dead) {		// se não estiver morto 
		if(!end) {	// se não ganhou o jogo
			if(fullscreen) glutFullScreen(); 
			else glutReshapeWindow(windowSizeInitial[0], windowSizeInitial[1]);		
	
			//Desenhar o cenário	
			glCallList(displayList+0);				// terreno
			glCallList(displayList+1);				// iluminacao
			glCallList(displayList+2);				// árvores
			drawChave();
			drawPlayer();
			setCastelo();
			detectaTorres();
			texto();
			menuAjuda();
			if(help == 1) {
				ajuda();
			}
			
			frame++;									// actualiza o número de frames
			time1 = glutGet(GLUT_ELAPSED_TIME);			// actualiza o tempo
			if (time1 - timebase > 1000) {				// se > que 1 segundo
				fps = frame*1000.0/(time1-timebase);	// actualiza o número de frames por segundo
				timebase = time1;
				frame = 0;
			}

			for(int i=0; i<nBalas;i++) {
				if(balas[i][3] == 1.0) drawBala(i);
			}
		}
		else {
			menuFim(0); // caso ganhe
		}
	}
	else { // caso esteja morto
		menuFim(1);
	}
	glutSwapBuffers();
}

int main(int argc, char **argv) {


	// inicialização
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);									// posicao incial do canto superior esquerdo do ecrã
	glutInitWindowSize(windowSizeInitial[0], windowSizeInitial[1]);	// dimensão da janela (largura, altura) em pixeis
	glutCreateWindow(NAME);
	

	// registo de funções
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
    //registo da funções do teclado 
	glutKeyboardFunc(pressKey);
	glutKeyboardUpFunc(releaseKey);
	glutSpecialFunc(pressSpKey);
	glutSpecialUpFunc(releaseSpKey);
	
	// funções de registo do rato
	glutIgnoreKeyRepeat(1);
	glutPassiveMotionFunc(fpassiveMouse);
	glutSetCursor(GLUT_CURSOR_NONE);
	
	float w = glutGet(GLUT_WINDOW_WIDTH);
	float h = glutGet(GLUT_WINDOW_HEIGHT);
	glutWarpPointer(w/2,h/2);
	
    // settings para o openGL
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos); // luz direccional
	glEnable(GL_DEPTH_TEST); // activa o Z_bufffer
	glEnable(GL_LIGHTING); // activar a
	glEnable(GL_LIGHT0); //  iluminação
	glEnable(GL_COLOR_MATERIAL); // activar o uso da cor do objecto
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR,FogCol);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, FogStart);
	glFogf(GL_FOG_END, FogStop);
	
	glFogfv(GL_FOG_COLOR,FogCol);						
	glFogi(GL_FOG_MODE, GL_LINEAR);						
	glFogf(GL_FOG_START, FogStart);						
	glFogf(GL_FOG_END, FogStop);						
	
	ilInit();
	//iluInit();
	

	loadImageTerrain(&image,&alturas,ALTURA_MAX,&comprimento,&largura);
	//iluScale(larguraTerrreno,larguraTerrreno,8);

	PlaySound(TEXT("DATA/musica.wav"), NULL, SND_FILENAME | SND_ASYNC );
	srand((unsigned)time(NULL));	// random
	initScence();
	
	calcPos(alpha, beta);

	//carrega objectos
	key = glmReadOBJ("DATA/Key.obj");
	tesouro = glmReadOBJ("DATA/tesouro.obj");

	//carrega md2
	boneco = new Md2Player("DATA/md2","mario");
	boneco->setScale(0.02f);
	torreMD = new Md2Player("DATA/md2","turret");
	torreMD->setScale(0.1f);
	tree = new Md2Player("DATA/md2","tree");
	tree->setScale(0.3f);
	bullet = new Md2Player("DATA/md2","bullet");
	bullet->setScale(0.05f);
	
	// atribuição da escala dos objectos
	glmScale(key, 1);
	glmScale(tesouro, 3);

	if(vbo) generateVBO(buffers,vertexB,normalB,textB,comprimento,largura,alturas,test);

	// DISPLAY LISTS
	displayList = glGenLists(3);
	glNewList(displayList+0,GL_COMPILE);	 
		if(!vbo) drawTerreno();
		else drawTerrainVBO(comprimento,largura,alturas,test);
	glEndList();
	glNewList(displayList+1,GL_COMPILE);
		focoLuz();
		centro();
		escala();
	glEndList();
	glNewList(displayList+2,GL_COMPILE);
		drawArvores();
	glEndList();
	
	factorMetro = comprimento/larguraTerrreno;			// factor que determina quanto se consegue andar
	
	// entrar no ciclo do GLUT 
	glutMainLoop();

	return 0;
}
