//author: Alex Saenbandith and Seirra Smith
//date:  November 5, 2014
//description: A simple program that displays two 3D models and several textures as hanging "paintings". The
// user can change the position of the two lights on the models by holding either shift or ctrl and dragging
// the left mouse button along the screen. Different lighting can be achieved by right clicking to open up the menu.
// The various images can be animated using the menu as well which will display on the right wall.
// Proposed points: (14 or 15?) The program meets all the basic criteria as well as using unique 3D models and animation. The
// question mark comes into play because whether or not the program is "cool" or interesting enough to earn the last two
// points is really up in the air. Though I do feel that there is enough work put in that we should at least get 1 point

#define _CRT_SECURE_NO_WARNINGS

#include <math.h>
#include <iostream>
#include "glm.h"
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if defined(__APPLE__)
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include<GLUT/glut.h>
#else
#include <stdlib.h>
#include<GL/glut.h>
#define M_PI 3.1415926
#endif

using namespace std;

#define WIDTH 600
#define HEIGHT 600
#define TRUE 1
#define FALSE 0

// Model variables
GLMmodel *objmodelPtr;
GLMmodel *objmodelPtr2;

// Movement-related variables
double angle = 0;
double xpos = 0;
double zpos = -5;
double ypos = 0;
double step = 1.0;

// Color variables
GLfloat ambientColor[3];
GLfloat diffuseColor[3];
GLfloat specularColor[3];
GLfloat shininess[1];

// Lighting Variables
GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
double light_rotX = 0.0;
double light_rotY = 0.0;

GLfloat lightPosition2[] = { 5.0, 5.0, 5.0, 0.0 };
double light2_rotX = 0.0;
double light2_rotY = 0.0;

// Light-toggle variables
int shift_active = FALSE;
int control_active = FALSE;
int right_button_down = FALSE;
int left_button_down = FALSE;

// Mouse Variables
int previous_mouseX;
int previous_mouseY;
double view_rotX;
double view_rotY;
double zoom = 20.0;

// SubMenu variables
int subMenuID;
int subMenuID2;
int subMenuID3;

// Wall variables
GLfloat floorVertices[] = { -8, -2, -30, 8, -2, -30, 8, -2, 3, -8, -2, 3 };
GLfloat floorVerticesNormals[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };

GLfloat leftVertices[] = { -8, 9, -30, -8, -2, -30, -8, -2, 3, -8, 9, 3 };
GLfloat leftVerticesNormals[] = { -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };

GLfloat rightVertices[] = { 8, 9, -30, 8, -2, -30, 8, -2, 3, 8, 9, 3 };
GLfloat rightVerticesNormals[] = { -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0 };

GLfloat frontVertices[] = { 8, -2, 3, 8, 9, 3, -8, 9, 3, -8, -2, 3 };
GLfloat frontVerticesNormals[] = { 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1 };

GLfloat backVertices[] = { 8, -2, -30, 8, 9, -30, -8, 9, -30, -8, -2, -30 };
GLfloat backVerticesNormals[] = { 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1 };

// White wall color
GLfloat whiteColor[] = { 1.0, 1.0, 1.0, 1.0 };

// Blue floor color
GLfloat blueColor[] = { 0.0, 0.0, 1.0, 1.0 };

// Wall texture variables
GLfloat lwTex[] = { -7.98, 9, -30, -7.98, -2, -30, -7.98, -2, 3, -7.98, 9, 3 };
GLfloat lwTexV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

GLfloat rwTex[] = { 7.98, 9, -30, 7.98, -2, -30, 7.98, -2, 3, 7.98, 9, 3 };
GLfloat rwTexV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

GLfloat bwTex[] = { 8, -2, -29.98, 8, 9, -29.98, -8, 9, -29.98, -8, -2, -29.98 };
GLfloat bwTexV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

int wallTexWidth[3], wallTexHeight[3]; GLubyte * wallTexture[3];
GLuint wallTexName[3];

// Picture variables
GLuint picTexName[7];

int picAWidth, picAHeight;//Picture A variables
GLubyte * picATexture[30];
GLubyte *textureA = NULL;

GLfloat picAVertices[] = { -7.90, -1.0, 2.0, -7.90, -1.0, -2.0, -7.90, 2.0, -2.0, -7.90, 2.0, 2.0 };
GLfloat picATV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

int picBWidth, picBHeight;//Picture B variables
GLubyte * picBTexture[52];
GLubyte *textureB = NULL;

GLfloat picBVertices[] = { -7.90, 3.0, 2.0, -7.90, 3.0, -2.0, -7.90, 6.0, -2.0, -7.90, 6.0, 2.0 };
GLfloat picBTV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

int picCWidth, picCHeight;//Picture C variables
GLubyte * picCTexture[15];
GLubyte *textureC = NULL;

GLfloat picCVertices[] = {-7.90, -1.0, -14.0, -7.90, -1.0, -10.0, -7.90, 2.0, -10.0, -7.90, 2.0, -14.0 };
GLfloat picCTV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

int picDWidth, picDHeight;//Picture D variables
GLubyte * picDTexture[24];
GLubyte *textureD = NULL;

GLfloat picDVertices[] = { -7.90, 3.0, -14.0, -7.90, 3.0, -10.0, -7.90, 6.0, -10.0, -7.90, 6.0, -14.0 };
GLfloat picDTV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

int picEWidth, picEHeight;//Picture E variables
GLubyte * picETexture[38];
GLubyte *textureE = NULL;

GLfloat picEVertices[] = { 7.90, -1.0, -16.0, 7.90, -1.0, -6.0, 7.90, 8.0, -6.0, 7.90, 8.0, -16.0 };
GLfloat picETV[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0 };


//Frame management variables
int frameDelay = 70;
int frameSwitch = 0;
int frameSetA = 0;
int frameSetB = 0;
int frameSetC = 0;
int frameSetD = 0;
int frameSetE = 0;
int frameSetF = 0;
int frameSetG = 0;


//function prototypes
void display();
void reshape(int w, int h);
void init(void);
void mouse_moved(int x, int y);
void mouse(int button, int state, int x, int y);
void key(unsigned char key, int x, int y);

void updateCamera(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 600.0);
	/*rotation and translation*/
	glRotatef(angle, 0, 1, 0);
	glTranslatef(xpos, ypos, zpos);
	cout << xpos << endl;
	cout << ypos << endl;
	cout << zpos << endl;
	cout << " " << endl;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutPostRedisplay();
}

void display(void) {
	/*clear all pixels*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*code to allow 3D navigation*/
	glPushMatrix();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glTranslatef(0, -1, -10);
	glmDraw(objmodelPtr2, GLM_SMOOTH);

	glPopMatrix();
	glPushMatrix();
	glTranslatef(1, -1, -11);
	glmDraw(objmodelPtr, GLM_SMOOTH);
	glPopMatrix();

	//draw the floor
	glMaterialfv(GL_FRONT, GL_AMBIENT, blueColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blueColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	glVertexPointer(3, GL_FLOAT, 0, floorVertices);
	glNormalPointer(GL_FLOAT, 0, floorVerticesNormals);
	glDrawArrays(GL_POLYGON, 0, 4);

	//draw walls
	glMaterialfv(GL_FRONT, GL_AMBIENT, whiteColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteColor);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// right wall
	glVertexPointer(3, GL_FLOAT, 0, rightVertices);
	glNormalPointer(GL_FLOAT, 0, rightVerticesNormals);
	glDrawArrays(GL_POLYGON, 0, 4);

	//left wall
	glVertexPointer(3, GL_FLOAT, 0, leftVertices);
	glNormalPointer(GL_FLOAT, 0, leftVerticesNormals);
	glDrawArrays(GL_POLYGON, 0, 4);

	//back wall
	glVertexPointer(3, GL_FLOAT, 0, backVertices);
	glNormalPointer(GL_FLOAT, 0, backVerticesNormals);
	glDrawArrays(GL_POLYGON, 0, 4);

	//front wall
	//glVertexPointer(3, GL_FLOAT, 0, frontVertices);
	//glNormalPointer(GL_FLOAT, 0, frontVerticesNormals);
	//glDrawArrays(GL_POLYGON, 0, 4);

	//Apply texture to walls
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindTexture(GL_TEXTURE_2D, wallTexName[0]); //left wall texture
	glVertexPointer(3, GL_FLOAT, 0, lwTex);
	glTexCoordPointer(2, GL_FLOAT, 0, lwTexV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindTexture(GL_TEXTURE_2D, wallTexName[0]); //right wall texture
	glVertexPointer(3, GL_FLOAT, 0, rwTex);
	glTexCoordPointer(2, GL_FLOAT, 0, rwTexV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindTexture(GL_TEXTURE_2D, wallTexName[0]); //back wall texture
	glVertexPointer(3, GL_FLOAT, 0, bwTex);
	glTexCoordPointer(2, GL_FLOAT, 0, bwTexV);
	glDrawArrays(GL_POLYGON, 0, 4);

	//Apply picture textures
	glBindTexture(GL_TEXTURE_2D, picTexName[0]); //Picture A texture
	glVertexPointer(3, GL_FLOAT, 0, picAVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, picATV);
	glDrawArrays(GL_POLYGON, 0, 4);
	
	glBindTexture(GL_TEXTURE_2D, picTexName[1]); //Picture B texture
	glVertexPointer(3, GL_FLOAT, 0, picBVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, picBTV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindTexture(GL_TEXTURE_2D, picTexName[2]); //Picture C texture
	glVertexPointer(3, GL_FLOAT, 0, picCVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, picCTV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindTexture(GL_TEXTURE_2D, picTexName[3]); //Picture D texture
	glVertexPointer(3, GL_FLOAT, 0, picDVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, picDTV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glBindTexture(GL_TEXTURE_2D, picTexName[4]); //Picture E texture
	glVertexPointer(3, GL_FLOAT, 0, picEVertices);
	glTexCoordPointer(2, GL_FLOAT, 0, picETV);
	glDrawArrays(GL_POLYGON, 0, 4);

	glEnable(GL_LIGHTING);


	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glPopMatrix();
	glutSwapBuffers();
}

void init(void) {
	/*set clearing (background) color*/
	glClearColor(0.0, 0.0, 0.0, 0.0);
	
	updateCamera();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*enable lighting and depth*/
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);

	GLfloat whiteLight[] = { 1.0, 1.0, 1.0 };
	GLfloat ambientLight[] = { 0.5, 0.5, 0.5 };
	/*set up lighting parameters*/
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteLight);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, whiteLight);
	glLightfv(GL_LIGHT1, GL_SPECULAR, whiteLight);

	// Ambient color
	ambientColor[0] = 0.5;
	ambientColor[1] = 0.0;
	ambientColor[2] = 0.0;
	// Diffuse color
	diffuseColor[0] = 1.0;
	diffuseColor[1] = 0.0;
	diffuseColor[2] = 0.0;
	// Specular color
	specularColor[0] = 1.0;
	specularColor[1] = 1.0;
	specularColor[2] = 1.0;
	shininess[0] = 50.0;
	

	//Generate textures
	glGenTextures(1, wallTexName);
	glGenTextures(7, picTexName);

	//Read in wall texture
	glBindTexture(GL_TEXTURE_2D, wallTexName[0]);
	wallTexture[0] = glmReadPPM("space.ppm", &wallTexWidth[0], &wallTexHeight[0]); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wallTexWidth[0], wallTexHeight[0], 0, GL_RGB, GL_UNSIGNED_BYTE, wallTexture[0]);

	//Read in picture textures (Oh so many textures)
	
	// Picture A textures
	glBindTexture(GL_TEXTURE_2D, picTexName[0]);

	textureA = glmReadPPM("a1.ppm", &picAWidth, &picAHeight);// Begin Picture A texture read-in
	picATexture[0] = glmReadPPM("a1.ppm", &picAWidth, &picAHeight);
	picATexture[1] = glmReadPPM("a2.ppm", &picAWidth, &picAHeight);
	picATexture[2] = glmReadPPM("a3.ppm", &picAWidth, &picAHeight);
	picATexture[3] = glmReadPPM("a4.ppm", &picAWidth, &picAHeight);
	picATexture[4] = glmReadPPM("a5.ppm", &picAWidth, &picAHeight);
	picATexture[5] = glmReadPPM("a6.ppm", &picAWidth, &picAHeight);
	picATexture[6] = glmReadPPM("a7.ppm", &picAWidth, &picAHeight);
	picATexture[7] = glmReadPPM("a8.ppm", &picAWidth, &picAHeight);
	picATexture[8] = glmReadPPM("a9.ppm", &picAWidth, &picAHeight);
	picATexture[9] = glmReadPPM("a10.ppm", &picAWidth, &picAHeight);
	picATexture[10] = glmReadPPM("a11.ppm", &picAWidth, &picAHeight);
	picATexture[11] = glmReadPPM("a12.ppm", &picAWidth, &picAHeight);
	picATexture[12] = glmReadPPM("a13.ppm", &picAWidth, &picAHeight);
	picATexture[13] = glmReadPPM("a14.ppm", &picAWidth, &picAHeight);
	picATexture[14] = glmReadPPM("a15.ppm", &picAWidth, &picAHeight);
	picATexture[15] = glmReadPPM("a16.ppm", &picAWidth, &picAHeight);
	picATexture[16] = glmReadPPM("a17.ppm", &picAWidth, &picAHeight);
	picATexture[17] = glmReadPPM("a18.ppm", &picAWidth, &picAHeight);
	picATexture[18] = glmReadPPM("a19.ppm", &picAWidth, &picAHeight);
	picATexture[19] = glmReadPPM("a20.ppm", &picAWidth, &picAHeight);
	picATexture[20] = glmReadPPM("a21.ppm", &picAWidth, &picAHeight);
	picATexture[21] = glmReadPPM("a22.ppm", &picAWidth, &picAHeight);
	picATexture[22] = glmReadPPM("a23.ppm", &picAWidth, &picAHeight);
	picATexture[23] = glmReadPPM("a24.ppm", &picAWidth, &picAHeight);
	picATexture[24] = glmReadPPM("a25.ppm", &picAWidth, &picAHeight);
	picATexture[25] = glmReadPPM("a26.ppm", &picAWidth, &picAHeight);
	picATexture[26] = glmReadPPM("a27.ppm", &picAWidth, &picAHeight);
	picATexture[27] = glmReadPPM("a28.ppm", &picAWidth, &picAHeight);
	picATexture[28] = glmReadPPM("a29.ppm", &picAWidth, &picAHeight);
	picATexture[29] = glmReadPPM("a30.ppm", &picAWidth, &picAHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, picATexture[0]);

	glBindTexture(GL_TEXTURE_2D, picTexName[1]);

	textureB = glmReadPPM("b1.ppm", &picBWidth, &picBHeight);// Begin Picture B texture read-in
	picBTexture[0] = glmReadPPM("b1.ppm", &picBWidth, &picBHeight);
	picBTexture[1] = glmReadPPM("b2.ppm", &picBWidth, &picBHeight);
	picBTexture[2] = glmReadPPM("b3.ppm", &picBWidth, &picBHeight);
	picBTexture[3] = glmReadPPM("b4.ppm", &picBWidth, &picBHeight);
	picBTexture[4] = glmReadPPM("b5.ppm", &picBWidth, &picBHeight);
	picBTexture[5] = glmReadPPM("b6.ppm", &picBWidth, &picBHeight);
	picBTexture[6] = glmReadPPM("b7.ppm", &picBWidth, &picBHeight);
	picBTexture[7] = glmReadPPM("b8.ppm", &picBWidth, &picBHeight);
	picBTexture[8] = glmReadPPM("b9.ppm", &picBWidth, &picBHeight);
	picBTexture[9] = glmReadPPM("b10.ppm", &picBWidth, &picBHeight);
	picBTexture[10] = glmReadPPM("b11.ppm", &picBWidth, &picBHeight);
	picBTexture[11] = glmReadPPM("b12.ppm", &picBWidth, &picBHeight);
	picBTexture[12] = glmReadPPM("b13.ppm", &picBWidth, &picBHeight);
	picBTexture[12] = glmReadPPM("b14.ppm", &picBWidth, &picBHeight);
	picBTexture[14] = glmReadPPM("b15.ppm", &picBWidth, &picBHeight);
	picBTexture[15] = glmReadPPM("b16.ppm", &picBWidth, &picBHeight);
	picBTexture[16] = glmReadPPM("b17.ppm", &picBWidth, &picBHeight);
	picBTexture[17] = glmReadPPM("b18.ppm", &picBWidth, &picBHeight);
	picBTexture[18] = glmReadPPM("b19.ppm", &picBWidth, &picBHeight);
	picBTexture[19] = glmReadPPM("b20.ppm", &picBWidth, &picBHeight);
	picBTexture[20] = glmReadPPM("b21.ppm", &picBWidth, &picBHeight);
	picBTexture[21] = glmReadPPM("b22.ppm", &picBWidth, &picBHeight);
	picBTexture[22] = glmReadPPM("b23.ppm", &picBWidth, &picBHeight);
	picBTexture[23] = glmReadPPM("b24.ppm", &picBWidth, &picBHeight);
	picBTexture[24] = glmReadPPM("b25.ppm", &picBWidth, &picBHeight);
	picBTexture[25] = glmReadPPM("b26.ppm", &picBWidth, &picBHeight);
	picBTexture[26] = glmReadPPM("b27.ppm", &picBWidth, &picBHeight);
	picBTexture[27] = glmReadPPM("b28.ppm", &picBWidth, &picBHeight);
	picBTexture[28] = glmReadPPM("b29.ppm", &picBWidth, &picBHeight);
	picBTexture[29] = glmReadPPM("b30.ppm", &picBWidth, &picBHeight);
	picBTexture[30] = glmReadPPM("b31.ppm", &picBWidth, &picBHeight);
	picBTexture[31] = glmReadPPM("b32.ppm", &picBWidth, &picBHeight);
	picBTexture[32] = glmReadPPM("b33.ppm", &picBWidth, &picBHeight);
	picBTexture[33] = glmReadPPM("b34.ppm", &picBWidth, &picBHeight);
	picBTexture[34] = glmReadPPM("b35.ppm", &picBWidth, &picBHeight);
	picBTexture[35] = glmReadPPM("b36.ppm", &picBWidth, &picBHeight);
	picBTexture[36] = glmReadPPM("b37.ppm", &picBWidth, &picBHeight);
	picBTexture[37] = glmReadPPM("b38.ppm", &picBWidth, &picBHeight);
	picBTexture[38] = glmReadPPM("b39.ppm", &picBWidth, &picBHeight);
	picBTexture[39] = glmReadPPM("b40.ppm", &picBWidth, &picBHeight);
	picBTexture[40] = glmReadPPM("b41.ppm", &picBWidth, &picBHeight);
	picBTexture[41] = glmReadPPM("b42.ppm", &picBWidth, &picBHeight);
	picBTexture[42] = glmReadPPM("b43.ppm", &picBWidth, &picBHeight);
	picBTexture[43] = glmReadPPM("b44.ppm", &picBWidth, &picBHeight);
	picBTexture[44] = glmReadPPM("b45.ppm", &picBWidth, &picBHeight);
	picBTexture[45] = glmReadPPM("b46.ppm", &picBWidth, &picBHeight);
	picBTexture[46] = glmReadPPM("b47.ppm", &picBWidth, &picBHeight);
	picBTexture[47] = glmReadPPM("b48.ppm", &picBWidth, &picBHeight);
	picBTexture[48] = glmReadPPM("b49.ppm", &picBWidth, &picBHeight);
	picBTexture[49] = glmReadPPM("b50.ppm", &picBWidth, &picBHeight);
	picBTexture[50] = glmReadPPM("b51.ppm", &picBWidth, &picBHeight);
	picBTexture[51] = glmReadPPM("b52.ppm", &picBWidth, &picBHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, picBTexture[0]);

	glBindTexture(GL_TEXTURE_2D, picTexName[2]);

	textureC = glmReadPPM("c1.ppm", &picCWidth, &picCHeight);// Begin Picture C texture read-in
	picCTexture[0] = glmReadPPM("c1.ppm", &picCWidth, &picCHeight);
	picCTexture[1] = glmReadPPM("c2.ppm", &picCWidth, &picCHeight);
	picCTexture[2] = glmReadPPM("c3.ppm", &picCWidth, &picCHeight);
	picCTexture[3] = glmReadPPM("c4.ppm", &picCWidth, &picCHeight);
	picCTexture[4] = glmReadPPM("c5.ppm", &picCWidth, &picCHeight);
	picCTexture[5] = glmReadPPM("c6.ppm", &picCWidth, &picCHeight);
	picCTexture[6] = glmReadPPM("c7.ppm", &picCWidth, &picCHeight);
	picCTexture[7] = glmReadPPM("c8.ppm", &picCWidth, &picCHeight);
	picCTexture[8] = glmReadPPM("c9.ppm", &picCWidth, &picCHeight);
	picCTexture[9] = glmReadPPM("c10.ppm", &picCWidth, &picCHeight);
	picCTexture[10] = glmReadPPM("c11.ppm", &picCWidth, &picCHeight);
	picCTexture[11] = glmReadPPM("c12.ppm", &picCWidth, &picCHeight);
	picCTexture[12] = glmReadPPM("c13.ppm", &picCWidth, &picCHeight);
	picCTexture[13] = glmReadPPM("c14.ppm", &picCWidth, &picCHeight);
	picCTexture[14] = glmReadPPM("c15.ppm", &picCWidth, &picCHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, picCTexture[0]);

	glBindTexture(GL_TEXTURE_2D, picTexName[3]);

	textureD = glmReadPPM("d1.ppm", &picDWidth, &picDHeight);// Begin Picture D texture read-in
	picDTexture[0] = glmReadPPM("d1.ppm", &picDWidth, &picDHeight);
	picDTexture[1] = glmReadPPM("d2.ppm", &picDWidth, &picDHeight);
	picDTexture[2] = glmReadPPM("d3.ppm", &picDWidth, &picDHeight);
	picDTexture[3] = glmReadPPM("d4.ppm", &picDWidth, &picDHeight);
	picDTexture[4] = glmReadPPM("d5.ppm", &picDWidth, &picDHeight);
	picDTexture[5] = glmReadPPM("d6.ppm", &picDWidth, &picDHeight);
	picDTexture[6] = glmReadPPM("d7.ppm", &picDWidth, &picDHeight);
	picDTexture[7] = glmReadPPM("d8.ppm", &picDWidth, &picDHeight);
	picDTexture[8] = glmReadPPM("d9.ppm", &picDWidth, &picDHeight);
	picDTexture[9] = glmReadPPM("d10.ppm", &picDWidth, &picDHeight);
	picDTexture[10] = glmReadPPM("d11.ppm", &picDWidth, &picDHeight);
	picDTexture[11] = glmReadPPM("d12.ppm", &picDWidth, &picDHeight);
	picDTexture[12] = glmReadPPM("d13.ppm", &picDWidth, &picDHeight);
	picDTexture[13] = glmReadPPM("d14.ppm", &picDWidth, &picDHeight);
	picDTexture[14] = glmReadPPM("d15.ppm", &picDWidth, &picDHeight);
	picDTexture[15] = glmReadPPM("d16.ppm", &picDWidth, &picDHeight);
	picDTexture[16] = glmReadPPM("d17.ppm", &picDWidth, &picDHeight);
	picDTexture[17] = glmReadPPM("d18.ppm", &picDWidth, &picDHeight);
	picDTexture[18] = glmReadPPM("d19.ppm", &picDWidth, &picDHeight);
	picDTexture[19] = glmReadPPM("d20.ppm", &picDWidth, &picDHeight);
	picDTexture[20] = glmReadPPM("d21.ppm", &picDWidth, &picDHeight);
	picDTexture[21] = glmReadPPM("d22.ppm", &picDWidth, &picDHeight);
	picDTexture[22] = glmReadPPM("d23.ppm", &picDWidth, &picDHeight);
	picDTexture[23] = glmReadPPM("d24.ppm", &picDWidth, &picDHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, picDTexture[0]);

	glBindTexture(GL_TEXTURE_2D, picTexName[4]);

	textureE = glmReadPPM("i1.ppm", &picEWidth, &picEHeight);// Begin Picture E texture read-in
	picETexture[0] = glmReadPPM("i1.ppm", &picEWidth, &picEHeight);
	picETexture[1] = glmReadPPM("i2.ppm", &picEWidth, &picEHeight);
	picETexture[2] = glmReadPPM("i3.ppm", &picEWidth, &picEHeight);
	picETexture[3] = glmReadPPM("i4.ppm", &picEWidth, &picEHeight);
	picETexture[4] = glmReadPPM("i5.ppm", &picEWidth, &picEHeight);
	picETexture[5] = glmReadPPM("i6.ppm", &picEWidth, &picEHeight);
	picETexture[6] = glmReadPPM("i7.ppm", &picEWidth, &picEHeight);
	picETexture[7] = glmReadPPM("i8.ppm", &picEWidth, &picEHeight);
	picETexture[8] = glmReadPPM("i9.ppm", &picEWidth, &picEHeight);
	picETexture[9] = glmReadPPM("i10.ppm", &picEWidth, &picEHeight);
	picETexture[10] = glmReadPPM("i11.ppm", &picEWidth, &picEHeight);
	picETexture[11] = glmReadPPM("i12.ppm", &picEWidth, &picEHeight);
	picETexture[12] = glmReadPPM("i13.ppm", &picEWidth, &picEHeight);
	picETexture[13] = glmReadPPM("i14.ppm", &picEWidth, &picEHeight);
	picETexture[14] = glmReadPPM("i15.ppm", &picEWidth, &picEHeight);
	picETexture[15] = glmReadPPM("i16.ppm", &picEWidth, &picEHeight);
	picETexture[16] = glmReadPPM("i17.ppm", &picEWidth, &picEHeight);
	picETexture[17] = glmReadPPM("i18.ppm", &picEWidth, &picEHeight);
	picETexture[18] = glmReadPPM("i19.ppm", &picEWidth, &picEHeight);
	picETexture[19] = glmReadPPM("i20.ppm", &picEWidth, &picEHeight);
	picETexture[20] = glmReadPPM("i21.ppm", &picEWidth, &picEHeight);
	picETexture[21] = glmReadPPM("i22.ppm", &picEWidth, &picEHeight);
	picETexture[22] = glmReadPPM("i23.ppm", &picEWidth, &picEHeight);
	picETexture[23] = glmReadPPM("i24.ppm", &picEWidth, &picEHeight);
	picETexture[24] = glmReadPPM("i25.ppm", &picEWidth, &picEHeight);
	picETexture[25] = glmReadPPM("i26.ppm", &picEWidth, &picEHeight);
	picETexture[26] = glmReadPPM("i27.ppm", &picEWidth, &picEHeight);
	picETexture[27] = glmReadPPM("i28.ppm", &picEWidth, &picEHeight);
	picETexture[28] = glmReadPPM("i29.ppm", &picEWidth, &picEHeight);
	picETexture[29] = glmReadPPM("i30.ppm", &picEWidth, &picEHeight);
	picETexture[30] = glmReadPPM("i31.ppm", &picEWidth, &picEHeight);
	picETexture[31] = glmReadPPM("i32.ppm", &picEWidth, &picEHeight);
	picETexture[32] = glmReadPPM("i33.ppm", &picEWidth, &picEHeight);
	picETexture[33] = glmReadPPM("i34.ppm", &picEWidth, &picEHeight);
	picETexture[34] = glmReadPPM("i35.ppm", &picEWidth, &picEHeight);
	picETexture[35] = glmReadPPM("i36.ppm", &picEWidth, &picEHeight);
	picETexture[36] = glmReadPPM("i37.ppm", &picEWidth, &picEHeight);
	picETexture[37] = glmReadPPM("i38.ppm", &picEWidth, &picEHeight);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
		0, GL_RGB, GL_UNSIGNED_BYTE, picETexture[0]);

	// Read in 3D models
	if (!objmodelPtr)
	{
		objmodelPtr = glmReadOBJ("twilight.obj");
		if (!objmodelPtr) {
			cout << "error reading file" << endl;
			exit(0);
		}
		glmUnitize(objmodelPtr);
		glmFacetNormals(objmodelPtr);
		glmVertexNormals(objmodelPtr, 90.0);
	}

	if (!objmodelPtr2)
	{
		objmodelPtr2 = glmReadOBJ("celestia.obj");
		if (!objmodelPtr) {
			cout << "error reading file" << endl;
			exit(0);
		}
		glmUnitize(objmodelPtr2);
		glmFacetNormals(objmodelPtr2);
		glmVertexNormals(objmodelPtr2, 90.0);
	}
	

}

void special(int key, int x, int y){
	switch (key){
	case GLUT_KEY_UP: //move forward
		zpos = zpos + step * cos(angle * M_PI / 180);
		xpos = xpos - step * sin(angle * M_PI / 180);
		break;
	case GLUT_KEY_DOWN: //move backwards
		zpos = zpos - step * cos(angle * M_PI / 180);
		xpos = xpos + step * sin(angle * M_PI / 180);
		break;
	case GLUT_KEY_LEFT: //turn left
		angle = angle - 4.5;
		break;
	case GLUT_KEY_RIGHT: //color right
		angle = angle + 4.5;
		break;
	default:
		return;
	}
	updateCamera();
}

void reshape(int w, int h) {
	/*prevents user from reshaping window*/
	glutReshapeWindow(WIDTH, HEIGHT);
}

void mouse(int button, int state, int x, int y) {
	// Lighting code
	int mod = glutGetModifiers();
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON &&
		mod == GLUT_ACTIVE_SHIFT) {
		shift_active = TRUE;
	}
	else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON &&
		mod == GLUT_ACTIVE_CTRL) {
		control_active = TRUE;
	}
	else {
		shift_active = FALSE;
		control_active = FALSE;
	}

	//left mouse down rotation
	//right mouse down for zoom

	//keep track of where user pressed down
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		previous_mouseX = x;
		previous_mouseY = y;
		left_button_down = TRUE;
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		previous_mouseX = x;
		previous_mouseY = y;
		right_button_down = TRUE;
	}

	else {
		left_button_down = FALSE;
		right_button_down = FALSE;
	}

	glutPostRedisplay();
}

void mouseMoved(int x, int y) {

	double thetaY, thetaX;
	//left mouse effects rotation
	if (left_button_down == TRUE && !shift_active && !control_active) {
		thetaY = 360.0 * (x - previous_mouseX) / WIDTH;
		thetaX = 360.0 * (y - previous_mouseY) / HEIGHT;
		previous_mouseX = x;
		previous_mouseY = y;
		view_rotX += thetaX;
		view_rotY += thetaY;
	}

	// left mouse + shift affects light #1
	if (left_button_down && shift_active) {
		glPushMatrix();
		thetaY = 360.0 * (x - previous_mouseX) / WIDTH;
		thetaX = 360.0 * (y - previous_mouseY) / HEIGHT;
		previous_mouseX = x;
		previous_mouseY = y;
		light_rotX += thetaX;
		light_rotY += thetaY;
		glRotatef(light_rotX, 1, 0, 0);
		glRotatef(light_rotY, 0, 0, 1);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glPopMatrix();
	}

	if (left_button_down && control_active) {
		glPushMatrix();
		thetaY = 360.0 * (x - previous_mouseX) / WIDTH;
		thetaX = 360.0 * (y - previous_mouseY) / HEIGHT;
		previous_mouseX = x;
		previous_mouseY = y;
		light2_rotX += thetaX;
		light2_rotY += thetaY;
		glRotatef(light2_rotX, 1, 0, 0);
		glRotatef(light2_rotY, 0, 0, 1);
		glLightfv(GL_LIGHT1, GL_POSITION, lightPosition2);
		glPopMatrix();
	}

	//right mouse zoom
	if (right_button_down == TRUE) {
		zoom += (x - previous_mouseX) / 100.0;
		previous_mouseX = x;

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0);

	}
	glutPostRedisplay();

}

void menu(int value){
	/*Establishes initial menu
	*/
}

void lightingSubMenu(int value){
	if (value == 0){
		//ambient color
		ambientColor[0] = 0.5;
		ambientColor[1] = 0.0;
		ambientColor[2] = 0.0;
		//diffuse color
		diffuseColor[0] = 0.0;
		diffuseColor[1] = 0.0;
		diffuseColor[2] = 0.0;
		//specular color
		specularColor[0] = 0.0;
		specularColor[1] = 0.0;
		specularColor[2] = 0.0;
		shininess[0] = 0.0;

		cout << "Ambient" << endl;
	}
	else if (value == 1){
		//ambient color
		ambientColor[0] = 0.0;
		ambientColor[1] = 0.0;
		ambientColor[2] = 0.0;
		//diffuse color
		diffuseColor[0] = 1.0;
		diffuseColor[1] = 0.0;
		diffuseColor[2] = 0.0;
		//specular color
		specularColor[0] = 0.0;
		specularColor[1] = 0.0;
		specularColor[2] = 0.0;
		shininess[0] = 0.0;

		cout << "Diffuse" << endl;
	}
	else if (value == 2){
		//ambient color
		ambientColor[0] = 0.0;
		ambientColor[1] = 0.0;
		ambientColor[2] = 0.0;
		//diffuse color
		diffuseColor[0] = 0.0;
		diffuseColor[1] = 0.0;
		diffuseColor[2] = 0.0;
		//specular color
		specularColor[0] = 1.0;
		specularColor[1] = 1.0;
		specularColor[2] = 1.0;
		shininess[0] = 50.0;
		cout << "Specular" << endl;
	}
	else if (value == 3){
		//ambient color
		ambientColor[0] = 0.5;
		ambientColor[1] = 0.0;
		ambientColor[2] = 0.0;
		//diffuse color
		diffuseColor[0] = 1.0;
		diffuseColor[1] = 0.0;
		diffuseColor[2] = 0.0;
		//specular color
		specularColor[0] = 1.0;
		specularColor[1] = 1.0;
		specularColor[2] = 1.0;
		shininess[0] = 50.0;

		cout << "All" << endl;
	}

	glutPostRedisplay();
}

void shadingMenu(int value){
	if (value == 0){
		glShadeModel(GL_FLAT); //flat shading
	}
	else if (value == 1){
		glShadeModel(GL_SMOOTH); //Gouraud shading
	}
	glutPostRedisplay();
}

void animationMenu(int value){
	if (value == 1){
		frameSwitch = 1;
	}
	else if (value == 2){
		frameSwitch = 2;
	}
	else if (value == 3){
		frameSwitch = 3;
	}
	else if (value == 4){
		frameSwitch = 4;
	}
	else if (value == 5){
		frameSwitch = 5;
	}
	glutPostRedisplay();
}

void timer(int extra){
	if (frameSwitch == 1){

		// Picture A frame animation
		if (frameSetA == 0){
			textureA = picATexture[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 1){
			textureA = picATexture[1];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 2){
			textureA = picATexture[2];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 3){
			textureA = picATexture[3];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 4){
			textureA = picATexture[4];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 5){
			textureA = picATexture[5];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 6){
			textureA = picATexture[6];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 7){
			textureA = picATexture[7];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 8){
			textureA = picATexture[8];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 9){
			textureA = picATexture[9];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 10){
			textureA = picATexture[10];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 11){
			textureA = picATexture[11];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 12){
			textureA = picATexture[12];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 13){
			textureA = picATexture[13];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
			cout << "frame 14" << endl;
		}
		else if (frameSetA == 14){
			textureA = picATexture[14];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 15){
			textureA = picATexture[15];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 16){
			textureA = picATexture[16];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 17){
			textureA = picATexture[17];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 18){
			textureA = picATexture[18];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 19){
			textureA = picATexture[19];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 20){
			textureA = picATexture[20];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 21){
			textureA = picATexture[21];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
			cout << "frame 22" << endl;
		}
		else if (frameSetA == 22){
			textureA = picATexture[22];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 23){
			textureA = picATexture[23];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 24){
			textureA = picATexture[24];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
			cout << "frame 24" << endl;
		}
		else if (frameSetA == 25){
			textureA = picATexture[25];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 26){
			textureA = picATexture[26];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 27){
			textureA = picATexture[27];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 28){
			textureA = picATexture[28];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA == 29){
			textureA = picATexture[29];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picAWidth, picAHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureA);
			glutPostRedisplay();
			frameSetA++;
		}
		else if (frameSetA > 29){
			frameSetA = 0;
		}
	}

	if (frameSwitch == 2){
		// Picture B frame animation
		if (frameSetB == 0){
			textureB = picBTexture[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 1){
			textureB = picBTexture[1];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 2){
			textureB = picBTexture[2];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 3){
			textureB = picBTexture[3];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 4){
			textureB = picBTexture[4];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 5){
			textureB = picBTexture[5];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 6){
			textureB = picBTexture[6];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 7){
			textureB = picBTexture[7];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 8){
			textureB = picBTexture[8];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 9){
			textureB = picBTexture[9];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 10){
			textureB = picBTexture[10];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 11){
			textureB = picBTexture[11];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 12){
			textureB = picBTexture[12];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 13){
			textureB = picBTexture[13];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 14){
			textureB = picBTexture[14];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 15){
			textureB = picBTexture[15];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 16){
			textureB = picBTexture[16];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 17){
			textureB = picBTexture[17];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 18){
			textureB = picBTexture[18];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 19){
			textureB = picBTexture[19];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 20){
			textureB = picBTexture[20];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 21){
			textureB = picBTexture[21];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 22){
			textureB = picBTexture[22];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 23){
			textureB = picBTexture[23];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 24){
			textureB = picBTexture[24];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 25){
			textureB = picBTexture[25];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 26){
			textureB = picBTexture[26];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 27){
			textureB = picBTexture[27];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 28){
			textureB = picBTexture[28];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 29){
			textureB = picBTexture[29];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 30){
			textureB = picBTexture[30];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 31){
			textureB = picBTexture[31];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 32){
			textureB = picBTexture[32];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 33){
			textureB = picBTexture[33];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 34){
			textureB = picBTexture[34];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 35){
			textureB = picBTexture[35];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 36){
			textureB = picBTexture[36];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 37){
			textureB = picBTexture[37];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 38){
			textureB = picBTexture[38];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 39){
			textureB = picBTexture[39];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 40){
			textureB = picBTexture[40];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 41){
			textureB = picBTexture[41];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 42){
			textureB = picBTexture[42];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 43){
			textureB = picBTexture[43];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 44){
			textureB = picBTexture[44];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 45){
			textureB = picBTexture[45];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 46){
			textureB = picBTexture[46];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 47){
			textureB = picBTexture[47];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 48){
			textureB = picBTexture[48];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 49){
			textureB = picBTexture[49];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 50){
			textureB = picBTexture[50];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB == 51){
			textureB = picBTexture[51];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picBWidth, picBHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureB);
			glutPostRedisplay();
			frameSetB++;
		}
		else if (frameSetB > 51){
			frameSetB = 0;
		}
	}

	if (frameSwitch == 3){
		// Picture C frame animation
		if (frameSetC == 0){
			textureC = picCTexture[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 1){
			textureC = picCTexture[1];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 2){
			textureC = picCTexture[2];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 3){
			textureC = picCTexture[3];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 4){
			textureC = picCTexture[4];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 5){
			textureC = picCTexture[5];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 6){
			textureC = picCTexture[6];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 7){
			textureC = picCTexture[7];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 8){
			textureC = picCTexture[8];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 9){
			textureC = picCTexture[9];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 10){
			textureC = picCTexture[10];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 11){
			textureC = picCTexture[11];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 12){
			textureC = picCTexture[12];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 13){
			textureC = picCTexture[13];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC == 14){
			textureC = picCTexture[14];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picCWidth, picCHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureC);
			glutPostRedisplay();
			frameSetC++;
		}
		else if (frameSetC > 14){
			frameSetC = 0;
		}
	}
	if (frameSwitch == 4){
		// Picture D frame animation
		if (frameSetD == 0){
			textureD = picATexture[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 1){
			textureD = picDTexture[1];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 2){
			textureD = picDTexture[2];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 3){
			textureD = picDTexture[3];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 4){
			textureD = picDTexture[4];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 5){
			textureD = picDTexture[5];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 6){
			textureD = picDTexture[6];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 7){
			textureD = picDTexture[7];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 8){
			textureD = picDTexture[8];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 9){
			textureD = picDTexture[9];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 10){
			textureD = picDTexture[10];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 11){
			textureD = picDTexture[11];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 12){
			textureD = picDTexture[12];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 13){
			textureD = picDTexture[13];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 14){
			textureD = picDTexture[14];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 15){
			textureD = picDTexture[15];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 16){
			textureD = picDTexture[16];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 17){
			textureD = picDTexture[17];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 18){
			textureD = picDTexture[18];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 19){
			textureD = picDTexture[19];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 20){
			textureD = picDTexture[20];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 21){
			textureD = picDTexture[21];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 22){
			textureD = picDTexture[22];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD == 23){
			textureD = picDTexture[23];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picDWidth, picDHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureD);
			glutPostRedisplay();
			frameSetD++;
		}
		else if (frameSetD > 23){
			frameSetD = 0;
		}
	}

	if (frameSwitch == 5){
		// Picture E frame animation
		if (frameSetE == 0){
			textureE = picETexture[0];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 1){
			textureE = picETexture[1];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 2){
			textureE = picETexture[2];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 3){
			textureE = picETexture[3];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 4){
			textureE = picETexture[4];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 5){
			textureE = picETexture[5];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 6){
			textureE = picETexture[6];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 7){
			textureE = picETexture[7];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 8){
			textureE = picETexture[8];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 9){
			textureE = picETexture[9];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 10){
			textureE = picETexture[10];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 11){
			textureE = picETexture[11];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 12){
			textureE = picETexture[12];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 13){
			textureE = picETexture[13];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 14){
			textureE = picETexture[14];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 15){
			textureE = picETexture[15];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 16){
			textureE = picETexture[16];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 17){
			textureE = picETexture[17];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 18){
			textureE = picETexture[18];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 19){
			textureE = picETexture[19];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 20){
			textureE = picETexture[20];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 21){
			textureE = picETexture[21];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 22){
			textureE = picETexture[22];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 23){
			textureE = picETexture[23];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 24){
			textureE = picETexture[24];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 25){
			textureE = picETexture[25];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 26){
			textureE = picETexture[26];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 27){
			textureE = picETexture[27];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 28){
			textureE = picETexture[28];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 29){
			textureE = picETexture[29];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 30){
			textureE = picETexture[30];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 31){
			textureE = picETexture[31];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 32){
			textureE = picETexture[32];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 33){
			textureE = picETexture[33];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 34){
			textureE = picETexture[34];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 35){
			textureE = picETexture[35];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 36){
			textureE = picETexture[36];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE == 37){
			textureE = picETexture[37];
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, picEWidth, picEHeight,
				0, GL_RGB, GL_UNSIGNED_BYTE, textureE);
			glutPostRedisplay();
			frameSetE++;
		}
		else if (frameSetE > 37){
			frameSetE = 0;
		}
	}

	glutTimerFunc(extra, timer, extra);
	glutPostRedisplay();
}


int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Attempting to be cool");
	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMoved);
	glutSpecialFunc(special);

	//SubMenu code
	subMenuID = glutCreateMenu(lightingSubMenu);
	glutAddMenuEntry("Ambient", 0);
	glutAddMenuEntry("Diffuse", 1);
	glutAddMenuEntry("Specular", 2);
	glutAddMenuEntry("All", 3);

	subMenuID2 = glutCreateMenu(shadingMenu);
	glutAddMenuEntry("Flat shading", 0);
	glutAddMenuEntry("Gouraud Shading", 1);

	subMenuID3 = glutCreateMenu(animationMenu);
	glutAddMenuEntry("Picture A", 1);
	glutAddMenuEntry("Picture B", 2);
	glutAddMenuEntry("Picture C", 3);
	glutAddMenuEntry("Picture D", 4);
	glutAddMenuEntry("Picture E", 5);

	glutCreateMenu(menu);
	glutAddSubMenu("Lighting", subMenuID);
	glutAddSubMenu("Shading style", subMenuID2);
	glutAddSubMenu("Animate Picture", subMenuID3);
	
	// Open menu using right mouse button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutTimerFunc(0, timer, frameDelay);
	glutMainLoop();
}