#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#include "imageloader.h"
#include "glm.h"
#include <iostream>
//using namespace std;
#define PI 3.14

double eye[] = { 0, 0, 30 };
double center[] = { 0, 0, 0 };
double up[] = { 0.0, 1.0, 0.0 };


const char* modelname = "data/tableBasse2.obj";
GLMmodel* pmodel1;
const char* modelname2 = "data/mug.obj";
GLMmodel* pmodel2;
const char* modelname3 = "data/clicClac.obj";
GLMmodel* pmodel3;


static int lFoot = 0, rFoot = 0, Rshoulderx = 0, Rshouldery = 0, Rshoulderz = 0, Relbow = 0, Lshoulderx = 0, Lshouldery = 0, Lshoulderz = 0, Lelbow = 0, fingerBase1 = 0, fingerUp1 = 0, fingerBase2 = 0, fingerUp2 = 0;
static int  fingerBase3 = 0, fingerUp3 = 0, fingerBase4 = 0, fingerUp4 = 0, fingerBase5 = 0, fingerUp5 = 0, body = 180;
static int rightLegHipy = 0, rightLegHipz = -90, leftLegHipy = 0, leftLegHipz = -90, rightLegKnee = 90, leftLegKnee = 90;
int moving, startx, starty;

GLfloat angle = 0.0;   // in degrees 
GLfloat angle2 = 0.0;   // in degrees 

GLuint _textureId; //The id of the texture

double direction[3];
double speed = 0.1;

// RGBA
GLfloat light_ambient1[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 0.5, 5.0, 0.0, 0.0 };
GLfloat lightPos1[] = { -0.5, -5.0, -2.0, 0.0 };
//
//// Material Properties
//GLfloat mat_amb_diff[] = { 0.643, 0.753, 0.934, 1.0 };
//GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
//GLfloat shininess[] = { 100.0 };


// Body Positions
static float xBody = -8.6;
static float yBody = -1.5;
static float zBody = -1.5;

//table position
static float xtable = -4.0;
static float ytable = -4.0;
static float ztable = -12.0;


//mug position
static float xmug = 10.0;
static float ymug = -1;
static float zmug = -2.0;

//sofa position
static float xsofa = 5.0;
static float ysofa = -3.5;
static float zsofa = -10.0;
//jumping
int jumpPoses[3][6] = { {-45,45,-45,45,30,0},{0,0,0,0,-120,0},{0,0,0,0,-120,0} };
static int jumping = 0;
int frame2 = 0;
// walking 
int walkposes[8][6] = { {-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30} };
static int walking = 0;
int frame1 = 0;
// mug movement
int MugPoses[6][6] = { {-30,0,-2.0,-1.0,-90,0},{-45, 0 , -2.0 , -1.0 ,-90,0},{-50,20,-2.0 , -1.0,-100,0},{-30,30,0,0,-130,0} , { -45, 30, 1, 0, -90, 0 }, { -30,-30,1,0,-90,0 } };
int frame = 0;

static int mugx = 0;
static int mugy = -90;
void drawmodel(char* modelname, GLMmodel* pmodel)
{
    GLMmodel* model = glmReadOBJ(modelname);
    glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 90.0);
    glmScale(model, .15);
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId);			 //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
        0,							  //0 for now
        GL_RGB,					  //Format OpenGL uses for image
        image->width, image->height, //Width and height
        0,							  //The border of the image
        GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
        GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
                                     //as unsigned numbers
        image->pixels);			  //The actual pixel data
    return textureId;	//Returns the id of the texture
}
void initRendering(const char* bmbImg, GLuint& textureID)
{
    Image* image = loadBMP(bmbImg);
    _textureId = loadTexture(image);
    delete image;
    // Turn on the power
    glEnable(GL_LIGHTING);
    // Flip light switch
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    // assign light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    // Material Properties
    GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
    glEnable(GL_NORMALIZE);
    //Enable smooth shading
    //glShadeModel(GL_SMOOTH);
    // Enable Depth buffer
    glEnable(GL_DEPTH_TEST);
}
void crossProduct(double a[], double b[], double c[])
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
    double norm;
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
    norm = sqrt(norm);
    a[0] /= norm;
    a[1] /= norm;
    a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

    double temp[3];
    temp[0] = p[0];
    temp[1] = p[1];
    temp[2] = p[2];

    temp[0] = -a[2] * p[1] + a[1] * p[2];
    temp[1] = a[2] * p[0] - a[0] * p[2];
    temp[2] = -a[1] * p[0] + a[0] * p[1];

    temp[0] *= sin(theta);
    temp[1] *= sin(theta);
    temp[2] *= sin(theta);

    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

    temp[0] += cos(theta) * p[0];
    temp[1] += cos(theta) * p[1];
    temp[2] += cos(theta) * p[2];

    p[0] = temp[0];
    p[1] = temp[1];
    p[2] = temp[2];
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60.0, 1.0, 1.0, 20.0);
    //glMatrixMode(GL_MODELVIEW);
}

void reset()
{
    double e[] = { 0.0, 0.0, 2 };
    double c[] = { 0.0, 0.0, 1.0 };
    double u[] = { 0.0, 1.0, 0.0 };
    for (int i = 0; i < 3; i++)
    {
        eye[i] = e[i];
        center[i] = c[i];
        up[i] = u[i];
    }
}
void setPoses(int frame) {
    if (zmug != -40 && xBody >= 7) {
        Lshoulderz = MugPoses[frame][0];
        Lelbow = MugPoses[frame][0];
        Lshouldery = MugPoses[frame][1];
        //Lelbow = -MugPoses[frame][1];
        zmug = MugPoses[frame][2];
        ymug = MugPoses[frame][3];
        mugy = MugPoses[frame][4];
        mugx = MugPoses[frame][5];
    }
    else {
       // Lshoulderz = 0;
       //Lelbow = 0;
       //Lshouldery = -30;
    }
}
void timer(int value)
{
    frame = frame % 10;
    setPoses(frame);
    frame++;
    if (frame <=6) {
        glutPostRedisplay();
        glutTimerFunc(600, timer, 0);
    }
}
void setPoses1(int frame) {
    if (xBody <= -8.6) {
        rightLegHipz = 0;
        rightLegKnee = 0;
        leftLegHipz = 0;
        leftLegKnee = 0;
        xBody = -6.5;
        yBody = -0.2;
        Rshoulderz = 0;
        Rshouldery = 0;
        Lshoulderz = 0;
        Lshouldery = 0;
        frame = 0;
    }
    else if (xBody >= -8 && xBody <= 7) {
        rightLegHipz = walkposes[frame][0];
        rightLegKnee = walkposes[frame][1];
        leftLegHipz = walkposes[frame][2];
        leftLegKnee = walkposes[frame][3];
        Rshoulderz = walkposes[frame][4];
        Rshouldery = walkposes[frame][5];
        Lshoulderz = walkposes[frame][4];
        Lshouldery = walkposes[frame][5];
        xBody += 2;
    }
    else {
        xBody ==  6;
        rightLegHipz = 0;
        rightLegKnee = 0;
        leftLegHipz = 0;
        leftLegKnee = 0;
        Rshoulderz = 0;
        Rshouldery = 0;
        Lshoulderz = 0;
        Lshouldery = 0;
    }

}
void timer2(int value)
{
    frame1 = frame1 % 10;
    setPoses1(frame1);
    frame1++;
    if (frame1 <= 9) {
        glutPostRedisplay();
        glutTimerFunc(300, timer2, 0);
    }
}
bool jumpFrame = true;
void setPoses2(int frame) {
    if (xBody >= 7 && yBody <= 3) {
        rightLegHipz = jumpPoses[frame][0];
        rightLegKnee = jumpPoses[frame][1];
        leftLegHipz = jumpPoses[frame][2];
        leftLegKnee = jumpPoses[frame][3];
        Rshoulderz = jumpPoses[frame][4];
        Rshouldery = jumpPoses[frame][5];
        Lshoulderz = jumpPoses[frame][4];
        Lshouldery = jumpPoses[frame][5];
        yBody += 1;
    }
    else {
        //xBody == 6;
        jumpFrame = false;
        yBody = 0;
        rightLegHipz = 0;
        rightLegKnee = 0;
        leftLegHipz = 0;
        leftLegKnee = 0;
        Rshoulderz = 0;
        Rshouldery = 0;
        Lshoulderz = 0;
        Lshouldery = 0;
    }

}
void timer3(int value)
{
    frame2 = frame2 % 10;

    if (jumpFrame) {
        setPoses2(frame2);
        frame2++;
        glutPostRedisplay();
        glutTimerFunc(300, timer3, 0);
    }
}


void display(void)

{
    glClearColor(1.0, 1.0, 1.0, 1.0);


    // Clear Depth and Color buffers

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    //texture
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-20, -6, 30);
    glTexCoord2f(3.0f, 0.0f); glVertex3f(20, -6, 30);
    glTexCoord2f(3.0f, 3.0f); glVertex3f(20, -6, -70);
    glTexCoord2f(0.0f, 3.0f); glVertex3f(-20, -6, -70);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    //placing object
    //table
    glPushMatrix();
    glRotatef(360, 0.0, 1.0, 0.0);
    glRotatef(-90, 0.0, 1.0, 0.0);

    glTranslatef(xtable, ytable, ztable);
    glScalef(40.0, 40.0, 40.0);
    glColor3b(5, 5, 5);
    drawmodel((char*)modelname, pmodel1);
    glPopMatrix();

    //mug
    glPushMatrix();
    glTranslatef(xmug, ymug, zmug);
    glTranslatef(0, 0, 0);
    glScalef(6.0, 6.0, 6.0);
    glColor3b(5, 5, 5);
    drawmodel((char*)modelname2, pmodel2);
    glPopMatrix();

    //sofa
    glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(xsofa, ysofa, zsofa);
    glScalef(40.0, 40.0, 40.0);
    glColor3b(5, 5, 5);
    drawmodel((char*)modelname3, pmodel3);
    glPopMatrix();

    //robot
    glPushMatrix();
    glTranslatef(xBody, yBody, zBody);
    glRotatef((GLfloat)body, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    glPushMatrix();
    glScalef(1.0, 5.0, 3.0);
    glutWireCube(1.0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 3.5, 0.0);
    glutSolidSphere(0.8, 10, 10);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 1.9, 2.3);
    glRotatef((GLfloat)Rshoulderx, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Rshouldery, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)Rshoulderz, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.4, 0);
    /*glTranslatef(0, -0.6, 2.3);*/
    glPushMatrix();
    glScalef(0.6, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)Relbow, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0.0);
    glPushMatrix();
    glScalef(0.6, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    //Draw finger flang 1 base
    glTranslatef(-0.2, -1.1, -0.4);
    glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5 base
    glTranslatef(0.4, 0.0, -0.8);
    glRotatef((GLfloat)fingerBase5, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 1 
    glTranslatef(0.0, -0.3, 0.0);
    glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 
    glTranslatef(-0.4, 0.0, 0.8);
    glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp5, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 1.9, -2.3);
    glRotatef((GLfloat)Lshoulderx, 1.0, 0.0, 0.0);
    glRotatef((GLfloat)Lshouldery, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)Lshoulderz, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.4, 0);
    glPushMatrix();
    glScalef(0.6, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)Lelbow, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0.0);
    glPushMatrix();
    glScalef(0.6, 2, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    //Draw finger flang 1 base
    glTranslatef(-0.2, -1.1, -0.4);
    glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4 base
    glTranslatef(0.0, 0.0, 0.25);
    glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5 base
    glTranslatef(0.4, 0.0, -0.8);
    glRotatef((GLfloat)fingerBase5, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 1 
    glTranslatef(0.0, -0.3, 0.0);
    glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 2 
    glTranslatef(-0.4, 0.0, 0.8);
    glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 3 
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 4
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    //Draw finger flang 5
    glTranslatef(0.0, 0.0, -0.25);
    glRotatef((GLfloat)fingerUp5, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(0.1, 0.3, 0.1);
    glutWireCube(1);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -3.5, -0.75);
    glTranslatef(0.0, 1.0, 0);
    glRotatef((GLfloat)leftLegHipy, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)leftLegHipz, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0);
    glPushMatrix();
    glScalef(0.8, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)leftLegKnee, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0.0);
    glPushMatrix();
    glScalef(0.8, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    // foot
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)lFoot, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(1.8, 0.3, 1);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0, -3.5, 0.75);
    glTranslatef(0.0, 1.0, 0);
    glRotatef((GLfloat)rightLegHipy, 0.0, 1.0, 0.0);
    glRotatef((GLfloat)rightLegHipz, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0);
    glPushMatrix();
    glScalef(0.8, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)rightLegKnee, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -1.0, 0.0);
    glPushMatrix();
    glScalef(0.8, 2.0, 1.0);
    glutWireCube(1.0);
    glPopMatrix();
    // foot
    glTranslatef(0.0, -1.0, 0.0);
    glRotatef((GLfloat)rFoot, 0.0, 0.0, 1.0);
    glPushMatrix();
    glScalef(1.8, 0.3, 1);
    glutSolidCube(1.0);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    glutSwapBuffers();
}
void rotateCameraRight() // cam
{
    rotatePoint(up, PI / 20, eye);
}

void rotateCameraLeft() // cam
{
    rotatePoint(up, -PI / 20, eye);
}

// Rotation about horizontal direction

void rotateCameraUp() // cam
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, PI / 20, eye);
    rotatePoint(horizontal, PI / 20, up);
}

void rotateCameraDown() // cam
{
    double horizontal[3];
    double look[] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };
    crossProduct(up, look, horizontal);
    normalize(horizontal);
    rotatePoint(horizontal, -PI / 20, eye);
    rotatePoint(horizontal, -PI / 20, up);
}
// Forward and Backward
void zoomIn() // cam
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    double speed = 0.1;
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}

void zoomOut() // cam
{
    double direction[3];
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];
    double speed = 0.1;
    eye[0] -= direction[0] * speed;
    eye[1] -= direction[1] * speed;
    eye[2] -= direction[2] * speed;

    center[0] -= direction[0] * speed;
    center[1] -= direction[1] * speed;
    center[2] -= direction[2] * speed;

}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
}
void cameraKeys(int key, int x, int y) // cam
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotateCameraLeft();
        break;
    case GLUT_KEY_RIGHT:
        rotateCameraRight();
        break;
    case GLUT_KEY_UP:
        rotateCameraUp();
        break;
    case GLUT_KEY_DOWN:
        rotateCameraDown();
        break;
    }
    glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {

    case '0': // cam
        reset();
        glutPostRedisplay();
        break;

    case '+': // cam
        zoomIn();
        glutPostRedisplay();
        break;
    case '-': // cam
        zoomOut();
        glutPostRedisplay();
        break;
    case 'm':
        timer2(0);
        glutPostRedisplay();
        break;
    case 'b':
        timer3(0);
        glutPostRedisplay();
        break;
    case 'n':
        timer(0);
        glutPostRedisplay();
        break;

        //right arm movement
    case 'a':
        if (Rshoulderx < 90) {
            Rshoulderx = (Rshoulderx + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'A':
        if (Rshoulderx > -90) {
            Rshoulderx = (Rshoulderx - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 's':
        if (Rshouldery <= 90)
            Rshouldery = (Rshouldery + 5) % 360;
        glutPostRedisplay();
        break;
    case 'S':
        if (Rshouldery >= -90)
            Rshouldery = (Rshouldery - 5) % 360;
        glutPostRedisplay();
        break;
    case 'd':
        if (Rshoulderz < 180)
            Rshoulderz = (Rshoulderz + 5) % 360;
        glutPostRedisplay();
        break;
    case 'D':
        if (Rshoulderz >= -20)
            Rshoulderz = (Rshoulderz - 5) % 360;
        glutPostRedisplay();
        break;

        // right elbow movement
    case 'f':
        if (Relbow <= 160)
            Relbow = (Relbow + 5) % 360;
        glutPostRedisplay();
        break;
    case 'F':
        if (Relbow > 0)
            Relbow = (Relbow - 5) % 360;
        glutPostRedisplay();
        break;

        //Left arm movement
    case 'q':
        if (Lshoulderx < 90) {
            Lshoulderx = (Lshoulderx + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'Q':
        if (Lshoulderx > -90) {
            Lshoulderx = (Lshoulderx - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'w':
        if (Lshouldery <= 90)
            Lshouldery = (Lshouldery + 5) % 360;
        glutPostRedisplay();
        break;
    case 'W':
        if (Lshouldery >= -90)
            Lshouldery = (Lshouldery - 5) % 360;
        glutPostRedisplay();
        break;
    case 'e':
        if (Lshoulderz < 180)
            Lshoulderz = (Lshoulderz + 5) % 360;
        glutPostRedisplay();
        break;
    case 'E':
        if (Lshoulderz >= -20)
            Lshoulderz = (Lshoulderz - 5) % 360;
        glutPostRedisplay();
        break;

        //left elbow movement
    case 'r':
        if (Lelbow <= 160)
            Lelbow = (Lelbow + 5) % 360;
        glutPostRedisplay();
        break;
    case 'R':
        if (Lelbow > 0)
            Lelbow = (Lelbow - 5) % 360;
        glutPostRedisplay();
        break;

        // Right leg movement
    case 'g':
        if (rightLegHipy < 80) {
            rightLegHipy = (rightLegHipy + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'G':
        if (rightLegHipy > 0) {
            rightLegHipy = (rightLegHipy - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'h':
        if (rightLegHipz < 90) {
            rightLegHipz = (rightLegHipz + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'H':
        if (rightLegHipz > -30) {
            rightLegHipz = (rightLegHipz - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'j':
        if (rightLegKnee > -90) {
            rightLegKnee = (rightLegKnee - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'J':
        if (rightLegKnee < 0) {
            rightLegKnee = (rightLegKnee + 5) % 360;
        }
        glutPostRedisplay();
        break;

        // Left leg movement
    case 't':
        if (leftLegHipy > -80) {
            leftLegHipy = (leftLegHipy - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'T':
        if (leftLegHipy < 0) {
            leftLegHipy = (leftLegHipy + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'y':
        if (leftLegHipz < 90) {
            leftLegHipz = (leftLegHipz + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'Y':
        if (leftLegHipz > -30) {
            leftLegHipz = (leftLegHipz - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'u':
        if (leftLegKnee > -90) {
            leftLegKnee = (leftLegKnee - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'U':
        if (leftLegKnee < 0) {
            leftLegKnee = (leftLegKnee + 5) % 360;
        }
        glutPostRedisplay();
        break;


        //finger 1 movement
    case 'z':
        if (fingerBase1 < 80) {
            fingerBase1 = (fingerBase1 + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'Z':
        if (fingerBase1 != 0) {
            fingerBase1 = (fingerBase1 - 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'x':
        if (fingerUp1 < 80) {
            fingerUp1 = (fingerUp1 + 5) % 360;
        }
        glutPostRedisplay();
        break;
    case 'X':
        if (fingerUp1 != 0) {
            fingerUp1 = (fingerUp1 - 5) % 360;
        }
        glutPostRedisplay();
        break;

    case 27:
        exit(0);
        break;
    default:
        break;
    }
}
static void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            moving = 1;
            startx = x;
            starty = y;
        }
        if (state == GLUT_UP) {
            moving = 0;
        }
    }
}



static void motion(int x, int y)
{
    if (moving) {
        angle = angle + (x - startx);
        angle2 = angle2 + (y - starty);
        startx = x;
        starty = y;
        glutPostRedisplay();
    }
}
void Choose_texture(int id)
{
    /*
     * Description:
     *     This function is responsible for changing the texture of the floor
     *     using a drop menu by the right click on the mouse.
     */
    switch (id)
    {
    case 1:
        initRendering("images/f1.bmp", _textureId);
        break;
    case 2:
        initRendering("images/f2.bmp", _textureId);
        break;
    case 3:
        initRendering("images/f3.bmp", _textureId);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Task3");
    //init();
    initRendering("images/wood.bmp", _textureId);
    glutCreateMenu(Choose_texture);
    glutAddMenuEntry("Floor 1", 1);
    glutAddMenuEntry("Floor 2", 2);
    glutAddMenuEntry("Floor 3", 3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1.0, 0.1, 150);
    //glutMouseFunc(mouse);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    // glutDisplayFunc(display);
     //glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(cameraKeys);
    glutDisplayFunc(display);
    /* glutMotionFunc(motion);  */
    glutMainLoop();
    return 0;
}