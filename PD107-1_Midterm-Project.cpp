#include <iostream> // cin, cout
#include <algorithm> // min(), max()
#include <cmath> // sqrt()
#include <cstdlib> // rand(), srand(), RAND_MAX
#include <ctime> // clock(), time(NULL)
#include <climits> // INT_MAX
using namespace std;

const int MAX_CORNER = 1000000;

class point
{
	public:
		double x, y;
		int radius, danger;
		init(double i, double j);
		init(double i, double j, int r, int p);
};

// return a random number from lower to upper
int random(int lower, int upper);

// input information
void getInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end);
// randomly create information
void randInfo(int& n, int& m, int& w, int& d, point*& danPt, point& start, point& end);

// return the distance of two point
double distance(point a, point b);

// return the derivative of the danger on the point(type == true -> /dx; type == false -> /dy)
double dDanger(point pt, point* danPt, int m, bool type);

// find a corner on the line
point findCorner(point from, point to, int n, point* danPt, int m, double maxD, int firstD = 1);



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
	
	int mapSize = 0, ptCnt = 0, weight = 0, distance = 0;
	point* danPt = nullptr;
	point start, end;

	/*
	getInfo(mapSize, ptCnt, weight, distance, danPt, start, end);
	/*/
	randInfo(mapSize, ptCnt, weight, distance, danPt, start, end);
	/**/
	
	double startT = 1000*clock()/CLOCKS_PER_SEC;
	double curT = 1000*clock()/CLOCKS_PER_SEC;
	double dt = 0;
	
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

	
	return 0;
}

point::init(double i, double j)
{
	x = i;
	y = j;
	radius = 0;
	danger = 0;
}
point::init(double i, double j, int r, int p)
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

// find a corner on the line
point findCorner(point from, point to, int n, point* danPt, int m, double maxD, int firstD)
{
	double slowDown = 0.95;
	double maxStep = 10000;
	double curStep = maxStep;
	double minStep = 1;
	
	double cost = 0, lcost = 0, rcost = 0, ucost = 0, dcost = 0;

	int scale = 1;

	point corner[1];
	if(from.x <= to.x)
		corner[0].x = from.x + random(0, to.x-from.x);
	else
		corner[0].x = from.x - random(0, from.x-to.x);
	if(from.y <= to.y)
		corner[0].y = from.y + random(0, to.y-from.y);
	else
		corner[0].y = from.y - random(from.y-to.y, 0);	

	cout << corner[0].x << " " << corner[0].y << "\n";
	cost = totalDanger(from, to, corner, 1, danPt, m);

	while(curStep > minStep)
	{
		point rCorner[1];
		point lCorner[1];
		point uCorner[1];
		point dCorner[1];
		rCorner[0].init(corner[0].x + scale, corner[0].y);
		lCorner[0].init(corner[0].x - scale, corner[0].y);
		uCorner[0].init(corner[0].x, corner[0].y + scale);
		dCorner[0].init(corner[0].x, corner[0].y - scale);
		if(from.x <= to.x)
		{
			if(rCorner[0].x > to.x)
				rCorner[0].x = -1;
			if(lCorner[0].x < from.x)
				lCorner[0].x = -1;
		}
		else
		{
			if(rCorner[0].x > from.x)
				rCorner[0].x = -1;
			if(lCorner[0].x < to.x)
				lCorner[0].x = -1;
		}
		if(from.y <= to.y)
		{
			if(uCorner[0].y > to.y)
				uCorner[0].y = -1;
			if(dCorner[0].y < from.y)
				dCorner[0].y = -1;
		}
		else
		{
			if(uCorner[0].y > from.y)
				uCorner[0].y = -1;
			if(dCorner[0].y < to.y)
				dCorner[0].y = -1;
		}
	
		cost = totalDanger(from, to, corner, 1, danPt, m);
		rcost = totalDanger(from, to, rCorner, 1, danPt, m);
		lcost = totalDanger(from, to, lCorner, 1, danPt, m);
		ucost = totalDanger(from, to, uCorner, 1, danPt, m);
		dcost = totalDanger(from, to, dCorner, 1, danPt, m);

		double minCost = cost;
		if((rcost < minCost) || (rcost == minCost && abs(dDanger(rCorner[0], danPt, m, true)) < abs(dDanger(corner[0], danPt, m, true))))
		{
			corner[0] = rCorner[0];
			minCost = rcost;
		}
		if((lcost < minCost) || (lcost == minCost && abs(dDanger(lCorner[0], danPt, m, true)) < abs(dDanger(corner[0], danPt, m, true))))
		{
			corner[0] = lCorner[0];
			minCost = lcost;
		}
		if((ucost < minCost) || (ucost == minCost && abs(dDanger(uCorner[0], danPt, m, false)) < abs(dDanger(corner[0], danPt, m, false))))
		{
			corner[0] = uCorner[0];
			minCost = ucost;
		}
		if((dcost < minCost) || (dcost == minCost && abs(dDanger(dCorner[0], danPt, m, false)) < abs(dDanger(corner[0], danPt, m, false))))
		{
			corner[0] = dCorner[0];
			minCost = dcost;
		}

		if(minCost != cost)
			scale = 1;
		else
			scale++;

		if(scale > 3)
			break;
	}
	
	//cout << "opt: " << corner[0].x << " " << corner[0].y << "\n--> " << cost << "\n\n";
	
	corner[0].x = static_cast<int>(corner[0].x);
	corner[0].y = static_cast<int>(corner[0].y);
	
	//cout << "done\n";
	
	return corner[0];
}


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
		
		//cout << "cnt = " << checkCnt << "\n";
	
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

