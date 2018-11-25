#include <iostream> // cin, cout
#include <algorithm> // min(), max()
#include <cmath> // sqrt()
#include <cstdlib> // rand(), srand(), RAND_MAX
#include <ctime> // clock(), time(NULL)
#include <climits> // INT_MAX
using namespace std;

const int MAX_CORNER = 2;
const double MAX_T = 800;

class Point
{
	public:
		double x, y;
		int radius, danger;
		void init(double i, double j);
		void init(double i, double j, int r, int p);
};

// return a random number from lower to upper
int random(int lower, int upper);

// input information
void getInfo(int& n, int& m, int& w, int& d, Point*& danPt, Point& start, Point& end);
// randomly create information
void randInfo(int& n, int& m, int& w, int& d, Point*& danPt, Point& start, Point& end);

// return the distance of two Point
double distance(Point a, Point b);

// return the derivative of the danger on the Point(type == true -> /dx; type == false -> /dy)
double dDanger(Point pt, Point* danPt, int m, bool type);

// find a corner on the line
Point findCorner(Point from, Point to, int n, Point* danPt, int m, double maxD, double firstD = 1);

Point optCorner(Point from, Point to, int n, Point* danPt, int m, double maxD, double maxT, double firstD = 1);

// find a optimal path, return the cost
double findPath(Point sol[], int& solCnt, Point from, Point to, int n, Point* danPt, int m, int weight, double maxD, double maxT);

// check if the path is legal(all the Point is in the map and does not exceed the distance)
bool islegal(Point start, Point end, Point corner[], int corCnt, int n, int d);

// return the danger of a Point
double pointDanger(Point pt, Point* danPt, int m);
// return the danger of a line
double lineDanger(Point from, Point to, Point* danPt, int m, double firstD = 1);
// return the danger of a path
double totalDanger(Point start, Point end, Point corner[], int corCnt, Point* danPt, int m, double firstD = 1);

int main()
{
	srand(time(NULL));
	
	int mapSize = 0, ptCnt = 0, weight = 0, distance = 0;
	Point* danPt = nullptr;
	Point start, end;

	//*
	getInfo(mapSize, ptCnt, weight, distance, danPt, start, end);
	/*/
	randInfo(mapSize, ptCnt, weight, distance, danPt, start, end);
	/**/
	
	double startT = 1000*clock()/CLOCKS_PER_SEC;
	double curT = 1000*clock()/CLOCKS_PER_SEC;
	double dt = 0;
	
	Point sol[MAX_CORNER];
	double optCost = lineDanger(start, end, danPt, ptCnt);
	int optCnt = 0;
	
	int cnt = 0;
	while(curT-startT+dt <= MAX_T)
	{
		//cout << "loop-" << cnt+1 << "\n";
		Point temp[MAX_CORNER];
		int tempCnt;
		double cost = findPath(temp, tempCnt, start, end, mapSize, danPt, ptCnt, weight, distance, MAX_T-(curT-startT));
		
		if(cost < optCost)
		{
			optCost = cost;
			for(int i = 0; i < tempCnt; i++)
			{
				sol[i] = temp[i];
			}
			optCnt = tempCnt;
		}
		
		cnt++;
		dt = 1000*clock()/CLOCKS_PER_SEC - curT;
		curT = 1000*clock()/CLOCKS_PER_SEC;
	}
	
	//*
	cout << optCnt;
	for(int i = 0; i < optCnt; i++)
	{
		cout << " " << sol[i].x << " " << sol[i].y;
	}
		
	/*/	
	cout << "\n<information>\n";
	cout << mapSize << " " << ptCnt << " " << weight << " " << distance << "\n";
	cout << start.x << " " << start.y << " " << end.x << " " << end.y << "\n";
	
	cout << "\noutput\n";
	cout << optCnt << "\n";
	for(int i = 0; i < optCnt; i++)
	{
		cout << sol[i].x << " " << sol[i].y << "\n";
	}
	cout << "cost: " << optCost << "\n\n";
	
	double naiveCost = lineDanger(start, end, danPt, ptCnt);
	cout << "naive cost: " << naiveCost << "\n\n";
	if(optCost < naiveCost)
	{
		cout << "SUCCESS!!!\n\n";
	}
	else
	{
		cout << "FAIL...\n\n";
	}
	
	curT = 1000*clock()/CLOCKS_PER_SEC;
	cout << "time: " << curT - startT << "\n";
	cout << "loop: " << cnt << "\n";
	/**/

	return 0;
}

void Point::init(double i, double j)
{
	x = i;
	y = j;
	radius = 0;
	danger = 0;
}
void Point::init(double i, double j, int r, int p)
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
void getInfo(int& n, int& m, int& w, int& d, Point*& danPt, Point& start, Point& end)
{
	cin >> n >> m >> w >> d;
	
	danPt = new Point [m];
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
void randInfo(int& n, int& m, int& w, int& d, Point*& danPt, Point& start, Point& end)
{
	n = random(1, 1000);
	m = random(1, min((n+1)*(n+1), 10000));
	w = random(0, 1000);

	danPt = new Point [m];
	
	for(int i = 0; i < m; i++)
	{
		danPt[i].init(random(0,n), random(0,n), random(0,n), random(1,1000));
	}
	
	start.init(random(0,n), random(0,n));
	end.init(random(0,n), random(0,n));
	
	d = random(max(n, static_cast<int>(distance(start, end))+1), min(10*n, static_cast<int>(distance(start, end))+1)); 
}

// return the distance of two Point
double distance(Point a, Point b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

// return the derivative of the danger on the Point
double dDanger(Point pt, Point* danPt, int m, bool type)
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
Point findCorner(Point from, Point to, int n, Point* danPt, int m, double maxD, double firstD)
{
	double slowDown = 0.9;
	double maxStep = 1000;
	double curStep = maxStep;
	double minStep = 1;

	Point corner[1];
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
		corner[0].y = from.y - random(0, from.y-to.y);
	}
	
	//cout << corner[0].x << " " << corner[0].y << "\n";
	
	//cout << "opt: " << corner[0].x << " " << corner[0].y << "\n--> " << cost << "\n\n";

	while(curStep > minStep)
	{
		//cout << corner[0].x<< "\n";
		
		double dx = dDanger(corner[0], danPt, m, true);
		double dy = dDanger(corner[0], danPt, m, false);
		
		//cout << "dx: " << dx << "\n";
		
		if(dx == 0)
		{
			if(random(0,1))
			{
				corner[0].x -= (random(-200,200)/100)*(curStep/maxStep);
			}
		}
		else
		{
			corner[0].x -= (random(0,200)/100)*(curStep/maxStep)/dx;
		}
		if(dy == 0)
		{
			if(random(0,1))
			{
				corner[0].y -= (random(-200,200)/100)*(curStep/maxStep);
			}
		}
		else
		{
			corner[0].y -= (random(0,200)/100)*(curStep/maxStep)/dy;
		}
		
		
		
		int cnt = 0;
		while(!islegal(from, to, corner, 1, n, maxD))
		{	
			if(cnt > 50)
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
		
			corner[0].x += dx;
			corner[0].y += dy;
			
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

Point optCorner(Point from, Point to, int n, Point* danPt, int m, double maxD, double maxT, double firstD)
{
	double startT = 1000*clock()/CLOCKS_PER_SEC;
	double curT = 1000*clock()/CLOCKS_PER_SEC;
	double dt = 0;
	
	Point ans[1];
	double optCost = INT_MAX;
	
	//cout << "opt\n";
	
	int cnt = 0;
	while(curT-startT+dt <= maxT && cnt<=100)
	{
		Point temp[1];
		temp[0] = findCorner(from, to, n, danPt, m, maxD, firstD);
		//cout << "-done\n";
		double cost = totalDanger(from, to, temp, 1, danPt, m, firstD);
		//cout << "-done\n";
		//cout << temp[0].x << " " << temp[0].y << " --> " << cost << "\n";
		
		if(cost < optCost)
		{
			optCost = cost;
			ans[0] = temp[0];
		}
		
		dt = 1000*clock()/CLOCKS_PER_SEC - curT;
		curT = 1000*clock()/CLOCKS_PER_SEC;
		cnt++;
	}

	return ans[0];
}


// find a optimal path, return the cost
double findPath(Point sol[], int& solCnt, Point from, Point to, int n, Point* danPt, int m, int weight, double maxD, double maxT)
{
	//cout << ";\n";
	double startT = 1000*clock()/CLOCKS_PER_SEC;
	double curT = 1000*clock()/CLOCKS_PER_SEC;
	double dt = 0;

	Point** tempSol = new Point* [MAX_CORNER];
	for(int i = 0; i < MAX_CORNER; i++)
	{
		tempSol[i] = new Point [i+1];
	}
	int cnt = 1;

	tempSol[0][0] = optCorner(from, to, n, danPt, m, maxD, (maxT-(curT-startT))/(1.0*MAX_CORNER*MAX_CORNER*(solCnt+1)));
	//cout << tempSol[0][0].x << " " << tempSol[0][0].y;
	
	//cout << "done\n";
	while(curT+dt <= maxT && cnt <= MAX_CORNER)
	{
		//cout << "while\n";
		// total distance
		double ttD = distance(from, tempSol[cnt-1][0]);
		for(int i = 0; i < cnt-1; i++)
		{
			ttD += distance(tempSol[cnt-1][i], tempSol[cnt-1][i+1]);
		}
		ttD += distance(tempSol[cnt-1][cnt-1], to);

		int position = 0;

		double d = distance(from, tempSol[cnt-1][0]);
		Point optCor[1];
		optCor[0] = optCorner(from, tempSol[cnt-1][0], n, danPt, m, d+maxD-ttD, (maxT-(curT-startT))/(MAX_CORNER*MAX_CORNER*(cnt+1)));
		double optDcost = totalDanger(from, tempSol[cnt-1][0], optCor, 1, danPt, m) - lineDanger(from, tempSol[cnt-1][0], danPt, m);
		position = 0;
		
		double firstD = 1 - (d - static_cast<int>(d));
		//cout << "-done\n";
		Point tempCor[1];
		double tempDcost;
		for(int i = 1; i < cnt; i++)
		{
			d = distance(tempSol[cnt-1][i-1], tempSol[cnt-1][i]);
			//cout << ".\n";
			//cout << "cnt=" << cnt << "\ni=" << i << "\n";
			tempCor[0] = optCorner(tempSol[cnt-1][i-1], tempSol[cnt-1][i], n, danPt, m,  d+maxD-ttD, (maxT-(curT-startT))/(MAX_CORNER*MAX_CORNER*(cnt+1)), firstD);
			//cout << "..\n";
			tempDcost = totalDanger(tempSol[cnt-1][i-1], tempSol[cnt-1][i], tempCor, 1, danPt, m, firstD) - lineDanger(tempSol[cnt-1][i-1], tempSol[cnt-1][i], danPt, m, firstD);
			//cout << "...\n";
			if(tempDcost < optDcost)
			{
				optDcost = tempDcost;
				optCor[0] = tempCor[0];
				position = i;
			}
			//cout << "....\n";
			firstD = 1 - (d - static_cast<int>(d));
		}
		//cout << "--done\n";
		d = distance(tempSol[cnt-1][cnt-1], to);
		tempCor[0] = optCorner(tempSol[cnt-1][cnt-1], to, n, danPt, m,  d+maxD-ttD, (maxT-(curT-startT))/(MAX_CORNER*MAX_CORNER*(cnt+1)), firstD);
		tempDcost = totalDanger(tempSol[cnt-1][cnt-1], to, tempCor, 1, danPt, m, firstD) - lineDanger(tempSol[cnt-1][cnt-1], to, danPt, m, firstD);
		if(tempDcost < optDcost)
		{
			optDcost = tempDcost;
			optCor[0] = tempCor[0];
			position = cnt;
		}
		
		
		//cout << "---done\n";
		// update
		for(int i = 0; i < position; i++)
		{
			tempSol[cnt][i] = tempSol[cnt-1][i];
		}
		tempSol[cnt][position] = optCor[0];
		for(int i = position+1; i < cnt; i++)
		{
			tempSol[cnt][i] = tempSol[cnt-1][i-1];
		}
		
		cnt++;
		
		dt = 1000*clock()/CLOCKS_PER_SEC - curT;
		curT = 1000*clock()/CLOCKS_PER_SEC;
	}

	double optCost = lineDanger(from, to, danPt, m);
	int optCnt = -1;
	for(int i = 0; i < cnt; i++)
	{
		double cost = totalDanger(from, to, tempSol[i], i+1, danPt, m) + weight*(cnt+1);
		//cout << cost << "\n";
		if(cost < optCost)
		{
			optCost = cost;
			optCnt = i;
		}
	}
	
	for(int i = 0; i <= optCnt; i++)
	{
		sol[i] = tempSol[optCnt][i];
	}
	solCnt = optCnt+1;
	
	for(int i = 0; i < MAX_CORNER; i++)
	{
		delete [] tempSol[i];
	}
	delete [] tempSol;

	return optCost;
}

// check if the path is legal(all the Point is in the map and does not exceed the distance)
bool islegal(Point start, Point end, Point corner[], int corCnt, int n, int d)
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

// return the danger of a Point
double pointDanger(Point pt, Point* danPt, int m)
{
	//cout << "--start\n";
	double ptDanger = 0;
	for(int i = 0; i < m; i++)
	{
		// if the position is in the influence radius of dangerous Point, add the danger
		if(distance(pt, danPt[i]) < danPt[i].radius)
		{
			// danger (with one dangerous Point) = (degree of danger) * [(radius) - (distance)] / (radius)
			ptDanger += danPt[i].danger * (danPt[i].radius - distance(pt, danPt[i])) / danPt[i].radius;
		}
	}
	//cout << "--done\n";
	return ptDanger;
}

// return the danger of a line
double lineDanger(Point from, Point to, Point* danPt, int m, double firstD)
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
	
		Point* checkPt = new Point [checkCnt];
	
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
double totalDanger(Point start, Point end, Point corner[], int corCnt, Point* danPt, int m, double firstD)
{
	//cout << "start\n";
	double ttDanger = 0;	
	ttDanger += lineDanger(start, corner[0], danPt, m, firstD);
	
	double d = distance(start, corner[0]);
	//cout << "d = " << d << "\n";
	d -= firstD;
	double fD = 1 - (d - static_cast<int>(d));
	//cout << "firstD = " << firstD << "\n";
	for(int i = 0; i < corCnt-1; i++)
	{
		ttDanger += lineDanger(corner[i], corner[i+1], danPt, m, fD);
		d = distance(corner[i], corner[i+1]);
		d -= firstD;
		firstD = 1 - (d - static_cast<int>(d));
		//cout << "firstD = " << firstD << "\n";
	}
	ttDanger += lineDanger(corner[corCnt-1], end, danPt, m, fD);

	return ttDanger;
	
}

