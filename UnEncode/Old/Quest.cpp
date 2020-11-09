#include "../Library/loadpng.h"
#include <GL/glut.h>

/* begin define */
#define POSX 200
#define POSY 40
#define WIDTH 480
#define HEIGHT 600
#define MAXX 20
#define MAXY 20
#define MAXTILE 36
#define MAXPATH 3
#define SIZE 40
#define MAXENEMY 12
#define MAXGOLD 12
#define MAXTYPE 4
#define MAXDRT 4
#define MAXSTT 8

float W2 = WIDTH / 2.0f;
float H2 = HEIGHT / 2.0f;
float SIZEF = (float)SIZE;
/* end define */

int MaxX, MaxY;
int *ptr;
int NewMap[MAXTILE] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int MapTile[MAXY][MAXX];
int Map[MAXY][MAXX];
int i, j, k, tStt = 0;
char IsRun, Drt, Stt;
float l, r, b, t, xHero, yHero, xMap, yMap, lE, rE, bE, tE;
int tmpInt, tmpI1, tmpI2;
int DefX, DefY, DefDrt, CountDownRespawn, DestX, DestY, PickedGold;
float tmpD, tmpD2;
char Loop6[] = {1, 2, 3, 4, 5, 0};
int Key[] = {GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_LEFT};
int MaxDrtEnemy[MAXTYPE] = {4, 4, 1, 1};
int MaxSttEnemy[MAXTYPE] = {8, 4, 6, 3};
int Time40 = 0;
int Time3 = 0;
int Loop3[] = {1, 2, 0};
int IsAlive;

float oMove = 5.0f, oCanMove = 18.0f, oCanMove2 = 14.0f;

void (*funcTimer)(int);
void timer(int);
void tNull(int);
void (*funcMove[4])();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void Lose();

struct Image
{
    GLubyte *img;
    GLuint w, h;
};

struct DrtInt
{
    char x, y;
};

struct Speed
{
    float x, y;
};

Image ImgHero[4][6];
Image ImgExplode[6];
Image ImgEnemy[MAXTYPE][MAXDRT][MAXSTT];
Image ImgTile[MAXTILE];
Image ImgPath[MAXPATH];
DrtInt DefOffset[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
Speed SpeedOffset[][4] = {
    {{0.0f, -4.0f}, {4.0f, 0.0f}, {0.0f, 4.0f}, {-4.0f, 0.0f}},
    {{0.0f, -5.0f}, {5.0f, 0.0f}, {0.0f, 5.0f}, {-5.0f, 0.0f}}};
int LoopSpeed[][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, {1, 2, 3, 4, 5, 6, 7, 0}};
int TimeSpeed[] = {0, 0};

void loadTexture(Image *img, const char *path)
{
    loadPng(&img->img, &img->w, &img->h, path);
}

void mapTexture(Image *img)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

class Enemy
{
public:
    static int Count;
    int Move, Drt, Stt;
    float l, r, b, t;
    Enemy() { Count++; }
    virtual void CheckLose()
    {
        if (l < xHero && xHero < r && b < yHero && yHero < t)
        {
            Lose();
        }
    }
    virtual void Action() {}
    virtual void Draw() {}
};
int Enemy::Count = 0;
Enemy *enemy[MAXENEMY];

class Enemy0 : public Enemy
{
public:
    static Image Img[4][8];
    static int Loop[];
    static float OffsetX, OffsetY;
    static int Switch[];
    static float w, h;
    int MaxMove, Speed;
    Enemy0(int x, int y, int Drt, int Move, int MaxMove)
    {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        Stt = 0;
        this->Drt = Drt;
        this->Move = Move;
        this->MaxMove = MaxMove;
        this->Speed = 0;
    }
    void Action()
    {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0)
        {
            Move++;
            if (Move == MaxMove)
            {
                Move = 0;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw()
    {
        mapTexture(&Img[Drt][Stt]);
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
    }
};
Image Enemy0::Img[4][8];
int Enemy0::Loop[8] = {1, 2, 3, 4, 5, 6, 7, 0};
float Enemy0::OffsetX = 0.0f;
float Enemy0::OffsetY = -2.0f;
int Enemy0::Switch[4] = {2, 3, 0, 1};
float Enemy0::w;
float Enemy0::h;

class Enemy1 : public Enemy
{
public:
    static Image Img[4][6];
    static int Loop[];
    static float OffsetX, OffsetY;
    static int Switch[];
    static float w, h;
    int MaxMove[2];
    int Drt2, Speed;
    Enemy1(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2)
    {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        Stt = 0;
        this->Drt = Drt;
        if (Drt % 2)
            Drt2 = 0;
        else
            Drt2 = 1;
        this->Move = Move;
        this->MaxMove[0] = MaxMove;
        this->MaxMove[1] = MaxMove2;
        this->Speed = 0;
    }
    void Action()
    {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0)
        {
            Move++;
            if (Move == MaxMove[Drt2])
            {
                Move = 0;
                Drt2 = 1 - Drt2;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw()
    {
        mapTexture(&Img[Drt][Stt]);
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
    }
};
Image Enemy1::Img[4][6];
int Enemy1::Loop[6] = {1, 2, 3, 4, 5, 0};
float Enemy1::OffsetX = -2.0f;
float Enemy1::OffsetY = -1.0f;
int Enemy1::Switch[4] = {1, 2, 3, 0};
float Enemy1::w;
float Enemy1::h;

class Enemy2 : public Enemy
{
public:
    static Image Img[10];
    static int Loop[];
    static float OffsetX, OffsetY;
    static int Switch[];
    static float w, h;
    int MaxMove[2];
    int Drt2, Speed;
    Enemy2(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2)
    {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        Stt = 0;
        this->Drt = Drt;
        if (Drt % 2)
            Drt2 = 0;
        else
            Drt2 = 1;
        this->Move = Move;
        this->MaxMove[0] = MaxMove;
        this->MaxMove[1] = MaxMove2;
        this->Speed = 1;
    }
    void Action()
    {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0)
        {
            Move++;
            if (Move == MaxMove[Drt2])
            {
                Move = 0;
                Drt2 = 1 - Drt2;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw()
    {
        mapTexture(&Img[Stt]);
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
    }
};
Image Enemy2::Img[10];
int Enemy2::Loop[10] = {1, 2, 3, 4, 5, 6, 7, 0, 9, 0};
float Enemy2::OffsetX = 1.0f;
float Enemy2::OffsetY = -8.0f;
int Enemy2::Switch[4] = {3, 0, 1, 2};
float Enemy2::w;
float Enemy2::h;

class Enemy3 : public Enemy
{
public:
    static Image Img[3];
    static int Loop[2][3];
    static float OffsetX, OffsetY;
    static float w, h;
    int MaxMove[2];
    int IsChange;
    Enemy3(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2)
    {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        this->Drt = Drt;
        if (Drt == 0)
            Stt = 0;
        else
            Stt = 2;
        this->Move = Move;
        this->MaxMove[0] = MaxMove;
        this->MaxMove[1] = MaxMove2;
        this->IsChange = 0;
    }
    void Action()
    {
        if (IsChange)
        {
            if (Time3 == 0)
            {
                if (Stt == 1)
                {
                    IsChange = 0;
                    Stt = Loop[Drt][Stt];
                    Drt = 1 - Drt;
                }
                else
                    Stt = Loop[Drt][Stt];
            }
        }
        else
        {
            Move++;
            if (Move == MaxMove[Drt])
            {
                Move = 0;
                IsChange = 1;
            }
        }
    }
    void Draw()
    {
        mapTexture(&Img[Stt]);
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
    }
    void CheckLose()
    {
        if (Drt == 1 && l < xHero && xHero < r && b < yHero && yHero < t)
        {
            Lose();
        }
    }
};
Image Enemy3::Img[3];
int Enemy3::Loop[2][3] = {{1, 2, 2}, {0, 0, 1}};
float Enemy3::OffsetX = 6.0f;
float Enemy3::OffsetY = 0.0f;
float Enemy3::w;
float Enemy3::h;

class Gold
{
public:
    static Image Img[8];
    static int Loop[8], CurrentStt, Count;
    static float OffsetX, OffsetY;
    static float w, h;
    float l, r, b, t;
    int Stt, IsAlive;
    Gold(int x, int y)
    {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        this->Stt = CurrentStt;
        this->IsAlive = 1;
        CurrentStt = Loop[CurrentStt];
        Count++;
    }
    void Action()
    {
        if (IsAlive)
        {
            if (Time3 == 0)
                Stt = Loop[Stt];
            Collide();
        }
    }
    void Draw()
    {
        if (IsAlive)
        {
            mapTexture(&Img[Stt]);
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
        }
    }
    void Collide()
    {
        if (IsAlive && l < xHero && xHero < r && b < yHero && yHero < t)
        {
            IsAlive = 0;
            PickedGold++;
            if (PickedGold == Count)
            {
                Map[DestY][DestX] = 3;
            }
        }
    }
};
Image Gold::Img[8];
int Gold::Loop[8] = {1, 2, 3, 4, 5, 6, 7, 0};
int Gold::CurrentStt = 0;
int Gold::Count = 0;
float Gold::OffsetX = 10.0f;
float Gold::OffsetY = 5.0f;
float Gold::w;
float Gold::h;
Gold *gold[MAXGOLD];

int cWeakPath = 0;
class WeakPath
{
public:
    WeakPath() {}
};
WeakPath *pWeak[3];

int cPatrolPath = 0;
class PatrolPath
{
public:
    PatrolPath(char x, char y, char Drt, char Move, char MaxMove)
    {
        this->x = x;
        this->y = y;
        Map[y][x] = 1;
        this->l = x * SIZE + 2;
        this->b = y * SIZE + 8;
        this->r = this->l + ImgPath[1].w;
        this->t = this->b + ImgPath[1].h;
        this->Drt = Drt;
        this->Stt = 0;
        this->Move = Move;
        this->MaxMove = MaxMove;
        cPatrolPath++;
    }
    char Drt, Stt, Move, MaxMove, x, y;
    float l, r, b, t;
    void Action()
    {
        Stt++;
        if (l < xHero && xHero < r && b < yHero && yHero < t)
        {
            //            xHero += DefOffset3[Drt].x;
            //            xMap -= DefOffset3[Drt].x;
        }
        //        l += DefOffset3[Drt].x;
        //        r += DefOffset3[Drt].x;
        if (Stt == 5)
        {
            Map[y][x + DefOffset[Drt].x] = 1;
        }
        if (Stt == 10)
        {
            Map[y][x] = 2;
            x += DefOffset[Drt].x;
            Stt = 0;
            Move++;
            if (Move == MaxMove)
            {
                Move = 0;
                Drt = 4 - Drt;
            }
        }
    }
    void Draw()
    {
        mapTexture(&ImgPath[1]);
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
    }
};

PatrolPath *pPatrol[3];

int cChaosPath = 0;
char Loop9[] = {8, 0, 1, 2, 3, 4, 5, 6, 7};
char SwitchChaosPath[] = {1, 8};

class ChaosPath
{
public:
    ChaosPath(char x, char y, char move)
    {
        if (move < 2)
            Stt = 1;
        else
            Stt = 0;
        this->x = x;
        this->y = y;
        this->Move = move;
        this->l = x * SIZE + 2;
        this->b = y * SIZE + 8;
        this->r = this->l + ImgPath[2].w;
        this->t = this->b + ImgPath[2].h;
        cChaosPath++;
    }
    char x, y, Stt, Move;
    float l, r, b, t;
    void Action()
    {
        Move = Loop9[Move];
        if (Move == SwitchChaosPath[Stt])
        {
            Stt = 1 - Stt;
        }
    }
    void Draw()
    {
        if (Stt == 1)
        {
            mapTexture(&ImgPath[2]);
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
        }
    }
};
ChaosPath *pChaos[17];

void ImportMap()
{
    FILE *f = fopen("Map.txt", "r");
    fscanf(f, "%d%d", &MaxX, &MaxY);
    for (i = 0; i < MaxY; i++)
    {
        for (j = 0; j < MaxX; j++)
        {
            fscanf(f, "%d", &MapTile[i][j]);
            Map[i][j] = NewMap[MapTile[i][j]];
        }
    }
    fscanf(f, "%d%d%d", &DefX, &DefY, &DefDrt);
    fscanf(f, "%d%d", &DestX, &DestY);
    fscanf(f, "%d", &j);
    int v0, v1, v2, v3, v4, v5;
    for (i = 0; i < j; i++)
    {
        fscanf(f, "%d", &k);
        switch (k)
        {
        case 0:
            fscanf(f, "%d%d%d%d%d", &v0, &v1, &v2, &v3, &v4);
            enemy[i] = new Enemy0(v0, v1, v2, v3, v4);
            break;
        case 1:
            fscanf(f, "%d%d%d%d%d%d", &v0, &v1, &v2, &v3, &v4, &v5);
            enemy[i] = new Enemy1(v0, v1, v2, v3, v4, v5);
            break;
        case 2:
            fscanf(f, "%d%d%d%d%d%d", &v0, &v1, &v2, &v3, &v4, &v5);
            enemy[i] = new Enemy2(v0, v1, v2, v3, v4, v5);
            break;
        case 3:
            fscanf(f, "%d%d%d%d%d%d", &v0, &v1, &v2, &v3, &v4, &v5);
            enemy[i] = new Enemy3(v0, v1, v2, v3, v4, v5);
            break;
        }
    }
    fscanf(f, "%d", &j);
    for (i = 0; i < j; i++)
    {
        fscanf(f, "%d%d", &v0, &v1);
        gold[i] = new Gold(v0, v1);
    }
    fclose(f);
}

void InitEnemy()
{
    // Enemy - 0
    for (i = 0; i < 4; i++)
        for (j = 0; j < 8; j++)
            Enemy0::Img[i][j] = ImgEnemy[0][i][j];
    Enemy0::w = ImgEnemy[0][0][0].w;
    Enemy0::h = ImgEnemy[0][0][0].h;
    // Enemy - 1
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            Enemy1::Img[i][j] = ImgEnemy[1][i][j];
    for (i = 0; i < 4; i++)
        for (j = 4; j < 6; j++)
            Enemy1::Img[i][j] = ImgEnemy[1][i][6 - j];
    Enemy1::w = ImgEnemy[1][0][0].w;
    Enemy1::h = ImgEnemy[1][0][0].h;
    // Enemy - 2
    for (j = 0; j < 6; j++)
        Enemy2::Img[j] = ImgEnemy[2][0][j];
    for (j = 6; j < 10; j++)
        Enemy2::Img[j] = ImgEnemy[2][0][10 - j];
    Enemy2::w = ImgEnemy[2][0][0].w;
    Enemy2::h = ImgEnemy[2][0][0].h;
    // Enemy - 3
    for (j = 0; j < 3; j++)
        Enemy3::Img[j] = ImgEnemy[3][0][j];
    Enemy3::w = ImgEnemy[3][0][0].w;
    Enemy3::h = ImgEnemy[3][0][0].h;
}

void ReInitGame()
{
    xHero = DefX * SIZE + SIZE / 2;
    yHero = DefY * SIZE + SIZE / 2;
    IsRun = 0;
    IsAlive = 1;
    Drt = DefDrt;
    Stt = 0;
    xMap = W2 - xHero;
    yMap = H2 - yHero + 10.0f;
    CountDownRespawn = 0;
    PickedGold = 0;
    for (i = 0; i < Gold::Count; i++)
        gold[i]->IsAlive = 1;
    Map[DestY][DestX] = 0;
}

void InitGame()
{
    char str[21];
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 6; j++)
        {
            sprintf(str, "Images/Hero/%d%d.png", i, j);
            loadTexture(&ImgHero[i][j], str);
        }
    }
    for (i = 0; i < 6; i++)
    {
        sprintf(str, "Images/Hero/E%d.png", i);
        loadTexture(&ImgExplode[i], str);
    }
    for (i = 0; i < MAXTILE; i++)
    {
        sprintf(str, "Images/Tile/%02d.png", i);
        loadTexture(&ImgTile[i], str);
    }
    for (i = 0; i < MAXTYPE; i++)
    {
        for (j = 0; j < MaxDrtEnemy[i]; j++)
        {
            for (k = 0; k < MaxSttEnemy[i]; k++)
            {
                sprintf(str, "Images/Enemy/%d%d%d.png", i, j, k);
                loadTexture(&ImgEnemy[i][j][k], str);
            }
        }
    }
    for (i = 0; i < MAXPATH; i++)
    {
        sprintf(str, "Images/Path/%02d.png", i);
        loadTexture(&ImgPath[i], str);
    }
    for (i = 0; i < 8; i++)
    {
        sprintf(str, "Images/Gold/%02d.png", i);
        loadTexture(&Gold::Img[i], str);
    }
    Gold::w = Gold::Img[0].w;
    Gold::h = Gold::Img[0].h;
    InitEnemy();
    ImportMap();
    ReInitGame();
    l = W2 - ImgHero[0][0].w / 2;
    r = l + ImgHero[0][0].w;
    b = H2 - ImgHero[0][0].h / 2 + 2.0f;
    t = b + ImgHero[0][0].h;
    lE = W2 - ImgExplode[0].w / 2;
    rE = lE + ImgExplode[0].w;
    bE = H2 - ImgExplode[0].h / 2;
    tE = bE + ImgExplode[0].h;
    funcTimer = &timer;
    funcMove[0] = &moveUp;
    funcMove[1] = &moveRight;
    funcMove[2] = &moveDown;
    funcMove[3] = &moveLeft;
    //    pPatrol[0] = new Path(1, 5, 1, 0, 7);
    //    pPatrol[1] = new Path(8, 6, 3, 0, 7);
    //	pChaos[0]=new ChaosPath(1,0,1);
    //	pChaos[1]=new ChaosPath(3,0,1);
    //	pChaos[2]=new ChaosPath(0,0,2);
    //	pChaos[3]=new ChaosPath(1,1,2);
    //	pChaos[4]=new ChaosPath(0,1,3);
    //	pChaos[5]=new ChaosPath(2,1,3);
    //	pChaos[6]=new ChaosPath(1,2,3);
    //	pChaos[7]=new ChaosPath(2,0,4);
    //	pChaos[8]=new ChaosPath(3,1,4);
    //	pChaos[9]=new ChaosPath(1,0,5);
    //	pChaos[10]=new ChaosPath(3,2,5);
    //	pChaos[11]=new ChaosPath(1,1,6);
    //	pChaos[12]=new ChaosPath(0,1,7);
    //	pChaos[13]=new ChaosPath(1,2,7);
    //	pChaos[14]=new ChaosPath(0,2,8);
    //	pChaos[15]=new ChaosPath(2,2,8);
    //	pChaos[16]=new ChaosPath(1,3,8);
}

void InitGL()
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
    InitGame();
}

void Lose()
{
    Stt = 0;
    IsAlive = 0;
}

void Win()
{
    funcTimer = &tNull;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xMap, yMap, 0.0f);
    tmpD2 = tmpD = 0.0f;
    for (i = 0; i < MaxY; i++)
    {
        for (j = 0; j < MaxX; j++)
        {
            if (MapTile[i][j] > -1)
            {
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
    for (i = 0; i < Enemy::Count; i++)
        enemy[i]->Draw();
    for (i = 0; i < Gold::Count; i++)
        gold[i]->Draw();
    glLoadIdentity();
    if (IsAlive)
    {
        mapTexture(&ImgHero[Drt][Stt]);
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
    }
    else
    {
        mapTexture(&ImgExplode[Stt]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(lE, bE);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(rE, bE);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(rE, tE);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(lE, tE);
        glEnd();
    }
    glutSwapBuffers();
}

void resize(int x, int y)
{
    glutPositionWindow(POSX, POSY);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void moveUp()
{
    tmpInt = (int)(yHero - oCanMove) / SIZE;
    tmpI1 = (int)(xHero - oCanMove2) / SIZE;
    tmpI2 = (int)(xHero + oCanMove2) / SIZE;
    if (Map[tmpInt][tmpI1] > 0 && Map[tmpInt][tmpI2] > 0)
    {
        yHero -= oMove;
        yMap += oMove;
    }
}

void moveDown()
{
    tmpInt = (int)(yHero + oCanMove) / SIZE;
    tmpI1 = (int)(xHero - oCanMove2) / SIZE;
    tmpI2 = (int)(xHero + oCanMove2) / SIZE;
    if (Map[tmpInt][tmpI1] > 0 && Map[tmpInt][tmpI2] > 0)
    {
        yHero += oMove;
        yMap -= oMove;
    }
}

void moveLeft()
{
    tmpInt = (int)(xHero - oCanMove) / SIZE;
    tmpI1 = (int)(yHero - oCanMove2) / SIZE;
    tmpI2 = (int)(yHero + oCanMove2) / SIZE;
    if (Map[tmpI1][tmpInt] > 0 && Map[tmpI2][tmpInt] > 0)
    {
        xHero -= oMove;
        xMap += oMove;
    }
}

void moveRight()
{
    tmpInt = (int)(xHero + oCanMove) / SIZE;
    tmpI1 = (int)(yHero - oCanMove2) / SIZE;
    tmpI2 = (int)(yHero + oCanMove2) / SIZE;
    if (Map[tmpI1][tmpInt] > 0 && Map[tmpI2][tmpInt] > 0)
    {
        xHero += oMove;
        xMap -= oMove;
    }
}

void tNull(int value) {}

void timer(int value)
{
    if (IsAlive)
    {
        switch (Map[(int)(yHero - 5) / SIZE][(int)(xHero - 5) / SIZE])
        {
        case 2:
            Lose();
            break;
        case 3:
            Win();
            break;
        }
        if (IsRun)
        {
            funcMove[Drt]();
        }
    }
    Time3 = Loop3[Time3];
    for (i = 0; i < 2; i++)
        TimeSpeed[i] = LoopSpeed[i][TimeSpeed[i]];
    if (Time3 == 0)
    {
        for (i = 0; i < Gold::Count; i++)
            gold[i]->Action();
        if (IsAlive)
            Stt = Loop6[Stt];
        else
        {
            if (Stt < 5)
                Stt++;
            else
            {
                CountDownRespawn++;
                if (CountDownRespawn == 6)
                    ReInitGame();
            }
        }
    }
    for (i = 0; i < Enemy::Count; i++)
    {
        enemy[i]->Action();
        if (IsAlive)
            enemy[i]->CheckLose();
    }
    glutPostRedisplay();
    glutTimerFunc(25, funcTimer, 0);
}

void special(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        Drt = 0;
        IsRun = 1;
        break;
    case GLUT_KEY_RIGHT:
        Drt = 1;
        IsRun = 1;
        break;
    case GLUT_KEY_DOWN:
        Drt = 2;
        IsRun = 1;
        break;
    case GLUT_KEY_LEFT:
        Drt = 3;
        IsRun = 1;
        break;
    }
}

void specialUp(int key, int x, int y)
{
    if (key == Key[Drt])
        IsRun = 0;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POSX, POSY);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Poo's Quest");
    InitGL();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIgnoreKeyRepeat(GL_TRUE);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
