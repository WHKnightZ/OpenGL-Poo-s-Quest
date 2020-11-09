WeakPath->PathWeak
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
int cWeakPath = 0;
class WeakPath {
public:
    WeakPath() {}
};
WeakPath* pWeak[3];

int cPatrolPath = 0;
class PatrolPath {
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
        if (l < xHero && xHero < r && b < yHero && yHero < t) {
            //            xHero += DefOffset3[Drt].x;
            //            xMap -= DefOffset3[Drt].x;
        }
        //        l += DefOffset3[Drt].x;
        //        r += DefOffset3[Drt].x;
        if (Stt == 5) {
            Map[y][x + DefOffset[Drt].x] = 1;
        }
        if (Stt == 10) {
            Map[y][x] = 2;
            x += DefOffset[Drt].x;
            Stt = 0;
            Move++;
            if (Move == MaxMove) {
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

PatrolPath* pPatrol[3];

int cChaosPath = 0;
char Loop9[] = { 8, 0, 1, 2, 3, 4, 5, 6, 7 };
char SwitchChaosPath[] = { 1, 8 };

class ChaosPath {
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
        if (Move == SwitchChaosPath[Stt]) {
            Stt = 1 - Stt;
        }
    }
    void Draw()
    {
        if (Stt == 1) {
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
ChaosPath* pChaos[17];
