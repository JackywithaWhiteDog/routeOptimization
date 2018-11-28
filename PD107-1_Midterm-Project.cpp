#include <iostream> // cin, cout
#include <vector> // vector :: reserve(), insert(), puch_back()
#include <algorithm> // sort()
#include <cmath> // hypot(), floor()
#include <ctime> // time(NULL), clock(), CLOCKS_PER_SEC
#include <cstdlib> // rand(), srand(), RAND_MAX
using namespace std;

struct Point
{
	double x;
	double y;
	double r;
	double p;
	double distance(Point a) {return (hypot(x-a.x, y-a.y));}
	double danger();
	void dDanger(double& dx, double& dy);
};

const int MAX_CORNER = 5;
const int MAP_RES = 300, DAN_RES = 3000;
const double TIME_LIMIT = 950;
const double MAX_TEMP = 1000000, MIN_TEMP = 13500, COOL_DOWN = 0.98;

// basic information
int n, m, w, d;
vector<Point> danPt;
Point s, e;

int unit;

vector<double> curDdanger;
vector<double> curDx;
vector<double> curDy;
int lastChoice = 0;

vector<Point> solution;
vector<Point> nowBest;

void inputInfo();
void showInfo();
void outputSol();
bool ptCmpX(Point a, Point b) { return (a.x < b.x); }
bool intCmpX(Point a, int b) { return (a.x < b); }

void findPath(double maxTime);

int selectLine(int cnt);
double lnDdanger(Point from, Point to, double firstD, double& dx, double& dy); // return dx^2 + dy^2

Point findCorner(Point from, Point to, double firstD, double maxTime); // search for best corner on a line

void update(); // update nowBest

double lnDanger(Point from, Point to, double firstD);
double ttDanger(vector<Point> corner);

int main()
{
	srand(time(NULL));

	inputInfo();
	// showInfo(); // for check
	
	//double startTime = 1000*clock()/CLOCKS_PER_SEC;
	double curTime = 1000*clock()/CLOCKS_PER_SEC;

	while(curTime < TIME_LIMIT)
	{
		findPath(TIME_LIMIT - curTime);
		curTime = 1000*clock()/CLOCKS_PER_SEC;
	}

	outputSol();

	return 0;
}

double Point::danger()
{
	double dan = 0;
	int center = lower_bound(danPt.begin(), danPt.end(), x, intCmpX) - danPt.begin();

	for(int i = center-DAN_RES; i <= center+DAN_RES; i++)	
		if(i >= 0 && i < m)
			if(distance(danPt[i]) < danPt[i].r)
				dan += danPt[i].p * (danPt[i].r - distance(danPt[i])) / danPt[i].r;

	return dan;
}

void Point::dDanger(double& dx, double& dy)
{
	int center = lower_bound(danPt.begin(), danPt.end(), x, intCmpX) - danPt.begin();

	for(int i = center-DAN_RES; i <= center+DAN_RES; i++)	
		if(i >= 0 && i < m)
			if(distance(danPt[i]) < danPt[i].r)
			{
				dx += danPt[i].p * (- (x - danPt[i].x)/distance(danPt[i])) / danPt[i].r;
				dy += danPt[i].p * (- (y - danPt[i].y)/distance(danPt[i])) / danPt[i].r;
			}
}

void inputInfo()
{
	cin >> n >> m >> w >> d;

	danPt.reserve(m);
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
		cin >> danPt[i].r;
	}
	for(int i = 0; i < m; i++)
	{
		cin >> danPt[i].p;
	}

	cin >> s.x >> s.y >> e.x >> e.y;

	unit = n/MAP_RES + 1;
	sort(danPt.begin(), danPt.end(), ptCmpX);
}

void showInfo()
{
	cout << "\n<check_start>\n";
	cout << n << " " << m << " " << w << " " << d << "\n";
	for(int i = 0; i < m; i++)
		cout << danPt[i].x << " " << danPt[i].y << " " << danPt[i].r << " " << danPt[i].p << "\n";
	cout << s.x << " " << s.y << " " << e.x << " " << e.y;
	cout << "\n<check_complete>\n\n";
}

void outputSol()
{
	cout << nowBest.size();
	for(unsigned int i = 0; i < nowBest.size(); i++)
		cout << " " << nowBest[i].x << " "  << nowBest[i].y;
}

void findPath(double maxTime)
{
	double startTime = 1000*clock()/CLOCKS_PER_SEC;
	double dTime = 0;
	int cnt = 0;
	double curTime = 1000*clock()/CLOCKS_PER_SEC;
	double onceTime;
	while(cnt <= MAX_CORNER && (curTime + dTime) - startTime < maxTime)
	{
		//cout << "\ncnt = " << cnt << "\n";
		if(cnt == 0)
		{
			curDx.assign(1, 0);
			curDy.assign(1, 0);
			lnDdanger(s, e, unit, curDx[0], curDy[0]);
			double now = 1000*clock()/CLOCKS_PER_SEC;
			onceTime = (maxTime-now)/(MAX_CORNER-cnt);
			Point pt = findCorner(s, e, unit, onceTime);
			if(pt.x >= 0 && pt.y >= 0)
				solution.insert(solution.begin(), pt);
			else
				break;
		}
		else
		{
			int choice = selectLine(cnt);
			//cout << "choice = " << choice << "\n";
			if(choice == 0)
			{
				double now = 1000*clock()/CLOCKS_PER_SEC;
				onceTime = (maxTime-now)/(MAX_CORNER-cnt);
				Point pt = findCorner(s, solution[0], unit, onceTime);
				if(pt.x >= 0 && pt.y >= 0)
					solution.insert(solution.begin(), pt);
				else
					break;
			}
			else if(choice == cnt)
			{
				double firstD = unit*(static_cast<int>(solution[cnt-2].distance(solution[cnt-1]))/unit + 1) - solution[cnt-2].distance(solution[cnt-1]);
				double now = 1000*clock()/CLOCKS_PER_SEC;
				onceTime = (maxTime-now)/(MAX_CORNER-cnt);
				Point pt = findCorner(solution[cnt-1], e, firstD, onceTime);
				if(pt.x >= 0 && pt.y >= 0)
					solution.push_back(pt);
				else
					break;
			}
			else
			{
				double firstD = unit*(static_cast<int>(solution[choice-2].distance(solution[choice-1]))/unit + 1) - solution[choice-2].distance(solution[choice-1]);
				double now = 1000*clock()/CLOCKS_PER_SEC;
				onceTime = (maxTime-now)/(MAX_CORNER-cnt);
				Point pt = findCorner(solution[choice-1], solution[choice], firstD, onceTime);
				if(pt.x >= 0 && pt.y >= 0)
					solution.insert(solution.begin()+choice, pt);
				else
					break;
			}
		}
		
		update();
		cnt++;
	}

		cnt++;
		dTime = 1000*clock()/CLOCKS_PER_SEC - curTime;
		curTime = 1000*clock()/CLOCKS_PER_SEC;
}

int selectLine(int cnt)
{
	double tempDx[2] = {0}, tempDy[2] = {0};

	double firstD;
	if(cnt == 1)
	{
		curDdanger.reserve(2);
		if(s.distance(solution[0]) > unit)
		{
			curDdanger[0] = lnDdanger(s, solution[0], unit, tempDx[0], tempDy[0]);
			firstD = unit*(static_cast<int>(s.distance(solution[0]))/unit + 1) - s.distance(solution[0]);
		}
		else
		{
			curDdanger[0] = 0;
			firstD = unit - s.distance(solution[0]);
		}
		
		if(solution[0].distance(e) > firstD)
			curDdanger[1] = lnDdanger(solution[0], e, firstD, tempDx[1], tempDy[1]);
		else
			curDdanger[1] = 0;
	}
	else
	{
		//cout << "lastChoice: " << lastChoice << "\n" ;
		switch(lastChoice)
		{
			case 0:
			{
				if(s.distance(solution[0]) > unit)
					curDdanger[0] = lnDdanger(s, solution[0], unit, tempDx[0], tempDy[0]);
				else
					curDdanger[0] = 0;
				//cout << "done\n";
				break;
			}
			case 1:
			{
				firstD = unit*(static_cast<int>(s.distance(solution[0]))/unit + 1) - s.distance(solution[0]);
				//cout << "firstD: " << firstD << "\n";
				//cout << "solution[0] ~ solution[1]: " << solution[0].distance(solution[1]) << "\n";
				if(solution[0].distance(solution[1]) > firstD)
					curDdanger[1] = lnDdanger(solution[0], solution[1], firstD, tempDx[0], tempDy[0]);
				else
					curDdanger[1] = 0;
				//cout << "done\n";
				break;
			}
			default:
			{
				firstD = unit*(static_cast<int>(solution[lastChoice-2].distance(solution[lastChoice-1]))/unit + 1) - solution[lastChoice-2].distance(solution[lastChoice-1]);
				if(solution[lastChoice-1].distance(solution[lastChoice]) > firstD)
					curDdanger[lastChoice] = lnDdanger(solution[lastChoice-1], solution[lastChoice], firstD, tempDx[0], tempDy[0]);
				else
					curDdanger[lastChoice] = 0;
				//cout << "done\n";
				break;
			}
		}


		if(lastChoice == 0)
		{
			firstD = unit*(static_cast<int>(s.distance(solution[0]))/unit + 1) - s.distance(solution[0]);
			if(solution[0].distance(solution[1]) > firstD)
				curDdanger.insert(curDdanger.begin()+1, lnDdanger(solution[0], solution[1], firstD, tempDx[1], tempDy[1]));
			else
				curDdanger.insert(curDdanger.begin()+1, 0);
			//cout << "done\n";
		}
		else if(lastChoice == cnt-1)
		{
			firstD = unit*(static_cast<int>(solution[cnt-2].distance(solution[cnt-1]))/unit + 1) - solution[cnt-2].distance(solution[cnt-1]);
			if(solution[cnt-1].distance(e) > firstD)
				curDdanger.push_back(lnDdanger(solution[cnt-1], e, firstD, tempDx[1], tempDy[1]));
			else
				curDdanger.push_back(0);
			//cout << "done\n";
		}
		else
		{
			firstD = unit*(static_cast<int>(solution[lastChoice-1].distance(solution[lastChoice]))/unit + 1) - solution[lastChoice-1].distance(solution[lastChoice]);
			if(solution[lastChoice].distance(solution[lastChoice+1]) > firstD)
			{
				//cout << "do\n";
				curDdanger.insert(curDdanger.begin()+(lastChoice+1), lnDdanger(solution[lastChoice], solution[lastChoice+1], firstD, tempDx[1], tempDy[1]));
			}
			else
			{
				//cout << "dont\n";
				curDdanger.insert(curDdanger.begin()+(lastChoice+1), 0);
			}
			//cout << "done\n";
		}
	}

	curDx.insert(curDx.begin()+lastChoice, tempDx[0]);
	curDx.insert(curDx.begin()+lastChoice+1, tempDx[1]);
	curDy.insert(curDy.begin()+lastChoice, tempDy[0]);
	curDy.insert(curDy.begin()+lastChoice+1, tempDy[1]);

	cnt++;

	int choice = 0;
	double maxDD = 0;
	for(int i = 0; i < cnt; i++)
	{
		if(curDdanger[i] > maxDD)
		{
			choice = i;
			maxDD = curDdanger[i];
		}
	}

	lastChoice = choice;
	return choice;
}

double lnDdanger(Point from, Point to, double firstD, double& dx, double& dy) // return dx^2 + dy^2
{
	double d = from.distance(to);
	int checkCnt = floor((d-firstD)/unit);

	Point checkPt;
	if(firstD != 0)
	{
		checkPt = {from.x + (to.x-from.x)*firstD/d, from.y + (to.y-from.y)*firstD/d};
		if(checkPt.x != e.x || checkPt.y != e.y)
			checkPt.dDanger(dx, dy);
	}

	for(int i = 1; i < checkCnt; i++)
	{
		checkPt = {checkPt.x + (to.x-from.x)*unit/d, checkPt.y + (to.y-from.y)*unit/d};

		if(checkPt.x == e.x && checkPt.y == e.y)
			break;
		else
			checkPt.dDanger(dx, dy);
	}
	
	return hypot(dx, dy);
}

Point findCorner(Point from, Point to, double firstD, double maxTime) // search for best corner on a line
{
	//cout << "<findCorner>\n";
	//cout << from.x << " " << from.y << " " << to.x << " " << to.y << "\n";
	double startTime = 1000*clock()/CLOCKS_PER_SEC;
	double dTime = 0;
	double temper = MAX_TEMP;
	
	double dist = from.distance(to);
	
	double newX;
	double newY;
	bool isNew = true;
	
	Point corner;
	int loopCnt = 0;
	do
	{
		loopCnt++;
		//cout << "dowhile: " << loopCnt+1 << "\n";
		newX = floor(from.x + (to.x-from.x)*(1.0*rand()/RAND_MAX) - loopCnt*unit*curDx[lastChoice]+0.5);
		newY = floor(from.y + (to.y-from.y)*(1.0*rand()/RAND_MAX) - loopCnt*unit*curDy[lastChoice]+0.5);

		if(newX < 0)
			newX = 0;
		else if(newX > n)
			newX = n;
		if(newY < 0)
			newY = 0;
		else if(newY > n)
			newY = n;
		
		if((newX == s.x && newY == s.y) || (newX == e.x && newY == e.y))
			isNew = false;
		if(isNew)
		{
			for(unsigned int i = 0; i < solution.size(); i++)
				if(newX == solution[i].x && newY == solution[i].y)
				{
					isNew = false;
					break;
				}
		}
		
		if(loopCnt > 10000)
		{
			corner = {-1, -1};
			return corner;
		}
		
	}while(!isNew);

	
	corner = {newX, newY};

	double curTime = 1000*clock()/CLOCKS_PER_SEC;
	while(temper > MIN_TEMP && (curTime-startTime) + dTime < maxTime)
	{
		double dx = 0, dy = 0;
		lnDdanger(from, corner, firstD, dx, dy);
		firstD = unit*(static_cast<int>(from.distance(corner)/unit + 1)) - from.distance(corner);
		lnDdanger(corner, to, firstD, dx, dy);

		newX = floor(corner.x - unit*dx*(1.0*rand()/RAND_MAX)*(temper/MAX_TEMP)*dist*0.5 + 0.5);
		newY = floor(corner.y - unit*dy*(1.0*rand()/RAND_MAX)*(temper/MAX_TEMP)*dist*0.5 + 0.5);
		
		if(newX < 0)
			newX = 0;
		else if(newX > n)
			newX = n;
		if(newY < 0)
			newY = 0;
		else if(newY > n)
			newY = n;
		
		isNew = true;
		if((newX == s.x && newY == s.y) || (newX == e.x && newY == e.y))
			isNew = false;
		if(isNew)
		{
			for(unsigned int i = 0; i < solution.size(); i++)
				if(newX == solution[i].x && newY == solution[i].y)
				{
					isNew = false;
					break;
				}
			if(isNew)
				corner = {newX, newY};
		}

		temper *= COOL_DOWN;
		dTime = 1000*clock()/CLOCKS_PER_SEC - curTime;
		curTime = 1000*clock()/CLOCKS_PER_SEC;
	}

	return corner;
}

void update() // update nowBest
{
//	cout << "<update>\n";
//	for(int i = 0; i < solution.size(); i++)
//		cout << solution[i].x << " " << solution[i].y << " ";
	//cout << "\n--> " << ttDanger(solution) << "\n";
	
	//cout << "nowBest: " << ttDanger(nowBest) << "\n";
	if(ttDanger(solution) < ttDanger(nowBest))
		nowBest.assign(solution.begin(), solution.end());
}

// return the danger of a line
double lnDanger(Point from, Point to, double firstD)
{
	//cout << "\n<lnDanger>\n";
	double d = from.distance(to);
	//cout << "d = " << d << "\n";
	
	int checkCnt = floor((d-firstD)/unit) + 1;

	double lnDan = 0;

	Point checkPt = {from.x + (to.x-from.x)*firstD/d, from.y + (to.y-from.y)*firstD/d};
	lnDan += checkPt.danger();

	for(int i = 1; i < checkCnt; i++)
	{
		checkPt = {checkPt.x + (to.x-from.x)*unit/d, checkPt.y + (to.y-from.y)*unit/d};

		if(checkPt.x == e.x && checkPt.y == e.y)
			break;
		else
			lnDan += checkPt.danger();
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
		ttDan += lnDanger(s, corner.front(), unit);

		double firstD = unit*(static_cast<int>(s.distance(corner[0]))/unit + 1) - s.distance(corner[0]);
		for(unsigned int i = 0; i < corner.size()-1; i++)
		{
			double d = corner[i].distance(corner[i+1]);
			if(d <= firstD)
			{
				firstD -= d;
				continue;
			}
			else
			{
				ttDan += lnDanger(corner[i], corner[i+1], firstD);
				firstD = unit*(static_cast<int>(d - firstD)/unit + 1) - (d - firstD);
			}
		}
		ttDan += lnDanger(corner.back(), e, firstD);
	}

	ttDan += w * corner.size();

	return ttDan;
}
