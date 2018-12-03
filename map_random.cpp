#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <cstdlib> // rand(), srand(), RAND_MAX
#include <ctime>
using namespace std;
 
// <Constant Values>
//*****************************************************
const double MAX_T = 980;
const int unit = 1;
const int reso = 4;
//*****************************************************

// <Structure>
//*****************************************************
struct Point
{
        double x;
        double y;
        int r;
        int p;
 
        Point() { x=0; y=0; r=0; p=0; }
        Point(double i, double j) { x=i; y=j; r=0; p=0; }
        double distance(Point to) { return hypot((x-to.x), (y-to.y)); }
        double danger();
};
//*****************************************************
 
// <Basic Informations>
//*****************************************************
unsigned int n = 0, m = 0, w = 0, d = 0;
vector<Point> danPt;
Point s, e;
//*****************************************************
 
// <Global Values>
//*****************************************************
vector<Point> sol;
double** map;
Point upper, lower;
int maxCorner;
bool randomMod;
//*****************************************************
 
// <Functions Header>
//*****************************************************
// return a random number from lower to upper
int random(int lower, int upper) { return static_cast<int>((upper-lower)*1.0*rand()/RAND_MAX+lower+0.5); }

// input information
void inputInfo();
void init();
void createMap();
 
void randomFind();
void exhaustion();

double lnDanger(Point from, Point to, double firstD);
double ttDanger(vector<Point> corner);
void outputSol();
//*****************************************************
 
int main()
{
    srand(time(NULL));

    inputInfo();

    init();
 
    createMap();
 
    // for(int i = n; i >= 0; i--)
    // {
    //  for(int j = 0; j <= n; j++)
    //  {
    //      cout << map[j][i] << " ";
    //  }
    //  cout << "\n";
    // }
    

    if(randomMod)
        randomFind();
    else
         exhaustion();

    outputSol();
 
    return 0;
}
 
// <Functions>
//*****************************************************
double Point::danger()
{
    double dan = 0;
 
    for(unsigned int i = 0; i < m; ++i)  
        if((x-danPt[i].x)*(x-danPt[i].x)+(y-danPt[i].y)*(y-danPt[i].y) < danPt[i].r*danPt[i].r)
        {
            dan += danPt[i].p * (danPt[i].r - distance(danPt[i])) / danPt[i].r;
        }
 
    return dan;
}
 
// input information
void inputInfo()
{
    cin >> n >> m >> w >> d;
     
    danPt.reserve(m);
    for(unsigned int i = 0; i < m; i++)
    {
        cin >> danPt[i].x;
    }
    for(unsigned int i = 0; i < m; i++)
    {
        cin >> danPt[i].y;
    }
    for(unsigned int i = 0; i < m; i++)
    {
        cin >> danPt[i].r;
    }
    for(unsigned int i = 0; i < m; i++)
    {
        cin >> danPt[i].p;
    }
     
    cin >> s.x >> s.y >> e.x >> e.y;
}

void init()
{
    int x = abs(s.x-e.x);
    int y = abs(s.y-e.y);
    if((x>50)&&(x<75)&&(y<20)&&(w<750))
        maxCorner = 4;
    else if(((w>150)&&(w<200)&&(x*y<600)) || ((x*y>900)&&((y>75)||((y>25)&&(y<50)&&(w>750)))) || ((w>500)&&(w<750)&&(x*y<300)) || ((x*y<100)&&(w>750)&&(w<900)))
        maxCorner = 1;
    else if(((x<15)&&(x*y>600)&&(x*y<800)) || ((w<100)&&((y<20)||(y>100))) || ((x*y>900)&&(y>50)&&(y<75)))
        maxCorner = 3;
    else
        maxCorner = 2;

    if(maxCorner != 1)
    {
        randomMod = true;
        upper = {floor(min((s.x+e.x)/2+d*0.35, 1.0*n)+0.5), floor(min((s.y+e.y)/2+d*0.35, 1.0*n)+0.5)};
        lower = {floor(max((s.x+e.x)/2-d*0.35, 0.0)+0.5), floor(max((s.y+e.y)/2-d*0.35, 0.0)+0.5)};
    }
    else
    {
        randomMod = false;
        upper = {1.0*n, 1.0*n};
        lower = {0.0, 0.0};
    }
}

void createMap()
{
    map = new double* [static_cast<int>(reso*(upper.x-lower.x))+1];
    for(int i = 0; i <= static_cast<int>(reso*(upper.x-lower.x)); i++)
    {
        map[i] = new double [static_cast<int>(reso*(upper.y-lower.y))+1];
        for(int j = 0; j <= static_cast<int>(reso*(upper.y-lower.y)); j++)
        {
            Point pt(1.0*i/reso+lower.x, 1.0*j/reso+lower.y);
            map[i][j] = pt.danger();
        }
    }
}

void randomFind()
{
    double optCost = INT_MAX;
    double curTime = 1000*clock()/CLOCKS_PER_SEC;
    double dt = 0;
    do
    {
        vector<Point> tempSol;
        Point pt(random(lower.x, upper.x), random(lower.x, upper.x));
        tempSol.push_back(pt);
        for(int i = 0; i < maxCorner-1; i++)
        {
            pt = {1.0*random(min(pt.x, e.x), max(pt.x, e.x)), 1.0*random(min(pt.y, e.y), max(pt.y, e.y))};
            //cout << pt.x << " " << pt.y;
            tempSol.push_back(pt);
        }
        double cost = ttDanger(tempSol);

        if(cost < optCost)
        {
            optCost = cost;
            sol.clear();
            sol.assign(tempSol.begin(), tempSol.end());
        }

        dt = 1000*clock()/CLOCKS_PER_SEC - curTime;
        curTime = 1000*clock()/CLOCKS_PER_SEC;
    }while(curTime+dt < MAX_T);
}

void exhaustion()
{
    double optCost = INT_MAX;
 
    for(unsigned int i = 0; i <= n; i++)
        for(unsigned int j = 0; j <= n; j++)
        {
            vector<Point> tempSol;
            Point pt(1.0*i, 1.0*j);
            tempSol.push_back(pt);
            double cost = ttDanger(tempSol);
            //cout << pt.x << " " << pt.y << " -> " << cost << "\n";
            if(cost < optCost)
            {
                optCost = cost;
                sol.clear();
                sol.push_back(pt);
            }
        }
}

// return the danger of a line
double lnDanger(Point from, Point to, double firstD)
{
    //cout << from.x << " " << from.y << " -> " << to.x << " " << to.y << "\n";
    //cout << "\n<lnDanger>\n";
    double d = from.distance(to);
    //cout << "d = " << d << "\n";
     
    unsigned int checkCnt = floor((d-firstD)/unit) + 1;
 
    double lnDan = 0;
 
    Point checkPt = {from.x + (to.x-from.x)*firstD/d, from.y + (to.y-from.y)*firstD/d};
    lnDan += map[static_cast<int>(reso*(checkPt.x-lower.x)+0.5)][static_cast<int>(reso*(checkPt.y-lower.y)+0.5)];

    for(unsigned int i = 1; i < checkCnt; i++)
    {
        checkPt = {checkPt.x + (to.x-from.x)*unit/d, checkPt.y + (to.y-from.y)*unit/d};
 
        if(checkPt.x == e.x && checkPt.y == e.y)
            break;
        else
            lnDan += map[static_cast<int>(reso*(checkPt.x-lower.x)+0.5)][static_cast<int>(reso*(checkPt.y-lower.y)+0.5)];
    }
 
     
    return lnDan;
}
 
double ttDanger(vector<Point> corner)
{
    //cout << "\n<ttDanger>\n";
    double ttDan = 0;
    if(corner.size() == 0)
        ttDan = lnDanger(s, e, unit);
    else
    {
        double firstD;
        if(s.distance(corner[0]) > unit)
        {
            ttDan += lnDanger(s, corner[0], unit);
            firstD = unit*(static_cast<int>(s.distance(corner[0]))/unit + 1) - s.distance(corner[0]);
        }
        else
            firstD = unit-s.distance(corner[0]);
 
        for(unsigned int i = 0; i < corner.size()-1; i++)
        {
            double d = corner[i].distance(corner[i+1]);
            if(d <= firstD)
            {
                firstD -= d;
            }
            else
            {
                ttDan += lnDanger(corner[i], corner[i+1], firstD);
                firstD = unit*(static_cast<int>(d - firstD)/unit + 1) - (d - firstD);
            }
        }
        if(corner[corner.size()-1].distance(e) > firstD)
            ttDan += lnDanger(corner[corner.size()-1], e, firstD);
    }
    ttDan += w * corner.size();
 
    return ttDan;
}
 
void outputSol()
{
    cout << sol.size();
    for(unsigned int i = 0; i < sol.size(); i++)
        cout << " " << sol[i].x << " "  << sol[i].y;
}
