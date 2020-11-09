#include "../../Library/loadpng.h"
#include <GL/glut.h>

/* begin define */
#define POSX 40
#define POSY 40
#define WIDTH 1280
#define HEIGHT 680
#define MAXX 22
#define MAXY 16
#define STARTX 20.0f
#define STARTY 20.0f
#define STARTXTILE 960.0f
#define STARTYTILE 40.0f
#define MAXTILE 39
#define SIZE 40
#define TILEPERROW 6

float SIZEF = (float)SIZE;
float SIZE2 = SIZE + 8;
float FULLX = STARTX + MAXX * SIZE;
float FULLY = STARTY + MAXY * SIZE;
int STARTXTILEI = (int)STARTXTILE;
int STARTYTILEI = (int)STARTYTILE;
int FULLXTILEI = (int)STARTXTILE + TILEPERROW * (SIZE + 8) - 8;
int FULLYTILEI = (int)STARTYTILE + (MAXTILE / TILEPERROW + 1) * (SIZE + 8) - 8;

/* end define */

int MaxX = 0, MaxY = 0, a, b, c;
int *ptr;
int MapTile[MAXY][MAXX];
int i, j, k, check;
int Choice = -1;

float lC, rC, bC, tC;

struct Image {
    GLubyte *img;
    GLuint w, h;
};

Image ImgTile[MAXTILE];
Image ImgChoice;

void loadTexture(Image *img, const char *path) {
    loadPng(&img->img, &img->w, &img->h, path);
}

void mapTexture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

void InitGame() {
    char str[21];
    for (i = 0; i < MAXTILE; i++) {
        sprintf(str, "Images/Tile/%02d.png", i);
        loadTexture(&ImgTile[i], str);
    }
    loadTexture(&ImgChoice, "Images/Choice.png");
    for (i = 0; i < MAXY; i++) {
        for (j = 0; j < MAXX; j++)
            MapTile[i][j] = -1;
    }
    FILE *f = fopen("Map.txt", "r");
    fscanf(f, "%d%d", &MaxX, &MaxY);
    for (i = 0; i < MaxY; i++)
        for (j = 0; j < MaxX; j++)
            fscanf(f, "%d", &MapTile[i][j]);
    fclose(f);
    glNewList(1, GL_COMPILE);
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
    for (i = 0; i <= MAXY; i++) {
        glVertex2f(STARTX, STARTY + i * SIZE);
        glVertex2f(FULLX, STARTY + i * SIZE);
    }
    for (i = 0; i <= MAXX; i++) {
        glVertex2f(STARTX + i * SIZE, STARTY);
        glVertex2f(STARTX + i * SIZE, FULLY);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    int tmp = STARTXTILE, tmp2 = STARTYTILE;
    for (i = 0; i < MAXTILE; i++) {
        mapTexture(&ImgTile[i]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(tmp, tmp2);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(tmp + SIZEF, tmp2);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(tmp + SIZEF, tmp2 + SIZEF);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(tmp, tmp2 + SIZEF);
        glEnd();
        if ((i + 1) % 6 == 0) {
            tmp = STARTXTILE;
            tmp2 += SIZEF + 8.0f;
        } else
            tmp += SIZEF + 8.0f;
    }
    glEndList();
}

void InitGL() {
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
    InitGame();
}

void SaveMap() {
    FILE *f = fopen("SaveMap.txt", "w");
    fprintf(f, "%d %d\n", MaxX, MaxY);
    for (i = 0; i < MaxY; i++) {
        for (j = 0; j < MaxX - 1; j++)
            fprintf(f, "%02d ", MapTile[i][j]);
        fprintf(f, "%02d\n", MapTile[i][MaxX - 1]);
    }
    fclose(f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glCallList(1);
    if (Choice > -1) {
        mapTexture(&ImgChoice);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(lC, bC);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(rC, bC);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(rC, tC);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(lC, tC);
        glEnd();
    }
    for (i = 0; i < MaxY; i++) {
        for (j = 0; j < MaxX; j++) {
            if (MapTile[i][j] > -1) {
                mapTexture(&ImgTile[MapTile[i][j]]);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(j * SIZE + STARTX, i * SIZE + STARTY);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f((j + 1) * SIZE + STARTX, i * SIZE + STARTY);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f((j + 1) * SIZE + STARTX, (i + 1) * SIZE + STARTY);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(j * SIZE + STARTX, (i + 1) * SIZE + STARTY);
                glEnd();
            }
        }
    }
    glutSwapBuffers();
}

void resize(int x, int y) {
    glutPositionWindow(POSX, POSY);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void special(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_RIGHT:
        break;
    case GLUT_KEY_DOWN:
        break;
    case GLUT_KEY_LEFT:
        break;
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 13)
        SaveMap();
}

void mouse(int button, int status, int x, int y) {
    if (button == GLUT_LEFT) {
        if (status == GLUT_DOWN) {
            if (x >= STARTXTILEI && x <= FULLXTILEI && y >= STARTYTILEI && y <= FULLYTILEI) {
                a = (x - STARTXTILEI) / SIZE2;
                b = (y - STARTYTILEI) / SIZE2;
                c = b * TILEPERROW + a;
                if (c < MAXTILE && Choice != c) {
                    Choice = c;
                    lC = a * SIZE2 + STARTXTILE;
                    rC = lC + SIZE;
                    bC = b * SIZE2 + STARTYTILE;
                    tC = bC + SIZE;
                    glutPostRedisplay();
                }
            } else if (x >= STARTX && x <= FULLX && y >= STARTY && y <= FULLY) {
                a = (x - STARTX) / SIZE;
                b = (y - STARTY) / SIZE;
                MapTile[b][a] = Choice;
                if (Choice > -1) {
                    if (a >= MaxX)
                        MaxX = a + 1;
                    if (b >= MaxY)
                        MaxY = b + 1;
                } else {
                    if (a == MaxX - 1) {
                        check = 1;
                        for (i = 0; i < MaxY; i++)
                            if (MapTile[i][a] > -1)
                                check = 0;
                        MaxX -= check;
                    }
                    if (b == MaxY - 1) {
                        check = 1;
                        for (i = 0; i < MaxX; i++)
                            if (MapTile[b][i] > -1)
                                check = 0;
                        MaxY -= check;
                    }
                }
                glutPostRedisplay();
            }
        }
    } else {
        if (Choice != -1) {
            Choice = -1;
            glutPostRedisplay();
        }
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POSX, POSY);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Draw");
    InitGL();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    //glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMainLoop();
    return 0;
}
