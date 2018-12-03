#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
using namespace std;
 
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
const int unit = 1;
const int reso = 4;
//*****************************************************
 
// <Functions Header>
//*****************************************************
// input information
void inputInfo();
void createMap();
 
double lnDanger(Point from, Point to, double firstD);
double ttDanger(vector<Point> corner);
void outputSol();
//*****************************************************
 
int main()
{
    inputInfo();
 
    createMap();
 
    // for(int i = n; i >= 0; i--)
    // {
    //  for(int j = 0; j <= n; j++)
    //  {
    //      cout << map[j][i] << " ";
    //  }
    //  cout << "\n";
    // }
 
 
 
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
 
void createMap()
{
    map = new double* [reso*n+1];
    for(unsigned int i = 0; i <= reso*n; i++)
    {
        map[i] = new double [reso*n+1];
        for(unsigned int j = 0; j <= reso*n; j++)
        {
            Point pt(1.0*i/reso, 1.0*j/reso);
            map[i][j] = pt.danger();
        }
    }
}
 
// return the danger of a line
double lnDanger(Point from, Point to, double firstD)
{
    //cout << "\n<lnDanger>\n";
    double d = from.distance(to);
    //cout << "d = " << d << "\n";
     
    unsigned int checkCnt = floor((d-firstD)/unit) + 1;
 
    double lnDan = 0;
 
    Point checkPt = {from.x + (to.x-from.x)*firstD/d, from.y + (to.y-from.y)*firstD/d};
    lnDan += map[static_cast<int>(1.0*reso*checkPt.x+0.5)][static_cast<int>(1.0*reso*checkPt.y+0.5)];
 
    for(unsigned int i = 1; i < checkCnt; i++)
    {
        checkPt = {checkPt.x + (to.x-from.x)*unit/d, checkPt.y + (to.y-from.y)*unit/d};
 
        if(checkPt.x == e.x && checkPt.y == e.y)
            break;
        else
            lnDan += map[static_cast<int>(1.0*reso*checkPt.x+0.5)][static_cast<int>(1.0*reso*checkPt.y+0.5)];
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