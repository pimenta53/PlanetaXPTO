#include "md2/Md2Player.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "glm.h"
#include "scene.h"
#include "terrain.h"

#include "Vec3.h"
#include "FrustumG.h"

extern int largura;								    //width da imagem
extern int comprimento;								//height da imagem
extern int bordaTerreno;
extern float* alturas;
extern int nAtingido;
extern float balas[nBalas][4];
extern float raioArvore;
extern int dead;
extern float velJogador;
extern float factorMetro;
extern float fps;
extern float vel;

extern Vec3 player, cam, direc;

extern GLMmodel *agente,*key,*tower,*arvore,*castelo,*tesouro;
extern Md2Player *boneco, *torreMD, *tree, *bullet;

extern int view ;
extern float alpha , beta ;			                //Angulos utilizados para calculo da posicao da camara e para onde ela aponta
extern float arvores[nArvores][3];
extern float posChave[nChaves][3];
extern float posTorre[nTorres][4];
extern int keys[nChaves];
extern int windowSizeInitial[2];
extern float factorMetro;
extern float endX;
extern float endZ;
extern int castel;

extern int displayList;
extern int keysPlayer;
extern int keysScene;
extern int vbo;

extern FrustumG frustum;
extern int frustumOn;
extern int move;

/*Radianos para Graus*/
float radToDeg(float val) {
	return ((180*val)/M_PI);
}

float degToRad(float val) {
	return val * M_PI / 180;
}

/*Fun��o que calcula o �ngulo, no plano XOZ, formado pelos pontos passados como par�metro com a posi��o actual do jogador*/ 
float calcAng(float b, float c, float playerx, float playerz){

	float dir[3];
	dir[0] = playerx - b;
	dir[2] = playerz - c;
	float dist = (sqrt(pow(dir[0], 2) + pow(dir[2], 2)));
	float ang = acos(dir[0] / dist);
	if (playerz > c) {
            ang = -ang;
	}
	return radToDeg(ang);
}

/*Fun��o que recebe dois arrays de floats (coordenadas de dois pontos) e calcula a dist�ncia entre estes*/
float dist(float a[3], float b[3]) {

	float dx, dy, dz;
	
	dx = a[0] - b[0];
	dy = a[1] - b[1];
	dz = a[2] - b[2];

	return ((sqrt(dx*dx + dy*dy + dz*dz)) / factorMetro);
}

/*Fun��o que calcula as coordenadas das torres aleatoriamente e armazena-as*/
void initTower(){
	
	// distribuir as torres no terreno
	for(int i = 0; i < nTorres; i++) {
		posTorre[i][XX] = ((rand()%(int)(largura))-(largura/2));		// posicao da chave em X
		posTorre[i][ZZ] = ((rand()%(int)(largura))-(largura/2));		// posicao da chave em Z
		posTorre[i][XX] < 0.0f ? posTorre[i][XX]+=bordaTerreno : posTorre[i][XX]-=bordaTerreno;			// coloca em X tendo em considera��o a borda
		posTorre[i][ZZ] < 0.0f ? posTorre[i][ZZ]+=bordaTerreno : posTorre[i][ZZ]-=bordaTerreno;			// coloca em Y tendo em considera��o a borda 
		posTorre[i][YY] = getAlturaSmoothWalk(alturas, largura, posTorre[i][XX]+largura/2, posTorre[i][ZZ]+largura/2)+0.3;	// posicao da chave em Y
		posTorre[i][3] = 0;
	}
}

/*Fun��o que calcula as coordenadas das �rvores aleatoriamente e guarda-as*/
void initArvores(){
	for(int i = 0; i < nArvores; i++) {
		arvores[i][XX] = ((rand()%(int)(largura))-(largura/2));		// posicao da chave em X
		arvores[i][ZZ] = ((rand()%(int)(largura))-(largura/2));		// posicao da chave em Z
		arvores[i][XX] < 0.0f ? arvores[i][XX]+=bordaTerreno : arvores[i][XX]-=bordaTerreno;			// coloca em X tendo em considera��o a borda
		arvores[i][ZZ] < 0.0f ? arvores[i][ZZ]+=bordaTerreno : arvores[i][ZZ]-=bordaTerreno;			// coloca em Y tendo em considera��o a borda 
		arvores[i][YY] = getAlturaSmoothWalk(alturas, largura, arvores[i][XX]+largura/2, arvores[i][ZZ]+largura/2)-0.75;	// posicao da chave em Y
	}
}

/*Fun��o que calcula as coordenadas das chaves aleatoriamente e guarda-as*/
void initChaves() {
	
	for(int i = 0; i < nChaves; i++) {
		keys[i] = 0;// caso a chave n�o tenha sido apanhada
		posChave[i][XX] = ((rand()%(int)(largura))-(largura/2));	// posicao da chave em X
		posChave[i][ZZ] = ((rand()%(int)(largura))-(largura/2));	// posicao da chave em Z
		posChave[i][XX] < 0.0f ? posChave[i][XX]+=bordaTerreno : posChave[i][XX]-=bordaTerreno;		// coloca em X tendo em considera��o a borda
		posChave[i][ZZ] < 0.0f ? posChave[i][ZZ]+=bordaTerreno : posChave[i][ZZ]-=bordaTerreno;		// coloca em Y tendo em considera��o a borda
		posChave[i][YY] = getAlturaSmoothWalk(alturas, largura, posChave[i][XX]+largura/2, posChave[i][ZZ]+largura/2);	// posicao da chave em Y
	}
}


void initScence(){

	// inicializa��o das vari�veis X e Y para o agente
	player.x =  0;		// posicao inicial em X
	player.z =  -(largura/2)+6;		// posicao inicial em Y
	player.y = getAlturaSmoothWalk(alturas, largura, player.x+largura/2, player.z+largura/2);
	endX = 0;
	endZ = (largura/2)-5;

	initChaves();
	initTower();
    initArvores();
}

/*Desenho do agente, num extremo do vale*/
void drawPlayer() {
	player.y = getAlturaSmoothWalk(alturas, largura, player.x+largura/2, player.z+largura/2);
	if(view == TPS) {
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glTranslatef(player.x, player.y, player.z);

		glRotatef(radToDeg(alpha), 0.0, 1.0, 0.0);
		if(move>0) {
			boneco->animate(0.5);
			boneco->drawPlayerItp (true,static_cast<Md2Object::Md2RenderMode>(0));
		}
		else {boneco->animate(0.0);
			boneco->drawPlayerFrame (0,static_cast<Md2Object::Md2RenderMode>(0));
		}
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();
	}
	glPushMatrix();
	
	glTranslatef(cam.x + 2*direc.x, cam.y + 2*direc.y, cam.z + 2*direc.z);
	glutSolidSphere(0.005, 10, 10);										
	glPopMatrix();	
}

/*Fun��o que verifica se a dist�ncia entre as chaves e o jogador � menor que radarChave e guarda para uma string a dist�ncia a que se encontra a chave mais pr�xima*/
void detectaChaves(char *ptr) {
	float posJogador[3];
	float chave[3];
	float distMin = radarChave;
	int numChave = -1;
	float aux;

	posJogador[XX] = player.x; 
	posJogador[YY] = player.y; 
	posJogador[ZZ] = player.z;
		
	for(int i=0; i<nChaves; i++) {
		if(!keys[i]) {
			chave[XX] = posChave[i][XX]; chave[YY] = posChave[i][YY]; chave[ZZ] = posChave[i][ZZ];
			aux = dist(chave,posJogador);
			if (aux < distMin && aux < radarChave) {
				distMin = aux;
				numChave = i;
			}
		}
	}
	// caso a chave mais pr�xima seja apanhada
	if(distMin < (distToque/factorMetro)) {
		keys[numChave] = 1;
		keysPlayer++;
	}
	else{
		if (numChave != -1) sprintf_s(ptr,100,"A chave mais proxima encontra-se a %.2f metros!",distMin);
	else{ if(keysPlayer==nChaves) sprintf_s(ptr,100,"TESOURO DESBLOQUEADO");
		else sprintf_s(ptr,100,"Nenhuma chave detectada!");
		}
	}
}

// desenha nArvores no cen�rio
void drawArvores(){

	for(int i=0; i<nArvores; i++){
		Vec3 b(arvores[i][XX]-23, arvores[i][YY], arvores[i][ZZ]-23);
		AABox aab(b,46,25,46);
		if (!frustumOn || (frustum.boxInFrustum(aab) != FrustumG::OUTSIDE)) {
			glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glTranslatef(arvores[i][XX], arvores[i][YY], arvores[i][ZZ]);
			tree->drawPlayerItp(0,static_cast<Md2Object::Md2RenderMode>(0));
			tree->animate(0.0);
			glPopMatrix();
		}
	}
}


//desenha balas que pra ja sao setas ou triangulos
void drawBala(int i){

	balas[i][2] += vel*sin(degToRad(-balas[i][0])); // posi��o z
	balas[i][1] += vel*cos(degToRad(-balas[i][0])); // posi��o x
	
	//verifica se sai do terreno
	if(balas[i][2] > (comprimento/2) || -balas[i][2] > (comprimento/2)) balas[i][3]=0.0;
	if(balas[i][1] > (largura/2) || -balas[i][1] > (largura/2)) balas[i][3]=0.0;

	//verificar se bate contra arvore
	for(int j=0; j<nArvores; j++){
		if((pow((balas[i][1]-arvores[j][XX]),2)+pow((balas[i][2]-arvores[j][ZZ]),2)) <= pow(raioArvore,2)){
		   balas[i][3]=0.0;
		}
	}

	if(castel == 1 && (balas[i][1]>endX-6 && balas[i][1]<endX+6) && (balas[i][2]>endZ-6 && balas[i][2]<endZ+6)) { balas[i][3]=0.0;}

	if(balas[i][3]==0.0) return;	
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(balas[i][1],getAlturaSmoothWalk(alturas, largura, balas[i][1]+largura/2, balas[i][2]+largura/2) + (1*factorMetro),balas[i][2]);
	glRotatef(balas[i][0] , 0, 1, 0);
	bullet->drawPlayerItp(0,static_cast<Md2Object::Md2RenderMode>(0));
	bullet->animate(0.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();   
	
	//quando bala atinge boneco � desactivada e conta que atingiu

	if((pow((player.x-balas[i][1]),2)+pow((player.z-balas[i][2]),2)) <= pow(1.0,2)) {
		nAtingido += 1;
		balas[i][3] = 0.0;
		dead += 1;
		return;
	}
}

//pega na posi��o do jogador e "dispara a bala"
void dispara(float playerX, float playerZ, float torreX, float torreY, float torreZ){
    
	float anguloDisp = calcAng(torreX,torreZ,playerX, playerZ);
    
    for(int i=0; i<nBalas; i++){
        if(balas[i][3]!=1.0){  // caso a posi��o da bala esteja vazia
			balas[i][0] = anguloDisp;  //angulo de disparo da bala
			balas[i][1] = torreX;      //posi��o da torre de onde foi disparada X
			balas[i][2] = torreZ;      //posi��o da torre de onde foi disparada Z
            balas[i][3] = 1.0; //esta em ac��o			
            return;
        }
	}
}

void detectaTorres(){

	float posJogador[3];
	float torre[3];
	
	posJogador[0] = player.x; posJogador[1] = player.y; posJogador[2] = player.z;
	
	glPushMatrix();
	for(int i = 0; i < nTorres; i++){
		torre[0] = posTorre[i][0]; torre[1] = posTorre[i][1]; torre[2] = posTorre[i][2];
		float aux = dist(torre,posJogador);
		if(aux < radarTorre) {
			Vec3 b(torre[0]-3,torre[1],torre[2]-3);
			AABox aab(b,6,6,6);
			if (!frustumOn || (frustum.boxInFrustum(aab) != FrustumG::OUTSIDE)) {
				glPushMatrix();
				glEnable(GL_TEXTURE_2D);
				glTranslatef(posTorre[i][0],posTorre[i][1],posTorre[i][2]);
				glRotatef(calcAng(posTorre[i][0],posTorre[i][2], player.x, player.z), 0, 1, 0);         
				torreMD->drawPlayerItp(0,static_cast<Md2Object::Md2RenderMode>(0));
				torreMD->animate(0.0);
				glDisable(GL_TEXTURE_2D);
				glPopMatrix();     
			}
			//para ver se passaram 3 segundos para poder disparar
			if(((posTorre[i][3])+3000) < glutGet(GLUT_ELAPSED_TIME)){
			//desenha bala
			dispara(player.x,player.z,posTorre[i][0], posTorre[i][1], posTorre[i][2]);
			posTorre[i][3] = glutGet(GLUT_ELAPSED_TIME);			
			}
		}
		else{
	      glPushMatrix();
		  glEnable(GL_TEXTURE_2D);
		  glTranslatef(posTorre[i][0],posTorre[i][1],posTorre[i][2]);
          torreMD->drawPlayerItp(0,static_cast<Md2Object::Md2RenderMode>(0));
		  torreMD->animate(0.0);
		  glDisable(GL_TEXTURE_2D);
		  glPopMatrix();   
		}
	}
	glPopMatrix();
}

/*Fun��o que desenha as chaves no cen�rio consoante as suas coordenadas calculadas*/
void drawChave(){
	float altura1,altura2;
	glPushMatrix();

	for(int i = 0; i < nChaves; i++){
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
		if(keys[i]==0) {
			glEnable(GL_TEXTURE_2D);
			glTranslatef(posChave[i][0],posChave[i][1],posChave[i][2]);
			// calcula as alturas da chave (para uma ponta e para a outra)
			altura1 = getAlturaSmoothWalk(alturas, largura, posChave[i][XX]-2.5+largura/2, posChave[i][ZZ]+largura/2);
			altura2 = getAlturaSmoothWalk(alturas, largura, posChave[i][XX]+2.5+largura/2, posChave[i][ZZ]+largura/2);
			glRotatef(atan((-1*(altura1-altura2))/5)*RAD,0,0,1);
			glmDraw(key,GLM_SMOOTH|GLM_MATERIAL);
			glDisable(GL_TEXTURE_2D);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPopMatrix();
}

/*Fun��o que desenha o tesouro,no vale, no extremo oposto ao agente*/
void drawCastelo() {
	glmDraw(tesouro,GLM_SMOOTH|GLM_MATERIAL);
}

// verifica se o tesouro aparece
void setCastelo() {
	// desenha apenas se apanhou todas as chaves
	float altura1, altura2;

	if(keysPlayer == nChaves){
		altura1 = getAltura(alturas, largura, endX+(largura/2), endZ+(largura/2));
		Vec3 b(endX-2,altura1,endZ-2);
		AABox aab(b,4,2,4);
		if (!frustumOn || (frustum.boxInFrustum(aab) != FrustumG::OUTSIDE)) {
			glPushMatrix();
			glTranslatef(endX,getAltura(alturas, largura, endX+(largura/2), endZ+(largura/2))+1, endZ);
			altura1 = getAlturaSmoothWalk(alturas, largura, endX-2.5+largura/2, endZ+largura/2);
			altura2 = getAlturaSmoothWalk(alturas, largura, endX+2.5+largura/2, endZ+largura/2);
			glRotatef(atan((-1*(altura1-altura2))/5)*RAD,0,0,1);
			glRotatef(180,0,1,0);

			if(!vbo) glCallList(displayList+1);
			else drawCastelo();
			glPopMatrix();			
		}
	}
}

// escala
void escala() {
	int i;
	glColor3f(0.8, 0.6, 0.4);
	for(i=0; i<5; i++) {
		glPushMatrix();
			glTranslatef(i, getAltura(alturas, largura, i, 0), 0);
		glPopMatrix();
	}
}

// centro
void centro() {
	glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(0, getAltura(alturas, largura, 0, 0), 0);
	glPopMatrix();
	glPushMatrix();
		glColor3f(1, 1, 1);
		glTranslatef(largura/2, 50, largura/2);
	glPopMatrix();
}