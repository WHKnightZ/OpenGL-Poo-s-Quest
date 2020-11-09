#include "../Library/loadpng.h"
#include <GL/glut.h>

/* begin define */
#define POSX 200
#define POSY 40
#define MAXX 10
#define MAXY 11
#define MAXTILE 23
#define MAXPATH 3
#define SIZE 40

int WIDTH = MAXX * SIZE;
int HEIGHT = MAXY * SIZE;
float W2 = MAXX * SIZE / 2.0f;
float H2 = MAXY * SIZE / 2.0f;
float SIZEF = (float)SIZE;
/* end define */

typedef struct Image {
    GLubyte* img;
    GLuint w, h;
} Image;

typedef struct Direction {
    float x, y;
} Direction;

Direction DefOffset[] = { { 0, -5 }, { 5, 0 }, { 0, 5 }, { -5, 0 } };
Direction DefOffset2[] = { { 0, -18 }, { 18, 0 }, { 0, 18 }, { -18, 0 } };
Image ImgHero[2][4][6];
Image ImgTile[MAXTILE];
Image ImgPath[MAXPATH];
int NewMap[MAXTILE] = { 1, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2, 2, 2,1,1,1 };

int MapTile[MAXY][MAXX];
int Map[MAXY][MAXX];
int i,j,k,tStt=0;
int IsRun,Drt,Stt;
float l,r,b,t,x,y,xMap,yMap;
float tmpD, tmpD2;
int Loop6[]={1,2,3,4,5,0};
int Key[]={GLUT_KEY_UP,GLUT_KEY_RIGHT,GLUT_KEY_DOWN,GLUT_KEY_LEFT};

void loadTexture(Image* img, const char* path)
{
    loadPng(&img->img, &img->w, &img->h, path);
}

void mapTexture(Image* img)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void importMap()
{
    FILE* f = fopen("Map.txt", "r");
    for (i = 0; i < MAXY; i++) {
        for (j = 0; j < MAXX; j++) {
            fscanf(f, "%d", &MapTile[i][j]);
            Map[i][j] = NewMap[MapTile[i][j]];
        }
    }
    fclose(f);
}

void initGame()
{
	importMap();
    char str[21];
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            for (k = 0; k < 6; k++) {
                sprintf(str, "Images/Hero/%d%d%d.png", i, j, k);
                loadTexture(&ImgHero[i][j][k], str);
            }
        }
    }
    for (i = 0; i < MAXTILE; i++) {
        sprintf(str, "Images/Tile/%02d.png", i);
        loadTexture(&ImgTile[i], str);
    }
    for (i = 0; i < MAXPATH; i++) {
        sprintf(str, "Images/Path/%02d.png", i);
        loadTexture(&ImgPath[i], str);
    }
    x=1*SIZE+SIZE/2;
    y=2*SIZE+SIZE/2;
    l=W2-ImgHero[0][0][0].w/2;
    r=l+ImgHero[0][0][0].w;
    b=H2-ImgHero[0][0][0].h/2;
    t=b+ImgHero[0][0][0].h;
    xMap=W2-x;
    yMap=H2-y+10.0f;
    IsRun=0;
    Drt=2;
    Stt=0;
}

void initGL()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.184f, 0.173f, 0.251f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    initGame();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xMap,yMap,0.0f);
    tmpD2 = tmpD = 0.0f;
    for (i = 0; i < MAXY; i++) {
        for (j = 0; j < MAXX; j++) {
        	if (MapTile[i][j]>-1){
        		mapTexture(&ImgTile[MapTile[i][j]]);
	            glBegin(GL_POLYGON);
	            glTexCoord2f(0.0f, 0.0f);
	            glVertex2f(tmpD2, tmpD);
	            glTexCoord2f(1.0f, 0.0f);
	            glVertex2f(tmpD2 + SIZEF, tmpD);
	            glTexCoord2f(1.0f, 1.0f);
	            glVertex2f(tmpD2 + SIZEF, tmpD + SIZEF);
	            glTexCoord2f(0.0f, 1.0f);
	            glVertex2f(tmpD2, tmpD + SIZEF);
	            glEnd();
			}
            tmpD2 += SIZEF;
        }
        tmpD2 = 0.0f;
        tmpD += SIZEF;
    }
    glLoadIdentity();
    mapTexture(&ImgHero[IsRun][Drt][Stt]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(l, b);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(r, b);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(r, t);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(l, t);
    glEnd();
    glutSwapBuffers();
}

void resize(int x, int y)
{
    glutPositionWindow(POSX, POSY);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void timer(int value)
{
	if (IsRun){
		if (Map[(int)(y+DefOffset2[Drt].y)/SIZE][(int)(x+DefOffset2[Drt].x)/SIZE]>0){
			x+=DefOffset[Drt].x;
			y+=DefOffset[Drt].y;
			xMap-=DefOffset[Drt].x;
			yMap-=DefOffset[Drt].y;
			printf("%d %d\n",(int)x/SIZE,(int)y/SIZE);
		}
	}
	tStt++;
	if (tStt==4){
		tStt=0;
		Stt=Loop6[Stt];
	}
    glutPostRedisplay();
    glutTimerFunc(25, timer, 0);
}

void special(int key, int x, int y){
	switch (key){
		case GLUT_KEY_UP: Drt=0; IsRun=1; break;
		case GLUT_KEY_RIGHT: Drt=1; IsRun=1; break;
		case GLUT_KEY_DOWN: Drt=2; IsRun=1; break;
		case GLUT_KEY_LEFT: Drt=3; IsRun=1; break;
	}
}

void specialUp(int key, int x, int y){
	if (key==Key[Drt]) IsRun=0;
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POSX, POSY);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Quest");
    initGL();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIgnoreKeyRepeat(GL_TRUE);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutTimerFunc(0,timer,0);
    glutMainLoop();
    return 0;
}
