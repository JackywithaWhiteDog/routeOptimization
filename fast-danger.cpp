#include <iostream> // cin, cout
#include <vector> // vector :: reserve(), insert(), puch_back()
#include <cmath> // hypot(), floor()
#include <ctime> // time(NULL), clock(), CLOCKS_PER_SEC
#include <cstdlib> // rand(), srand(), RAND_MAX
#include <algorithm>
using namespace std;

struct Point
{
    double x;
    double y;
    double r;
    double p;
    double distance(Point a) {return (hypot(x-a.x, y-a.y));}
    double danger();
};
 
// basic information
int n, m, w, d;
vector<Point> danPt;
Point s, e;


void inputInfo();

 
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    srand(time(NULL));
 
    inputInfo();

    for(unsigned int i=1001;--i;)
    {
        Point pt = {1.0*(rand()%(n+1)),1.0*(rand()%(n+1))};
        double dan = pt.danger();
    }
    return 0;
}
 
inline double Point::danger()
{
    register double Dan = 0;

    for(unsigned int i=0; i<m; i+=2)
    {
        Dan += max(danPt[i].p * (danPt[i].r - distance(danPt[i])) / danPt[i].r, 0.0);
        Dan += max(danPt[i+1].p * (danPt[i+1].r - distance(danPt[i+1])) / danPt[i+1].r, 0.0);

    }

    if((m & 1) != 0)
        Dan += max(danPt[m-1].p * (danPt[m-1].r - distance(danPt[m-1])) / danPt[m-1].r, 0.0);

    return Dan;
}
 
void inputInfo()
{
    cin >> n >> m >> w >> d;
 
    danPt.reserve(m);
    for(unsigned int i = 0; i < m; ++i)
    {
        cin >> danPt[i].x;
    }
    for(unsigned int i = 0; i < m; ++i)
    {
        cin >> danPt[i].y;
    }
    for(unsigned int i = 0; i < m; ++i)
    {
        cin >> danPt[i].r;
    }
    for(unsigned int i = 0; i < m; ++i)
    {
        cin >> danPt[i].p;
    }
 
    cin >> s.x >> s.y >> e.x >> e.y;
}