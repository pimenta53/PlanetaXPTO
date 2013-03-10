#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include "config.h"


extern int comprimento, largura;
extern int terrainTexture;
// dados para geracao do terreno - VBOs
extern GLuint buffers[3];
extern int* posicoes ;
extern float *vertexB, *normalB, *textB;



extern float * alturas;
// estrutura para o cálculo do vector normal de cada vertex
// representacao de um vector no espaço tridimensional
typedef struct{
	float x;
	float y;
	float z;
}Normais;

// variaveis necessarias neste ficheiro
Normais n,ab,ac;		// 3 vectores

GLuint grassTexture;	// variavel da textura

extern GLuint buffers[3];

/*Funcao que calcula a normal ao ponto x1,y1,z1, tendo em conta os pontos adjacentes a esquerda e direita*/
void calculateNormals (float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
	
	float d;
	
	ab.x = x2 - x1; 
	ab.y = y2 - y1;
	ab.z = z2 - z1;
	
	ac.x = x3 - x1;
	ac.y = y3 - y1;
	ac.z = z3 - z1;
	
	n.x = (ab.y * ac.z) - (ab.z * ac.y);
	n.y = (ab.z * ac.x) - (ab.x * ac.z);
	n.z = (ab.x * ac.y) - (ab.y * ac.x);
	
	//comprimento da normal
	d = sqrt((n.x*n.x) + (n.y*n.y) + (n.z*n.z));
	
	//normalizar
	n.x = n.x / d;
	n.y = n.y / d;
	n.z = n.z / d;
}



// função que recebe o array das alturas, a largura da matriz, a altura na posição X e Z da matriz de alturas dá a altura actual
float getAltura(float *alturas,int largura,int x,int z){  //correcta
	return alturas[x+(z*largura)];
}

float getAlturaSmoothWalk(float *alturas,int largura,float x,float z){
	float h=0.0f,fz,z1,fx,x1,h1,h2;
	z1=((int)z)*1.0f;
	x1=((int)x)*1.0f;
	fz = z - z1;fx = x - x1;
	h1 = (getAltura(alturas,largura,(int)x,(int) z)* (1-fz)) + (getAltura(alturas,largura,(int)x,((int) z)+1) * fz);
	h2 = (getAltura(alturas,largura,((int)x)+1,(int) z)* (1-fz)) + (getAltura(alturas,largura,((int)x)+1,((int) z)+1) * fz);
	h = h1 * (1 - fx) + h2 * fx;
	return h;
}

/*Funcao que "suaviza" as alturas do terreno,usando as alturas adjacentes em cada ponto e um factor de
 suavidade. Recebe o array das alturas,a altura e largura do array(matriz que este representa) e o
 n˙mero de vezes que vai ser aplicado este metodo*/
int smoothing(float ** alturas, int comprimento, int largura, int nr_vezes, float factor){
	for(int i = 0; i < nr_vezes; i++){
		// Linhas, esquerda para a direita
		float k = factor;
		int x, z;
		for(x = 1; x < largura; x++)
			for (z = 0; z < comprimento; z++)
				(*alturas)[x+(z*largura)] = getAltura(*alturas,largura,x-1,z) * (1-k) +getAltura(*alturas,largura,x,z) * k;
		
		// Linhas, direita para a esquerda
		for(x = largura-2; x < -1; x--)
			for (z = 0; z < comprimento; z++)
				(*alturas)[x+(z*largura)] = getAltura(*alturas,largura,x+1,z) * (1-k) + getAltura(*alturas,largura,x,z) * k;
		
		// Colunas, de baixo para cima
		for(x = 0; x < largura; x++)
			for (z = 1; z < comprimento; z++)
				(*alturas)[x+(z*largura)] = getAltura(*alturas,largura,x,z-1) * (1-k) + getAltura(*alturas,largura,x,z) * k;
		
		// Colunas, de cima para baixo
		for(x = 0; x < largura; x++)
			for (z = comprimento; z < -1; z--)
				(*alturas)[x+(z*largura)] = getAltura(*alturas,largura,x,z+1) * (1-k) + getAltura(*alturas,largura,x,z) * k;
	}
	return 1;
}


/*Funcao que carrega a imagem, recebendo o nome da imagem(url), a altura maxima em metros do terreno(para calcular ja o array 
 das alturas(matriz)) e retorna por referencia o apontador para a os dados da imagem(array com rgb de cada pixel), para as alturas(array com 
 as alturas ja calculadas tendo em consideraÁao a altura maxima), altura e largura da imagem(int)*/ 
GLuint loadImageTerrain (ILubyte** image,float** alturas ,int altura_max,int *comprimento,int * largura){
	int x = 0, i = 0;
	float min = altura_max*1.0f;
	ILuint ImgId;
	ILboolean success;
	ilGenImages(1,&ImgId);
	ilBindImage(ImgId);
	success = ilLoadImage ((ILstring)TERRAIN);
	if (success){
		success = ilConvertImage(IL_RGB,IL_UNSIGNED_BYTE);
		*image = ilGetData();
		*comprimento = ilGetInteger(IL_IMAGE_HEIGHT);
		*largura = ilGetInteger(IL_IMAGE_WIDTH);
		(*alturas) = (float*)malloc((*comprimento)*(*largura)*sizeof(float)); //aloca memoria para as alturas
		for(x = 0; x < (*comprimento)*(*largura)*3; x+= 3){
			(*alturas)[i] = ((((*image)[x]+(*image)[x+1]+(*image)[x+2])/3)*altura_max)/255.0f; // calcula as alturas
			if((*alturas)[i] < min && (*alturas)[i] >= 0.0f){
				min = (*alturas)[i];
			}
			i++;
		}
		for(x = 0; x < ((*comprimento)*(*largura)); x++){
			(*alturas)[x] = (*alturas)[x]-min;
		}
	}else{
		printf("ERRO\n");
		return 0;
	}
	smoothing(alturas,*comprimento,*largura,5,0.75f);
	ilDeleteImages(1, &ImgId);
	return 1;
}

/*Funcao que carrega uma textura,para que depois possa ser aplicada no terreno*/
GLuint loadImageTexture(){
	ILuint ImgId;
	ILboolean success;
	ilGenImages(1,&ImgId);
	ilBindImage(ImgId);
	success = ilLoadImage ((const ILstring)TEXTURE);
	if (success){
		success = ilConvertImage(IL_RGB,IL_UNSIGNED_BYTE);
		glGenTextures(1, &grassTexture);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		ILubyte* texData = ilGetData();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
					 texData);
	}else{
		printf("ERRO\n");
		return 0;
	}
	ilDeleteImages(1, &ImgId);
	return grassTexture;
}

/*Funcao que desenha o terreno,recebendo a altura,largura e o array com as alturas*/
void drawTerrain(int comprimento,int largura,float * alturas){
	
	char textura[]=TEXTURE;
	glColor3f(0.0,206/255.0f,0.0);
	if(textura[0] != '\0'){
		glEnable(GL_TEXTURE_2D);
		loadImageTexture();
		glBindTexture(GL_TEXTURE_2D, grassTexture);
	}
	for (int z = 0; z < comprimento-1; z++){
        glBegin(GL_TRIANGLE_STRIP);
        for (int x = 0; x < largura-1; x++){
			
			// desenha vertex 1
            if(terrainTexture) glTexCoord2f(0.0f, 0.0f);
			calculateNormals((float)x,getAltura(alturas,largura,x,z),(float)z,(float)x,getAltura(alturas,largura,x,z+1),(float)z+1,(float)x+1,getAltura(alturas,largura,x+1,z),(float)z);
			glNormal3f(n.x,n.y,n.z);
            glVertex3f((float)x-(comprimento/2),getAltura(alturas,largura,x,z), (float)z-(largura/2));
			
            // desenha vertex 2
            if(terrainTexture) glTexCoord2f(1.0f, 0.0f);
			calculateNormals((float)x+1,getAltura(alturas,largura,x+1,z),(float)z,(float)x+1,getAltura(alturas,largura,x+1,z+1),(float)z+1,(float)x+2,getAltura(alturas,largura,x+2,z),(float)z);
			glNormal3f(n.x,n.y,n.z);
            glVertex3f((float)(x+1)-(comprimento/2),getAltura(alturas,largura,x+1,z),(float)z-(largura/2));
			
            // desenha vertex 3
            if(terrainTexture) glTexCoord2f(0.0f, 1.0f);
			calculateNormals((float)x,getAltura(alturas,largura,x,z+1),(float)z+1,(float)x,getAltura(alturas,largura,x,z+2),(float)z+2,(float)x+1,getAltura(alturas,largura,x+1,z+1),(float)z+1);
			glNormal3f(n.x,n.y,n.z);
            glVertex3f((float)x-(comprimento/2), getAltura(alturas,largura,x,z+1) ,(float)(z+1)-(largura/2));
			
			// desenha vertex 4
            if(terrainTexture) glTexCoord2f(1.0f, 1.0f);
			calculateNormals((float)x+1,getAltura(alturas,largura,x+1,z+1),(float)z+1,(float)x+1,getAltura(alturas,largura,x+1,z+2),(float)z+2,(float)x+2,getAltura(alturas,largura,x+2,z+1),(float)z+1);
			glNormal3f(n.x,n.y,n.z);
			glVertex3f((float)(x+1)-(comprimento/2),getAltura(alturas,largura,x+1,z+1),(float)(z+1)-(largura/2));
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}

// desenha o terreno
void drawTerreno() {
	glPushMatrix();
		drawTerrain(comprimento,largura,alturas);
	glPopMatrix();
}

void generateVBO(GLuint *buffers,float *vertexB,float *normalB,float *textB,int comprimento,int largura,float *alturas,char *textura){

	int applytexture;
	if (textura[0] != '\0') applytexture = 1;
	else applytexture = 0;
	
	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	if(applytexture) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
	int size = comprimento * largura * sizeof(float)*3;
	posicoes = (int*)malloc(comprimento*largura*sizeof(int)*2);
	vertexB = (float*)malloc(size);
	normalB = (float*)malloc(size);
	if(applytexture) textB = (float*)malloc(comprimento*largura*sizeof(float)*2);
		
	int z = 0, x = 0;
	int pos = 0, typetext = 0;
	for(z = 0; z < comprimento; z++){
		for(x = 0; x < largura; x++){
			//calcula normais, vertices e coordenadas de textura
			calculateNormals((float)x,getAltura(alturas,largura,x,z),(float)z,(float)x,getAltura(alturas,largura,x,z+1),(float)z+1,(float)x+1,getAltura(alturas,largura,x+1,z),(float)z);
			normalB[pos++] = n.x;
			normalB[pos++] = n.y;
			normalB[pos++] = n.z;
			pos-=3;
			vertexB[pos++] = (float)x-(comprimento/2);
			vertexB[pos++] = getAltura(alturas,largura,x,z);
			vertexB[pos++] = (float)z-(largura/2);
			if (applytexture){
				textB[typetext++] = (float)x-(comprimento/2);
				textB[typetext++] = (float)z-(largura/2);
			}
		}
	}
	int k = 0;
	for(z = 1; z < comprimento; z++){
		for(x = 0; x < largura; x++){
			posicoes[k++] = largura*(z-1)+x;
			posicoes[k++] = z*largura+x;
		}
	}

	glGenBuffers(3, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, size,vertexB,GL_STATIC_DRAW);
    
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, size,normalB,GL_STATIC_DRAW);
	if(applytexture){
		glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
		glBufferData(GL_ARRAY_BUFFER, comprimento*largura*sizeof(float)*2,textB,GL_STATIC_DRAW);
		delete [] textB;
	}
	delete [] vertexB;
	delete [] normalB;
	glDisable(GL_TEXTURE_2D);
}

void drawTerrainVBO(int comprimento, int largura,float *alturas,char *textura){

	if(textura[0] != '\0'){
		glEnable(GL_TEXTURE_2D);
		loadImageTexture();
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		glBindBuffer(GL_ARRAY_BUFFER,buffers[2]);
		glTexCoordPointer(2,GL_FLOAT,0,0);
	}
	else glColor3f(0.0,206.0f/255.0f,0.0);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[1]);
	glNormalPointer(GL_FLOAT,0,0);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);
	
	int *pointer = posicoes;
	for (int i = 0; i < (comprimento-1); i++, pointer = pointer + largura*2){
		glDrawElements(GL_TRIANGLE_STRIP, (largura-1)*2 ,GL_UNSIGNED_INT, pointer);
	}
	glDisable(GL_TEXTURE_2D);
}