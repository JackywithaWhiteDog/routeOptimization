//
//  main.cpp
//  testin
//
//  Created by chihsin on 2018/11/12.
//  Copyright © 2018年 chihsin. All rights reserved.
//
#include <iostream> // cin, cout
#include <algorithm> // min(), max()
#include <cmath> // sqrt()
#include <cstdlib> // rand(), srand(), RAND_MAX
#include <ctime> // clock(), time(NULL)
#include <climits> // INT_MAX
using namespace std;

///const int MAX_CORNER = 1000000;

class point
{
public:
    double x, y;
    int radius, danger;
    void init(double i, double j);
    void init(double i, double j, int r, int p);
};

// return a random number from lower to upper
int random(int lower, int upper);

bool islarger(int a, int b);

// input information
void getInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end);
// randomly create information
void randInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end);

// return the distance of two point
double distance(point a, point b);

// return the derivative of the danger on the point(type == true -> /dx; type == false -> /dy)
double dDanger(point pt, point* danPt, int m, bool type);

/*
// find a corner on the line
point findCorner(point from, point to, int n, point* danPt, int m, double maxD, int firstD = 1);
*/

point psoFindCorner(point start, point end, point* danPt, int m, point pBCorner[], point &gBCorner, double pBest[], double &gBest, point &velocity, double startT, int dist);


// check if the path is legal(all the point is in the map and does not exceed the distance)
bool islegal(point start, point end, point corner[], int corCnt, int n, int d);

// return the danger of a point
double pointDanger(point pt, point* danPt, int m);
// return the danger of a line
double lineDanger(point from, point to, point* danPt, int m, double firstD = 1);
// return the danger of a path
double totalDanger(point start, point end, point corner[], int corCnt, point* danPt, int m);

int main()
{
    srand(time(NULL));
    double startT = 1000*clock()/CLOCKS_PER_SEC;
    //cout << startT << "\n";
    
    int mapSize = 0, ptCnt = 0, weight = 0, dist = 0;
    point* danPt = nullptr;
    point start, end;
    
    //insert data
    
    getInfo(mapSize, ptCnt, weight, dist, danPt, start, end);
    
    //randInfo(mapSize, ptCnt, weight, distance, danPt, start, end);
    
    //cout << start.x << " " << start.y << "\n";
    //cout << end.x << " " << end.y << "\n";
    
    
    point pBCorner[3];
    point gBCorner;
    double pBest[3] = {10000};
    double gBest = 10000000000;
    point velocity = {0, 0};
    
    
    psoFindCorner(start, end, danPt, ptCnt, pBCorner, gBCorner, pBest, gBest, velocity, startT, dist);
    gBest = gBest + weight;
    
    
    point gBCorner2;
    double gBest2 = 100000000;
    bool a = 0;
    if (lineDanger(start, gBCorner, danPt, ptCnt, 1) <= lineDanger(gBCorner, end, danPt, ptCnt, 1))
    {
        psoFindCorner(gBCorner, end, danPt, ptCnt, pBCorner, gBCorner2, pBest, gBest2, velocity, startT, dist);
        a = 1;
    }
    else
    {
        psoFindCorner(start, gBCorner, danPt, ptCnt, pBCorner, gBCorner2, pBest, gBest2, velocity, startT, dist);
        a = 0;
    }
    
    double smallest = 0;
    smallest = min(gBest, gBest2);
    smallest = min(smallest, lineDanger(start, end, danPt, ptCnt, 1));
    
    if (smallest == gBest)
        cout << "1" << " " << gBCorner.x <<  " " << gBCorner.y;
    else if (smallest == gBest2 && a == 1)
    {
        if (gBCorner2.x == gBCorner.x && gBCorner2.y == gBCorner.y)
            cout << "1" << " " << gBCorner.x <<  " " << gBCorner.y;
        else
            cout << "2" << " " << gBCorner.x << " " << gBCorner.y << " " << gBCorner2.x << " " << gBCorner2.y;
    }
    else if (smallest == gBest2 && a == 0)
    {
        if (gBCorner2.x == gBCorner.x && gBCorner2.y == gBCorner.y)
            cout << "1" << " " << gBCorner.x <<  " " << gBCorner.y;
        else
            cout << "2" << " " << gBCorner2.x << " " << gBCorner2.y << " " << gBCorner.x << " " << gBCorner.y;
    }
    else
        cout << "0";
    
    
    
    
    //double curT = 1000*clock()/CLOCKS_PER_SEC;
    //double dt = 0;
    
    /*
    point ans[1];
    ans[0].init(start.x, start.y);
    double optCost = INT_MAX;
    
    int cnt = 0;
    while(curT+dt <= 1000)
    {
        point temp[1];
        temp[0] = findCorner(start, end, mapSize, danPt, ptCnt, distance);
        //cout << "-done\n";
        double cost = totalDanger(start, end, temp, 1, danPt, ptCnt) + weight;
        //cout << "-done\n";
        cout << temp[0].x << " " << temp[0].y << " --> " << cost << "\n";
        
        if(cost < optCost)
        {
            optCost = cost;
            ans[0] = temp[0];
        }
        
        cnt++;
        dt = 1000*clock()/CLOCKS_PER_SEC - curT;
        curT = 1000*clock()/CLOCKS_PER_SEC;
    }
    
    cout << "\n";
    cout << mapSize << " " << ptCnt << " " << weight << " " << distance << "\n";
    cout << start.x << " " << start.y << " " << end.x << " " << end.y << "\n";
    
    cout << "\nbest: " << ans[0].x << " " << ans[0].y << " --> " << optCost << "\n";
    
    double naiveCost = lineDanger(start, end, danPt, ptCnt);
    cout << "straight: " << naiveCost << "\n";
    if(optCost <= naiveCost)
    {
        cout << "SUCCESS!!!\n\n";
    }
    else
    {
        cout << "FAIL...\n\n";
    }
    
    cout << "time: " << curT - startT << "\n";
    cout << "loop: " << cnt << "\n";
    */
    
    return 0;
}

bool islarger(int a, int b)
{
    if(a >= b)
        return true;
    else
        return false;
}

void point::init(double i, double j)
{
    x = i;
    y = j;
    radius = 0;
    danger = 0;
}
void point::init(double i, double j, int r, int p)
{
    x = i;
    y = j;
    radius = r;
    danger = p;
}

// return a random number from lower to upper
int random(int lower, int upper)
{
    return static_cast<int>((upper-lower)*static_cast<double>(rand())/RAND_MAX+lower);
}

// input information
void getInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end)
{
    cin >> n >> m >> w >> d;
    
    danPt = new point [m];
    for(int i = 0; i < m; i++)
    {
        cin >> danPt[i].x;
    }
    for(int i = 0; i < m; i++)
    {
        cin >> danPt[i].y;
    }
    for(int i = 0; i < m; i++)
    {
        cin >> danPt[i].radius;
    }
    for(int i = 0; i < m; i++)
    {
        cin >> danPt[i].danger;
    }
    
    
    cin >> start.x >> start.y >> end.x >> end.y;
    
    start.init(start.x, start.y);
    end.init(end.x, end.y);
}

// randomly create information
void randInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end)
{
    n = random(1, 1000);
    m = random(1, min((n+1)*(n+1), 10000));
    w = random(0, 1000);
    
    danPt = new point [m];
    
    for(int i = 0; i < m; i++)
    {
        danPt[i].init(random(0,n), random(0,n), random(0,n), random(1,1000));
    }
    
    start.init(random(0,n), random(0,n));
    end.init(random(0,n), random(0,n));
    
    d = random(max(n, static_cast<int>(distance(start, end))+1), min(10*n, static_cast<int>(distance(start, end))+1));
}

// return the distance of two point
double distance(point a, point b)
{
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

point psoFindCorner(point start, point end, point* danPt, int m, point pBCorner[], point &gBCorner, double pBest[], double &gBest, point &velocity, double startT, int dist)
{
    point particles[3];
    double b = 0;
    b = (end.y - start.y) / (end.x - start.x);
    for (int i = 0; i < 3; i++)
    {
        particles[i].x = random(start.x, end.x);
        particles[i].y = static_cast<int>(b * particles[i].x + start.y);
        //cout << particles[i].x << " " << particles[i].y << "\n";
    }
    int c1 = 1, c2 = 1;//can modify
    double we = 0.6;
    double curT = 1000*clock()/CLOCKS_PER_SEC;
    double dT = 0;
    while(dT + curT - startT < 400)
    {
        for (int i = 0; i < 3; i++)
        {
            //calculate
            if (totalDanger(start, end, &particles[i], 1, danPt, m) <= pBest[i])
            {
                pBest[i] = totalDanger(start, end, &particles[i], 1, danPt, m);
                pBCorner[i].x = particles[i].x;
                pBCorner[i].y = particles[i].y;
            }//update PBEST if required
            if (totalDanger(start, end, &particles[i], 1, danPt, m) <= gBest)
            {
                gBest = totalDanger(start, end, &particles[i], 1, danPt, m);
                if (distance(start, particles[i]) + distance(particles[i], end) < dist)
                {
                    gBCorner.x = particles[i].x;
                    gBCorner.y = particles[i].y;
                }
                //cout << gBCorner.x << " " << gBCorner.y << "\n";
            }//update GBEST if required
            //cout << totalDanger(start, end, &particles[i], 1, danPt, m) << "\n";
        }
        //cout << gBCorner.x << " " << gBCorner.y << "\n";
        
        we = we - we * 0.1;
        //update weight
        for (int i = 0; i < 3; i++)
        {
            //update velocity
            velocity.x = static_cast<int>(we * velocity.x + c1 * (pBCorner[i].x - particles[i].x) + c2 * (gBCorner.x - particles[i].x));
            velocity.y = static_cast<int>(we * velocity.y + c1 * (pBCorner[i].y - particles[i].y) + c2 * (gBCorner.y - particles[i].y));
            //update position
            if (islarger(start.x, end.x) == 1)
            {
                if (particles[i].x + velocity.x > start.x or particles[i].x + velocity.x < end.x)
                    continue;
            }
            else
            {
                if (particles[i].x + velocity.x < start.x or particles[i].x + velocity.x > end.x)
                    continue;
            }
            if (islarger(start.y, end.y) == 1)
            {
                if (particles[i].y + velocity.y > start.y or particles[i].y + velocity.y < end.y)
                    continue;
            }
            else
            {
                if (particles[i].y + velocity.y < start.y or particles[i].y + velocity.y > end.y)
                    continue;
            }
            particles[i].x = particles[i].x + velocity.x;
            particles[i].y = particles[i].y + velocity.y;
        }
        dT = 1000*clock()/CLOCKS_PER_SEC - curT;
        curT = 1000*clock()/CLOCKS_PER_SEC;
    }
    return gBCorner;
}

/*
// return the derivative of the danger on the point
double dDanger(point pt, point* danPt, int m, bool type)
{
    double deri = 0;
    for(int i = 0; i < m; i++)
    {
        if(distance(pt, danPt[i]) < danPt[i].radius)
        {
            double d = distance(pt, danPt[i]);
            if(d == 0)
            {
                d = 1;
            }
            if(type == true)
            {
                deri += -(danPt[i].danger)*(pt.x - danPt[i].x)/(danPt[i].radius*d);
            }
            else
            {
                deri += -(danPt[i].danger)*(pt.y - danPt[i].y)/(danPt[i].radius*d);
            }
        }
    }
    
    return deri;
}
*/

/*
// find a corner on the line
point findCorner(point from, point to, int n, point* danPt, int m, double maxD, int firstD)
{
    double slowDown = 0.95;
    double maxStep = 10000;
    double curStep = maxStep;
    double minStep = 1;
    
    double cost;
    
    
    point corner[1];
    if(from.x <= to.x)
    {
        corner[0].x = from.x + random(0, to.x-from.x);
    }
    else
    {
        corner[0].x = from.x - random(0, from.x-to.x);
    }
    if(from.y <= to.y)
    {
        corner[0].y = from.y + random(0, to.y-from.y);
    }
    else
    {
        corner[0].y = from.y - random(from.y-to.y, 0);
    }
    
    cout << corner[0].x << " " << corner[0].y << "\n";
    
    cost = totalDanger(from, to, corner, 1, danPt, m);
    
    //cout << "opt: " << corner[0].x << " " << corner[0].y << "\n--> " << cost << "\n\n";
    
    while(curStep > minStep)
    {
        //cout << corner[0].x<< "\n";
        
        double dx = dDanger(corner[0], danPt, m, true);
        double dy = dDanger(corner[0], danPt, m, false);
        
        //cout << "dx: " << dx << "\n";
        
        corner[0].x -= dx*(random(0,200)/100)*(curStep/maxStep);
        corner[0].y -= dy*(random(0,200)/100)*(curStep/maxStep);
        
        int cnt = 0;
        while(!islegal(from, to, corner, 1, n, maxD))
        {
            corner[0].x += dx;
            corner[0].y += dy;
            
            if(cnt > 200)
            {
                if(corner[0].x < 0)
                {
                    corner[0].x = random(0, n/2);
                }
                else if(corner[0].x > 0)
                {
                    corner[0].x = random(n/2, n);
                }
                
                if(corner[0].y < 0)
                {
                    corner[0].y = random(0, n/2);
                }
                else if(corner[0].y > 0)
                {
                    corner[0].y = random(n/2, n);
                }
                
                break;
            }
            
            dx *= 0.5;
            dy *= 0.5;
            
            corner[0].x -= dx;
            corner[0].y -= dy;
            cnt++;
        }
        
        //cout << corner[0].x << " " << corner[0].y << "\n";
        curStep *= slowDown;
    }
    
    //cout << "done\n";
    
    corner[0].x = static_cast<int>(corner[0].x);
    corner[0].y = static_cast<int>(corner[0].y);
    
    //cout << "done\n";
    
    return corner[0];
}
*/

// check if the path is legal(all the point is in the map and does not exceed the distance)
bool islegal(point start, point end, point corner[], int corCnt, int n, int d)
{
    for(int i = 0; i < corCnt; i++)
    {
        if(corner[i].x < 0 || corner[i].x > n || corner[i].y < 0 || corner[i].y > n)
        {
            //cout << "--out of map--\n";
            return false;
        }
    }
    
    double totalD = 0;
    totalD += distance(start, corner[0]);
    for(int i = 1; i < corCnt-1; i++)
    {
        totalD += distance(corner[i], corner[i+1]);
    }
    totalD += distance(corner[corCnt-1], end);
    
    if(totalD > d)
    {
        //cout << "--too far--\n";
        return false;
    }
    else
    {
        //cout << "--legal--\n";
        return true;
    }
}

// return the danger of a point
double pointDanger(point pt, point* danPt, int m)
{
    //cout << "--start\n";
    double ptDanger = 0;
    for(int i = 0; i < m; i++)
    {
        // if the position is in the influence radius of dangerous point, add the danger
        if(distance(pt, danPt[i]) < danPt[i].radius)
        {
            // danger (with one dangerous point) = (degree of danger) * [(radius) - (distance)] / (radius)
            ptDanger += danPt[i].danger * (danPt[i].radius - distance(pt, danPt[i])) / danPt[i].radius;
        }
    }
    //cout << "--done\n";
    return ptDanger;
}

// return the danger of a line
double lineDanger(point from, point to, point* danPt, int m, double firstD)
{
    //cout << "-start\n";
    double d = distance(from, to);
    
    //cout << "d = " << d << "\n";
    
    int checkCnt = 0;
    double lnDanger = 0;
    if(d >= 1)
    {
        checkCnt = static_cast<int>(d - firstD) + 1;
        
        /*
        cout << "d = " << d << "\n";
        cout << "firstD = " << firstD << "\n";
        cout << "cnt = " << checkCnt << "\n";
        */
        
        point* checkPt = new point [checkCnt];
        
        checkPt[0].init(from.x+(firstD/d)*(to.x - from.x), from.y+(firstD/d)*(to.y - from.y));
        for(int i = 1; i < checkCnt; i++)
        {
            checkPt[i].init(checkPt[i-1].x+(static_cast<double>(to.x - from.x)/d), checkPt[i-1].y+(static_cast<double>(to.y - from.y)/d));
            //cout << "(" << checkPt[i].x << ", " << checkPt[i].y << ") ";
        }
        
        //cout << "\n";
        
        for(int i = 0; i < checkCnt; i++)
        {
            lnDanger += pointDanger(checkPt[i], danPt, m);
        }
        
        delete [] checkPt;
        
    }
    
    
    return lnDanger;
}

// return the danger of a path
double totalDanger(point start, point end, point corner[], int corCnt, point* danPt, int m)
{
    //cout << "start\n";
    double ttDanger = 0;	
    ttDanger += lineDanger(start, corner[0], danPt, m);
    
    double d = distance(start, corner[0]);
    //cout << "d = " << d << "\n";
    double firstD = 1 - (d - static_cast<int>(d));
    //cout << "firstD = " << firstD << "\n";
    for(int i = 0; i < corCnt-1; i++)
    {
        ttDanger += lineDanger(corner[i], corner[i+1], danPt, m, firstD);
        d = distance(corner[i], corner[i+1]);
        firstD = 1 - (d - static_cast<int>(d));
        //cout << "firstD = " << firstD << "\n";
    }
    ttDanger += lineDanger(corner[corCnt-1], end, danPt, m, firstD);
    
    return ttDanger;
    
}
