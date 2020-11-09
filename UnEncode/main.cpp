#include "../Library/loadpng.h"
#include <GL/glut.h>

/* begin define */
#define POSX 200
#define POSY 40
#define WIDTH 480
#define HEIGHT 600
#define MAXX 20
#define MAXY 20
#define MAXTILE 39
#define MAXPATH 3
#define SIZE 40
#define MAXENEMY 20 //20
#define MAXGOLD 12  //12
#define MAXTYPE 4
#define MAXDRT 4
#define MAXSTT 8
#define DEFSWITCHSTAGE -480.0f
#define DEFSTAGE -143.0f
#define ENDSTAGE 510.0f
#define OFFSETCOLLISION 5.0f;

const float W2 = WIDTH / 2.0f;
const float H2 = HEIGHT / 2.0f;
const float SIZEF = (float)SIZE;
const float COLL1 = -OFFSETCOLLISION;
const float COLL2 = SIZE + 2 * OFFSETCOLLISION;
/* end define */

int ArrStr[500];
int MaxX, MaxY;
const int NewMap[MAXTILE] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0};
int MapTile[MAXY][MAXX];
int Map[MAXY][MAXX];
int i, j, k, tStt = 0;
char IsRun, Drt, Stt;
float l, r, b, t, xHero, yHero, xMap, yMap, lE, rE, bE, tE;
int tmpInt, tmpI1, tmpI2;
int DefX, DefY, DefDrt, CountDownRespawn, DestX, DestY, PickedGold;
float tmp, tmpD, tmpD2;
const char Loop6[] = {1, 2, 3, 4, 5, 0};
const int Key[] = {GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_LEFT};
const int MaxDrtEnemy[MAXTYPE] = {4, 4, 1, 1};
const int MaxSttEnemy[MAXTYPE] = {8, 4, 6, 3};
int Time3;
const int Loop3[] = {1, 2, 0};
int IsAlive;
int Level, MaxLevel;

float oMove = 5.0f, oCanMove = 18.0f, oCanMove2 = 14.0f;

void timer(int);
void (*funcMove[4])();
void moveLeft();
void moveRight();
void moveUp();
void moveDown();
void Lose();
void special(int, int, int);
void specialUp(int, int, int);
void specialMenu(int, int, int);
void keyboard(unsigned char, int, int);
void keyboardMenu(unsigned char, int, int);
void tBeginSwitchStage(int);
void tEndSwitchStage(int);
void tBeginStage(int);
void tEndStage(int);

struct Image {
    GLubyte *img;
    GLuint w, h;
};

/* Main Menu */
struct CompPath {
    int IsRun, Drt;
};

const int MapMenu[17][14] = {{-1, -1, 2, 3, 3, 3, 3, 3, 3, 4, -1, -1, -1, -1},
                             {-1, -1, 11, 23, 23, 23, 23, 23, 23, 12, -1, -1, -1, -1},
                             {-1, -1, 11, 0, 0, 0, 0, 0, 0, 12, -1, -1, -1, -1},
                             {-1, -1, 11, 0, 0, 0, 0, 0, 0, 12, -1, -1, -1, -1},
                             {-1, -1, 7, 6, 0, 10, 8, 6, 0, 12, -1, -1, -1, -1},
                             {-1, -1, -1, 11, 0, 12, -1, 11, 0, 5, 3, 3, 3, 4},
                             {-1, -1, -1, 11, 0, 12, -1, 11, 0, 23, 23, 23, 23, 12},
                             {02, 3, 3, 1, 0, 5, 4, 11, 20, 18, 18, 18, 18, 12},
                             {11, 23, 23, 23, 0, 23, 12, 11, 21, 22, 22, 22, 22, 12},
                             {11, 0, 14, 15, 16, 0, 12, 11, 19, 19, 19, 19, 21, 12},
                             {11, 0, 23, 23, 23, 0, 12, 11, 22, 22, 22, 22, 21, 12},
                             {11, 0, 14, 15, 16, 0, 12, 11, 21, 19, 19, 19, 19, 12},
                             {11, 0, 23, 23, 23, 0, 12, 11, 36, 36, 36, 36, 36, 12},
                             {07, 6, 0, 10, 8, 8, 9, 7, 8, 8, 8, 6, 0, 12},
                             {-1, 11, 0, 12, -1, -1, -1, -1, -1, -1, -1, 11, 0, 12},
                             {-1, 11, 0, 12, -1, -1, -1, -1, -1, -1, -1, 33, 27, 34},
                             {-1, 33, 27, 34, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};
const int MaxXMenu = 14, MaxYMenu = 17, StartXMenu = 2, StartYMenu = 13, DrtMenu = 0, DestXMenu = 12, DestYMenu = 15, CountEnemyMenu = 11, CountGoldMenu = 3;
const int EnemyMenu[] = {2, 1, 12, 1, 0, 4, 4, 1, 2, 5, 8, 3, 0, 4, 4, 1, 0, 3, 10, 1, 2, 4, 0, 3, 4, 4, 0, 0, 60, 60, 3, 4, 6, 0, 0, 60, 60, 3, 4, 2, 0, 0, 60, 20, 3, 5, 2, 0, 40, 60, 20, 3, 6, 2, 0, 0, 60, 20, 3, 7, 2, 0, 40, 60, 20, 1, 3, 3, 0, 0, 5, 2, 0, 1, 8, 1, 2, 0, 5, 2, 0};
const int GoldMenu[] = {3, 10, 4, 5, 12, 6};
Image ImgLogo;
Image ImgButton[2][2];
Image ImgNumBig[10];
Image ImgArrow[2];
int Choice, IsSelectLevel, InGame;
int ButtonStt[2];
float DefOffsetLogo[40];
float DefOffsetArrow[20];
int LoopOffsetLogo[40];
int LoopOffsetArrow[20];
int IsDrawArrow[2];
const int ArrowOffset[] = {1, -1};
int NumOffsetLogo = 0;
int NumOffsetArrow = 0;
const CompPath CPMove[][42] = {
    {{0, 0}, {0, 0}, {0, 0}, {1, 0}, {1, 1}, {1, 1}, {1, 1}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 3}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 3}, {1, 0}, {1, 0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 3}, {1, 3}, {1, 3}, {1, 2}, {1, 2}},
    {{0, 0}, {0, 0}, {0, 0}, {1, 0}, {1, 1}, {1, 1}, {1, 1}, {1, 0}, {1, 0}, {1, 3}, {1, 3}, {1, 1}, {1, 1}, {1, 0}, {1, 0}, {1, 3}, {1, 0}, {0, 0}, {0, 0}, {0, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {1, 3}, {1, 0}, {1, 0}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}, {1, 2}}};
int CurrentCP, CurrentCPStt, TimeCP, IsAtMenu;
/* Show Stage */
Image ImgSwitchStage;
Image ImgNum[10];
Image ImgGray, ImgStage, ImgDeaths;
int NumStage[2];
int Deaths[3];
float lStage, rStage, bStage, tStage;
float OffsetSwitchStage, OffsetStage;
int IsSwitchStage = 0, IsLoadStage = 0;
const float LStageText = 50.0f, RStageText = 148.0f;
const float LDeathsText = 240.0f, RDeathsText = 359.0f;
const float LStageNum[] = {164.0f, 184.0f}, RStageNum[] = {185.0f, 205.0f};
const float LDeathsNum[] = {375.0f, 395.0f, 415.0f}, RDeathsNum[] = {396.0f, 416.0, 436.0f};
const float BStageDeaths = 30.0f, TStageDeaths = 54.0f;
/* --------- */
struct DrtInt {
    char x, y;
};

struct Speed {
    float x, y;
};

Image ImgHero[4][6];
Image ImgExplode[6];
Image ImgEnemy[MAXTYPE][MAXDRT][MAXSTT];
Image ImgTile[MAXTILE];
Image ImgPath[MAXPATH];
const DrtInt DefOffset[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
const Speed SpeedOffset[][4] = {
    {{0.0f, -4.0f}, {4.0f, 0.0f}, {0.0f, 4.0f}, {-4.0f, 0.0f}},
    {{0.0f, -5.0f}, {5.0f, 0.0f}, {0.0f, 5.0f}, {-5.0f, 0.0f}},
    {{0.0f, -40.0f / 7}, {40.0f / 7, 0.0f}, {0.0f, 40.0f / 7}, {-40.0f / 7, 0.0f}}};
const int LoopSpeed[][10] = {{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, {1, 2, 3, 4, 5, 6, 7, 0}, {1, 2, 3, 4, 5, 6, 0}};
int TimeSpeed[3];

void loadTexture(Image *img, const char *path) {
    Load_Png(&img->img, &img->w, &img->h, path);
}

void mapTexture(Image *img) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->w, img->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->img);
}

class Enemy {
  public:
    static int Count;
    int Move, Drt, Stt;
    float l, r, b, t;
    float xl, xr, yb, yt;
    Enemy(int x, int y) {
        Count++;
        xl = x * SIZE + COLL1;
        xr = xl + COLL2;
        yb = y * SIZE + COLL1;
        yt = yb + COLL2;
    }
    virtual void CheckLose() {
        if (xl < xHero && xHero < xr && yb < yHero && yHero < yt) {
            Lose();
        }
    }
    virtual void Action() {}
    virtual void Draw() {}
};
int Enemy::Count = 0;
Enemy *enemy[MAXENEMY];

class Enemy0 : public Enemy {
  public:
    static Image Img[4][8];
    static int Loop[8];
    static float OffsetX, OffsetY;
    static int Switch[4];
    static float w, h;
    int MaxMove, Speed;
    Enemy0(int x, int y, int Drt, int Move, int MaxMove, int Speed)
        : Enemy(x, y) {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        Stt = 0;
        this->Drt = Drt;
        this->Move = Move;
        this->MaxMove = MaxMove;
        this->Speed = Speed;
    }
    void Action() {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        xl += SpeedOffset[Speed][Drt].x;
        yb += SpeedOffset[Speed][Drt].y;
        xr = xl + COLL2;
        yt = yb + COLL2;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0) {
            Move++;
            if (Move == MaxMove) {
                Move = 0;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw() {
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
float Enemy0::w = 0;
float Enemy0::h = 0;

class Enemy1 : public Enemy {
  public:
    static Image Img[4][6];
    static int Loop[6];
    static float OffsetX, OffsetY;
    static int Switch[4];
    static float w, h;
    int MaxMove[2];
    int Drt2, Speed;
    Enemy1(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2, int Speed)
        : Enemy(x, y) {
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
        this->Speed = Speed;
    }
    void Action() {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        xl += SpeedOffset[Speed][Drt].x;
        yb += SpeedOffset[Speed][Drt].y;
        xr = xl + COLL2;
        yt = yb + COLL2;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0) {
            Move++;
            if (Move == MaxMove[Drt2]) {
                Move = 0;
                Drt2 = 1 - Drt2;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw() {
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
float Enemy1::w = 0;
float Enemy1::h = 0;

class Enemy2 : public Enemy {
  public:
    static Image Img[10];
    static int Loop[10];
    static float OffsetX, OffsetY;
    static int Switch[4];
    static float w, h;
    int MaxMove[2];
    int Drt2, Speed;
    Enemy2(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2, int Speed)
        : Enemy(x, y) {
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
        this->Speed = Speed;
    }
    void Action() {
        l += SpeedOffset[Speed][Drt].x;
        b += SpeedOffset[Speed][Drt].y;
        r = l + w;
        t = b + h;
        xl += SpeedOffset[Speed][Drt].x;
        yb += SpeedOffset[Speed][Drt].y;
        xr = xl + COLL2;
        yt = yb + COLL2;
        if (Time3 == 0)
            Stt = Loop[Stt];
        if (TimeSpeed[Speed] == 0) {
            Move++;
            if (Move == MaxMove[Drt2]) {
                Move = 0;
                Drt2 = 1 - Drt2;
                Drt = Switch[Drt];
            }
        }
    }
    void Draw() {
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
float Enemy2::w = 0;
float Enemy2::h = 0;

class Enemy3 : public Enemy {
  public:
    static Image Img[3];
    static int Loop[2][3];
    static float OffsetX, OffsetY;
    static float w, h;
    int MaxMove[2];
    int IsChange;
    Enemy3(int x, int y, int Drt, int Move, int MaxMove, int MaxMove2)
        : Enemy(x, y) {
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
    void Action() {
        if (IsChange) {
            if (Time3 == 0) {
                if (Stt == 1) {
                    IsChange = 0;
                    Stt = Loop[Drt][Stt];
                    Drt = 1 - Drt;
                } else
                    Stt = Loop[Drt][Stt];
            }
        } else {
            Move++;
            if (Move == MaxMove[Drt]) {
                Move = 0;
                IsChange = 1;
            }
        }
    }
    void Draw() {
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
    void CheckLose() {
        if (Drt == 1 && xl < xHero && xHero < xr && yb < yHero && yHero < yt) {
            Lose();
        }
    }
};
Image Enemy3::Img[3];
int Enemy3::Loop[2][3] = {{1, 2, 2}, {0, 0, 1}};
float Enemy3::OffsetX = 6.0f;
float Enemy3::OffsetY = 0.0f;
float Enemy3::w = 0;
float Enemy3::h = 0;

class Gold {
  public:
    static Image Img[8];
    static int Loop[8], CurrentStt, Count;
    static float OffsetX, OffsetY;
    static float w, h;
    float l, r, b, t;
    int Stt, IsAlive;
    Gold(int x, int y) {
        l = x * SIZE + OffsetX;
        b = y * SIZE + OffsetY;
        r = l + w;
        t = b + h;
        this->Stt = CurrentStt;
        this->IsAlive = 1;
        CurrentStt = Loop[CurrentStt];
        Count++;
    }
    void Action() {
        if (IsAlive) {
            if (Time3 == 0)
                Stt = Loop[Stt];
            Collide();
        }
    }
    void Draw() {
        if (IsAlive) {
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
    void Collide() {
        if (IsAlive && l < xHero && xHero < r && b < yHero && yHero < t) {
            IsAlive = 0;
            PickedGold++;
            if (PickedGold == Count) {
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
float Gold::w = 0;
float Gold::h = 0;
Gold *gold[MAXGOLD];

void ImportMenu() {
    Enemy::Count = 0;
    Gold::Count = 0;
    MaxX = MaxXMenu;
    MaxY = MaxYMenu;
    for (i = 0; i < MaxY; i++) {
        for (j = 0; j < MaxX; j++) {
            MapTile[i][j] = MapMenu[i][j];
            Map[i][j] = NewMap[MapTile[i][j]];
        }
    }
    DefX = StartXMenu;
    DefY = StartYMenu;
    DefDrt = DrtMenu;
    DestX = DestXMenu;
    DestY = DestYMenu;
    j = 0;
    for (i = 0; i < CountEnemyMenu; i++) {
        k = EnemyMenu[j++];
        switch (k) {
        case 0:
            enemy[i] = new Enemy0(EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++]);
            break;
        case 1:
            enemy[i] = new Enemy1(EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++]);
            break;
        case 2:
            enemy[i] = new Enemy2(EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++]);
            break;
        case 3:
            enemy[i] = new Enemy3(EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++], EnemyMenu[j++]);
            break;
        }
    }
    if (CountGoldMenu == 0)
        Map[DestY][DestX] = 3;
    else {
        j = 0;
        for (i = 0; i < CountGoldMenu; i++) {
            gold[i] = new Gold(GoldMenu[j++], GoldMenu[j++]);
        }
    }
}

int ImportMap() {
    Enemy::Count = 0;
    Gold::Count = 0;
    char str[12];
    sprintf(str, "Maps/%02d.txt", Level);
    FILE *f = fopen(str, "r");
    if (f == NULL)
        return 0;
    int ch;
    int mm = 0;
    while (1) {
        if (fscanf(f, "%d", &ch) == EOF)
            break;
        ArrStr[mm++] = ch;
    }
    fclose(f);
    mm = 0;
    MaxX = ArrStr[mm++];
    MaxY = ArrStr[mm++];
    for (i = 0; i < MaxY; i++) {
        for (j = 0; j < MaxX; j++) {
            MapTile[i][j] = ArrStr[mm++];
            Map[i][j] = NewMap[MapTile[i][j]];
        }
    }
    DefX = ArrStr[mm++];
    DefY = ArrStr[mm++];
    DefDrt = ArrStr[mm++];
    DestX = ArrStr[mm++];
    DestY = ArrStr[mm++];
    j = ArrStr[mm++];
    for (i = 0; i < j; i++) {
        k = ArrStr[mm++];
        switch (k) {
        case 0:
            enemy[i] = new Enemy0(ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++]);
            break;
        case 1:
            enemy[i] = new Enemy1(ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++]);
            break;
        case 2:
            enemy[i] = new Enemy2(ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++]);
            break;
        case 3:
            enemy[i] = new Enemy3(ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++], ArrStr[mm++]);
            break;
        }
    }
    j = ArrStr[mm++];
    if (j == 0)
        Map[DestY][DestX] = 3;
    for (i = 0; i < j; i++) {
        gold[i] = new Gold(ArrStr[mm++], ArrStr[mm++]);
    }
    return 1;
}

void InitEnemy() {
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

void InitMenu() {
    CurrentCP = 0;
    CurrentCPStt = 0;
    TimeCP = 0;
    IsAtMenu = 1;
    ButtonStt[0] = 1;
    ButtonStt[1] = 0;
    Choice = 0;
    IsSelectLevel = 0;
    Level = 1;
    IsDrawArrow[0] = 0;
    IsDrawArrow[1] = MaxLevel > 1 ? 1 : 0;
    InGame = 0;
}

void InitHero() {
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
    if (Gold::Count > 0)
        Map[DestY][DestX] = 0;
}

void ReInitGame() {
    NumStage[0] = Level / 10;
    NumStage[1] = Level % 10;
    Deaths[0] = Deaths[1] = Deaths[2] = 0;
    Time3 = 0;
    for (i = 0; i < 3; i++)
        TimeSpeed[i] = 0;
    InitHero();
}

void InitOffset() {
    float s = 0.0f, v, a = 0.2f;
    v = 10 * a;
    for (i = 0; i <= 10; i++) {
        DefOffsetLogo[i] = (int)s;
        s += v;
        v -= a;
    }
    for (i = 11; i < 20; i++)
        DefOffsetLogo[i] = DefOffsetLogo[20 - i];
    for (i = 20; i < 30; i++)
        DefOffsetLogo[i] = -DefOffsetLogo[i - 20];
    for (i = 30; i < 40; i++)
        DefOffsetLogo[i] = -DefOffsetLogo[40 - i];
    for (i = 0; i < 39; i++)
        LoopOffsetLogo[i] = i + 1;
    LoopOffsetLogo[39] = 0;
    s = 0.0f, v = 0.0f, a = 0.2f;
    for (i = 0; i <= 10; i++) {
        DefOffsetArrow[i] = (int)s;
        v += a;
        s += v;
    }
    for (i = 11; i < 20; i++) {
        DefOffsetArrow[i] = DefOffsetArrow[20 - i];
    }
    for (i = 0; i < 19; i++)
        LoopOffsetArrow[i] = i + 1;
    LoopOffsetArrow[19] = 0;
}

void InitGame() {
    char str[25];
    MaxLevel = 1;
    FILE *f;
    while (true) {
        sprintf(str, "Maps/%02d.txt", MaxLevel);
        f = fopen(str, "r");
        if (f == NULL)
            break;
        fclose(f);
        MaxLevel++;
    }
    MaxLevel--;
    loadTexture(&ImgLogo, "Images/Menu/Logo.png");
    loadTexture(&ImgButton[0][0], "Images/Menu/Play.png");
    loadTexture(&ImgButton[0][1], "Images/Menu/Play2.png");
    loadTexture(&ImgButton[1][0], "Images/Menu/Load.png");
    loadTexture(&ImgButton[1][1], "Images/Menu/Load2.png");
    loadTexture(&ImgSwitchStage, "Images/Menu/Switch.png");
    for (i = 0; i < 10; i++) {
        sprintf(str, "Images/Num/S%d.png", i);
        loadTexture(&ImgNum[i], str);
        sprintf(str, "Images/Num/B%d.png", i);
        loadTexture(&ImgNumBig[i], str);
    }
    loadTexture(&ImgArrow[0], "Images/Num/L.png");
    loadTexture(&ImgArrow[1], "Images/Num/R.png");
    loadTexture(&ImgStage, "Images/Menu/Stage.png");
    loadTexture(&ImgGray, "Images/Menu/Gray.png");
    loadTexture(&ImgDeaths, "Images/Menu/Deaths.png");
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 6; j++) {
            sprintf(str, "Images/Hero/%d%d.png", i, j);
            loadTexture(&ImgHero[i][j], str);
        }
    }
    for (i = 0; i < 6; i++) {
        sprintf(str, "Images/Hero/E%d.png", i);
        loadTexture(&ImgExplode[i], str);
    }
    for (i = 0; i < MAXTILE; i++) {
        sprintf(str, "Images/Tile/%02d.png", i);
        loadTexture(&ImgTile[i], str);
    }
    for (i = 0; i < MAXTYPE; i++) {
        for (j = 0; j < MaxDrtEnemy[i]; j++) {
            for (k = 0; k < MaxSttEnemy[i]; k++) {
                sprintf(str, "Images/Enemy/%d%d%d.png", i, j, k);
                loadTexture(&ImgEnemy[i][j][k], str);
            }
        }
    }
    for (i = 0; i < MAXPATH; i++) {
        sprintf(str, "Images/Path/%02d.png", i);
        loadTexture(&ImgPath[i], str);
    }
    for (i = 0; i < 8; i++) {
        sprintf(str, "Images/Gold/%02d.png", i);
        loadTexture(&Gold::Img[i], str);
    }
    Gold::w = Gold::Img[0].w;
    Gold::h = Gold::Img[0].h;
    InitOffset();
    InitMenu();
    InitEnemy();
    ImportMenu();
    ReInitGame();
    l = W2 - ImgHero[0][0].w / 2;
    r = l + ImgHero[0][0].w;
    b = H2 - ImgHero[0][0].h / 2 + 2.0f;
    t = b + ImgHero[0][0].h;
    lE = W2 - ImgExplode[0].w / 2;
    rE = lE + ImgExplode[0].w;
    bE = H2 - ImgExplode[0].h / 2;
    tE = bE + ImgExplode[0].h;
    funcMove[0] = &moveUp;
    funcMove[1] = &moveRight;
    funcMove[2] = &moveDown;
    funcMove[3] = &moveLeft;
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

void Lose() {
    Deaths[2]++;
    if (Deaths[2] == 10) {
        Deaths[2] = 0;
        Deaths[1]++;
        if (Deaths[1] == 10) {
            Deaths[1] = 0;
            Deaths[0]++;
        }
    }
    Stt = 0;
    IsAlive = 0;
    if (IsAtMenu) {
        CurrentCPStt = TimeCP = 0;
        CurrentCP = 1 - CurrentCP;
    }
}

void Win() {
    IsRun = 0;
    Level++;
    glutSpecialFunc(NULL);
    glutSpecialUpFunc(NULL);
    InGame = 0;
    IsSwitchStage = 1;
    OffsetSwitchStage = DEFSWITCHSTAGE;
    glutTimerFunc(150, tBeginSwitchStage, 0);
}

void displayMenu() {
    mapTexture(&ImgLogo);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(89.0f, 70.0f + DefOffsetLogo[NumOffsetLogo]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(391.0f, 70.0f + DefOffsetLogo[NumOffsetLogo]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(391.0f, 238.0f + DefOffsetLogo[NumOffsetLogo]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(89.0f, 238.0f + DefOffsetLogo[NumOffsetLogo]);
    glEnd();
    if (IsSelectLevel) {
        mapTexture(&ImgGray);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 380.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(480.0f, 380.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(480.0f, 492.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.0f, 492.0f);
        glEnd();
        for (i = 0; i < 2; i++) {
            mapTexture(&ImgNumBig[NumStage[i]]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(177.0f + 63 * i, 400.0f);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(240.0f + 63 * i, 400.0f);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(240.0f + 63 * i, 472.0f);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(177.0f + 63 * i, 472.0f);
            glEnd();
            if (IsDrawArrow[i]) {
                tmp = 176 * i + ArrowOffset[i] * DefOffsetArrow[NumOffsetArrow];
                mapTexture(&ImgArrow[i]);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2f(140.0f + tmp, 415.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2f(164.0f + tmp, 415.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex2f(164.0f + tmp, 457.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex2f(140.0f + tmp, 457.0f);
                glEnd();
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            mapTexture(&ImgButton[i][ButtonStt[i]]);
            glBegin(GL_POLYGON);
            glTexCoord2f(0.0f, 0.0f);
            glVertex2f(135.0f, 380.0f + 80 * i);
            glTexCoord2f(1.0f, 0.0f);
            glVertex2f(345.0f, 380.0f + 80 * i);
            glTexCoord2f(1.0f, 1.0f);
            glVertex2f(345.0f, 449.0f + 80 * i);
            glTexCoord2f(0.0f, 1.0f);
            glVertex2f(135.0f, 449.0f + 80 * i);
            glEnd();
        }
    }
}

void displaySwitchStage() {
    mapTexture(&ImgSwitchStage);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(OffsetSwitchStage, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(480.0f + OffsetSwitchStage, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(480.0f + OffsetSwitchStage, 600.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(OffsetSwitchStage, 600.0f);
    glEnd();
}

void displayStage() {
    lStage = 0.0f;
    bStage = 264.0f;
    rStage = 480.0f;
    tStage = 336.0f;
    mapTexture(&ImgGray);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(lStage, bStage);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(rStage, bStage);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(rStage, tStage);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(lStage, tStage);
    glEnd();
    lStage = OffsetStage;
    bStage = 288.0f;
    rStage = lStage + 98.0f;
    tStage = 312.0f;
    mapTexture(&ImgStage);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(lStage, bStage);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(rStage, bStage);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(rStage, tStage);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(lStage, tStage);
    glEnd();
    lStage += 104.0f;
    rStage = lStage + 21.0f;
    mapTexture(&ImgNum[NumStage[0]]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(lStage, bStage);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(rStage, bStage);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(rStage, tStage);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(lStage, tStage);
    glEnd();
    lStage += 20.0f;
    rStage = lStage + 21.0f;
    mapTexture(&ImgNum[NumStage[1]]);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(lStage, bStage);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(rStage, bStage);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(rStage, tStage);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(lStage, tStage);
    glEnd();
}

void displayInGame() {
    mapTexture(&ImgGray);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(0.0f, 12.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(480.0f, 12.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(480.0f, 72.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(0.0f, 72.0f);
    glEnd();
    mapTexture(&ImgStage);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(LStageText, BStageDeaths);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(RStageText, BStageDeaths);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(RStageText, TStageDeaths);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(LStageText, TStageDeaths);
    glEnd();
    for (i = 0; i < 2; i++) {
        mapTexture(&ImgNum[NumStage[i]]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(LStageNum[i], BStageDeaths);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(RStageNum[i], BStageDeaths);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(RStageNum[i], TStageDeaths);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(LStageNum[i], TStageDeaths);
        glEnd();
    }
    mapTexture(&ImgDeaths);
    glBegin(GL_POLYGON);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(LDeathsText, BStageDeaths);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(RDeathsText, BStageDeaths);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(RDeathsText, TStageDeaths);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(LDeathsText, TStageDeaths);
    glEnd();
    for (i = 0; i < 3; i++) {
        mapTexture(&ImgNum[Deaths[i]]);
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(LDeathsNum[i], BStageDeaths);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(RDeathsNum[i], BStageDeaths);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(RDeathsNum[i], TStageDeaths);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(LDeathsNum[i], TStageDeaths);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(xMap, yMap, 0.0f);
    tmpD = 0.0f;
    tmpD2 = 0.0f;
    for (i = 0; i < MaxY; i++) {
        for (j = 0; j < MaxX; j++) {
            if (MapTile[i][j] > -1) {
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
    if (IsAlive) {
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
    } else {
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
    if (InGame)
        displayInGame();
    else {
        if (IsAtMenu)
            displayMenu();
        if (IsSwitchStage)
            displaySwitchStage();
        else if (IsLoadStage)
            displayStage();
    }
    glutSwapBuffers();
}

void resize(int x, int y) {
    glutPositionWindow(POSX, POSY);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void moveUp() {
    tmpInt = (int)(yHero - oCanMove) / SIZE;
    tmpI1 = (int)(xHero - oCanMove2) / SIZE;
    tmpI2 = (int)(xHero + oCanMove2) / SIZE;
    if (Map[tmpInt][tmpI1] > 0 && Map[tmpInt][tmpI2] > 0) {
        yHero -= oMove;
        yMap += oMove;
    }
}

void moveDown() {
    tmpInt = (int)(yHero + oCanMove) / SIZE;
    tmpI1 = (int)(xHero - oCanMove2) / SIZE;
    tmpI2 = (int)(xHero + oCanMove2) / SIZE;
    if (Map[tmpInt][tmpI1] > 0 && Map[tmpInt][tmpI2] > 0) {
        yHero += oMove;
        yMap -= oMove;
    }
}

void moveLeft() {
    tmpInt = (int)(xHero - oCanMove) / SIZE;
    tmpI1 = (int)(yHero - oCanMove2) / SIZE;
    tmpI2 = (int)(yHero + oCanMove2) / SIZE;
    if (Map[tmpI1][tmpInt] > 0 && Map[tmpI2][tmpInt] > 0) {
        xHero -= oMove;
        xMap += oMove;
    }
}

void moveRight() {
    tmpInt = (int)(xHero + oCanMove) / SIZE;
    tmpI1 = (int)(yHero - oCanMove2) / SIZE;
    tmpI2 = (int)(yHero + oCanMove2) / SIZE;
    if (Map[tmpI1][tmpInt] > 0 && Map[tmpI2][tmpInt] > 0) {
        xHero += oMove;
        xMap -= oMove;
    }
}

void timerMenu() {
    NumOffsetLogo = LoopOffsetLogo[NumOffsetLogo];
    NumOffsetArrow = LoopOffsetArrow[NumOffsetArrow];
    if (IsAlive) {
        TimeCP++;
        if (TimeCP == 8) {
            TimeCP = 0;
            IsRun = CPMove[CurrentCP][CurrentCPStt].IsRun;
            Drt = CPMove[CurrentCP][CurrentCPStt].Drt;
            CurrentCPStt++;
        }
    }
}

void tBeginSwitchStage(int value) {
    if (OffsetSwitchStage < 0.0f) {
        OffsetSwitchStage += 60.0f;
        glutTimerFunc(25, tBeginSwitchStage, 0);
    } else {
        if (ImportMap()) {
            ReInitGame();
        } else {
            InitMenu();
            ImportMenu();
            ReInitGame();
        }
        glutTimerFunc(800, tEndSwitchStage, 0);
    }
}

void tEndSwitchStage(int value) {
    if (OffsetSwitchStage < WIDTH) {
        OffsetSwitchStage += 60.0f;
        glutTimerFunc(25, tEndSwitchStage, 0);
    } else {
        IsSwitchStage = 0;
        if (IsAtMenu) {
            glutKeyboardFunc(keyboardMenu);
            glutSpecialFunc(specialMenu);
        } else {
            IsLoadStage = 1;
            OffsetStage = DEFSTAGE;
            glutTimerFunc(0, tBeginStage, 0);
        }
    }
}

void tBeginStage(int value) {
    if (OffsetStage < 150.0f) {
        printf("%f\n", OffsetStage);
        OffsetStage += 24.0f;
        glutTimerFunc(25, tBeginStage, 0);
    } else
        glutTimerFunc(800, tEndStage, 0);
}

void tEndStage(int value) {
    if (OffsetStage < ENDSTAGE) {
        OffsetStage += 24.0f;
        glutTimerFunc(25, tEndStage, 0);
    } else {
        IsLoadStage = 0;
        InGame = 1;
        glutKeyboardFunc(keyboard);
        glutSpecialFunc(special);
        glutSpecialUpFunc(specialUp);
    }
}

void timer(int value) {
    // Main Menu
    if (IsAtMenu)
        timerMenu();
    // Game
    if (IsAlive) {
        switch (Map[(int)(yHero - 5) / SIZE][(int)(xHero - 5) / SIZE]) {
        case 2:
            Lose();
            break;
        case 3:
            if (!IsSwitchStage)
                Win();
            break;
        }
        if (IsRun) {
            funcMove[Drt]();
        }
    }
    Time3 = Loop3[Time3];
    for (i = 0; i < 3; i++)
        TimeSpeed[i] = LoopSpeed[i][TimeSpeed[i]];
    if (Time3 == 0) {
        for (i = 0; i < Gold::Count; i++)
            gold[i]->Action();
        if (IsAlive)
            Stt = Loop6[Stt];
        else {
            if (Stt < 5)
                Stt++;
            else {
                CountDownRespawn++;
                if (CountDownRespawn == 6)
                    InitHero();
            }
        }
    }
    for (i = 0; i < Enemy::Count; i++) {
        enemy[i]->Action();
        if (IsAlive)
            enemy[i]->CheckLose();
    }
    glutPostRedisplay();
    glutTimerFunc(25, timer, 0);
}

void specialMenu(int key, int x, int y) {
    if (IsSelectLevel) {
        switch (key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_LEFT:
            if (Level > 1) {
                Level--;
                if (Level == 1)
                    IsDrawArrow[0] = 0;
                IsDrawArrow[1] = 1;
                NumStage[0] = Level / 10;
                NumStage[1] = Level % 10;
            }
            break;
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
            if (Level < MaxLevel) {
                Level++;
                if (Level == MaxLevel)
                    IsDrawArrow[1] = 0;
                IsDrawArrow[0] = 1;
                NumStage[0] = Level / 10;
                NumStage[1] = Level % 10;
            }
            break;
        }
    } else {
        switch (key) {
        case GLUT_KEY_UP:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
        case GLUT_KEY_LEFT:
            ButtonStt[Choice] = 0;
            Choice = 1 - Choice;
            ButtonStt[Choice] = 1;
            break;
        }
    }
}

void keyboardMenu(unsigned char key, int x, int y) {
    if (IsSelectLevel) {
        if (key == 13) {
            IsAtMenu = 0;
            glutKeyboardFunc(NULL);
            IsSwitchStage = 1;
            OffsetSwitchStage = DEFSWITCHSTAGE;
            glutTimerFunc(150, tBeginSwitchStage, 0);
        } else if (key == 27) {
            IsSelectLevel = 0;
        }
    } else {
        if (key == 13) {
            if (Choice == 0) {
                Level = 1;
                IsAtMenu = 0;
                glutKeyboardFunc(NULL);
                IsSwitchStage = 1;
                OffsetSwitchStage = DEFSWITCHSTAGE;
                glutTimerFunc(150, tBeginSwitchStage, 0);
            } else
                IsSelectLevel = 1;
        }
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) {
        Level = MaxLevel + 1;
        Win();
    }
}

void special(int key, int x, int y) {
    switch (key) {
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

void specialUp(int key, int x, int y) {
    if (key == Key[Drt])
        IsRun = 0;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(POSX, POSY);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Poo's Quest");
    InitGL();
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutIgnoreKeyRepeat(GL_TRUE);
    glutSpecialFunc(specialMenu);
    glutKeyboardFunc(keyboardMenu);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
