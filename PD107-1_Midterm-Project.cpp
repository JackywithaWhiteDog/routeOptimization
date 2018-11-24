#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

class point
{
	public:
		float x, y;
		int radius, danger;
		void init(int i, int j);
		void init(int i, int j, int r, int p);
};

// return a random number from lower to upper
int random(int lower, int upper);

// input information
void getInfo(int& n, int& m, int& w, int& d, point* danPt, point& start, point& end);
// randomly create information
void randInfo(int& n, int& m, int& w, int& d, point* danPt, point& start, point& end);

// return the distance of two point
float distance(point a, point b);

// check if the path is legal(all the point is in the map and does not exceed the distance)
bool islegal(point start, point end, point* corner, int corCnt, int n, int d);

// return the danger of a point
float pointDanger(point pt, point* danPt, int m);
// return the danger of a line
float lineDanger(point from, point to, point* danPt, int m, float firstD = 1);
// return the danger of a path
float totalDanger(point start, point end, point* corner, int corCnt, point* danPt, int m);

int main()
{
	srand(time(NULL));
	clock_t startTime = clock();
	
	int mapSize = 0, ptCnt = 0, weight = 0, distance = 0;
	point* danPt = nullptr;
	point start, end;
	
	return 0;
}

void point::init(int i, int j)
{
	x = i;
	y = j;
	radius = 0;
	danger = 0;
}
void point::init(int i, int j, int r, int p)
{
	x = i;
	y = j;
	radius = r;
	danger = p;
}

// return a random number from lower to upper
int random(int lower, int upper)
{
	return static_cast<int>((upper-lower)*static_cast<float>(rand())/RAND_MAX+lower);
}

// input information
void getInfo(int& n, int& m, int& w, int& d, point* danPt, point& start, point& end)
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
void randInfo(int& n, int& m, int& w, int& d, point* danPt, point& start, point& end)
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
float distance(point a, point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// check if the path is legal(all the point is in the map and does not exceed the distance)
bool islegal(point start, point end, point* corner, int corCnt, int n, int d)
{
	for(int i = 0; i < corCnt; i++)
	{
		if(corner[i].x < 0 || corner[i].x > n || corner[i].y < 0 || corner[i].y > n)
		{
			return false;
		}
	}
	
	float totalD = 0;
	totalD += distance(start, corner[0]);
	for(int i = 1; i < corCnt-1; i++)
	{
		totalD += distance(corner[i], corner[i+1]);
	}
	totalD += distance(corner[corCnt-1], end);
	
	if(totalD > d)
	{
		return false;
	}
	
	return true;
}

// return the danger of a point
float pointDanger(point pt, point* danPt, int m)
{
	float ptDanger = 0;
	for(int i = 0; i < m; i++)
	{
		// if the position is in the influence radius of dangerous point, add the danger
		if(distance(pt, danPt[i]) < danPt[i].radius)
		{
			// danger (with one dangerous point) = (degree of danger) * [(radius) - (distance)] / (radius)
			ptDanger += danPt[i].danger * (danPt[i].radius - distance(pt, danPt[i])) / danPt[i].radius;
		}
	}
	return ptDanger;
}

// return the danger of a line
float lineDanger(point from, point to, point* danPt, int m, float firstD)
{
	float d = distance(from, to);
	int checkCnt = static_cast<int>(d - firstD) + 1;

	point* checkPt = new point [checkCnt];
	
	checkPt[0].init(from.x+(firstD/d)*(to.x - from.x), from.y+(firstD/d)*(to.y - from.y));
	for(int i = 1; i < checkCnt; i++)
	{
		checkPt[i].init(checkPt[i-1].x+(to.x - from.x)/d, checkPt[i-1].y+(to.y - from.y)/d);
	}
	
	float lnDanger = 0;
	for(int i = 0; i < checkCnt; i++)
	{
		lnDanger += pointDanger(checkPt[i], danPt, m);
	}
	
	delete [] checkPt;
	
	return lnDanger;
}

// return the danger of a path
float totalDanger(point start, point end, point* corner, int corCnt, point* danPt, int m)
{
	float ttDanger = 0;	
	ttDanger += lineDanger(start, corner[0], danPt, m);
	
	float d = distance(start, corner[0]);
	float firstD = 1 - (d - static_cast<int>(d));
	for(int i = 0; i < corCnt-1; i++)
	{
		ttDanger += lineDanger(corner[i], corner[i+1], danPt, m, firstD);
		d = distance(corner[i], corner[i+1]);
		firstD = 1 - (d - static_cast<int>(d));
	}
	ttDanger += lineDanger(corner[corCnt-1], end, danPt, m, firstD);
	
	return ttDanger;
	
}

