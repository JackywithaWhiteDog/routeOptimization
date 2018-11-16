#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const int PAIR = 2;

float random(int upper, int lower);

void randInfo(int &n, int &m, int &w, int &d, int* &x, int* &y, int* &r, int* &p, int start[], int end[]);
void getInfo(int &n, int &m, int &w, int &d, int* &x, int* &y, int* &r, int* &p, int start[], int end[]);
void showInfo(int n, int m, int w, int d, const int* x, const int* y, const int* r, const int* p, const int start[], const int end[]);

float ptDistance(float x1, float y1, float x2, float y2);

float pointDanger(const float pt[], const int m, const int* x, const int* y, const int* r, const int* p);
float lineDanger(float start[], float end[], const int m, const int* x, const int* y, const int* r, const int* p, float firstD = 1);
float totalDanger(float start[], float end[], float corner[][PAIR], int corCnt, const int m, const int* x, const int* y, const int* r, const int* p);

int main()
{
	srand(time(NULL));
	
	int mapLen = 0, ptCnt = 0, weight = 0, distance = 0;
	int* x = nullptr;
	int* y = nullptr;
	int* r = nullptr;
	int* p = nullptr;
	int start[PAIR] = {0}, end[PAIR] = {0};
	
	/*
	getInfo(mapLen, ptCnt, weight, distance, x, y, r, p, start, end);
	/*/
	randInfo(mapLen, ptCnt, weight, distance, x, y, r, p, start, end);
//	showInfo(mapLen, ptCnt, weight, distance, x, y, r, p, start, end);
	/**/
	
	delete [] x;
	delete [] y;
	delete [] r;
	delete [] p;
	
	return 0;
 } 
 
float random(int lower, int upper)
{
	return (upper-lower)*static_cast<float>(rand())/RAND_MAX+lower;
}

void getInfo(int &n, int &m, int &w, int &d, int* &x, int* &y, int* &r, int* &p, int start[], int end[])
{
	cin >> n >> m >> w >> d;
	x = new int [m];
	y = new int [m];
	r = new int [m];
	p = new int [m];
	for(int i = 0; i < m; i++)
	{
		cin >> x[i] >> y[i] >> r[i] >> p[i];
	}
	cin >> start[0] >> start[1] >> end[0] >> end[1];
}

void randInfo(int &n, int &m, int &w, int &d, int* &x, int* &y, int* &r, int* &p, int start[], int end[])
{
	n = static_cast<int>(random(1, 1000));
	m = static_cast<int>(random(1, (n+1)*(n+1)));
	w = static_cast<int>(random(0, 1000));
	d = static_cast<int>(random(n, n*n)); 
	x = new int [m];
	y = new int [m];
	r = new int [m];
	p = new int [m];
	for(int i = 0; i < m; i++)
	{
		x[i] = static_cast<int>(random(0, n));
		y[i] = static_cast<int>(random(0, n));
		r[i] = static_cast<int>(random(0, n));
		p[i] = static_cast<int>(random(1, 1000));
	}
	start[0] = static_cast<int>(random(0, n));
	start[1] = static_cast<int>(random(0, n));
	end[0] = static_cast<int>(random(0, n));
	end[1] = static_cast<int>(random(0, n));
}

void showInfo(int n, int m, int w, int d, const int* x, const int* y, const int* r, const int* p, const int start[], const int end[])
{
	cout << n << " " << m << " " << w << " " << d << "\n";
	for(int i = 0; i < m; i++)
	{
		cout << x[i] << " " << y[i] << " " << r[i] << " " << p[i] << "\n";
	}
	cout << start[0] << " " << start[1] << " " << end[0] << " " << end[1] << "\n";
}

float ptDistance(float x1, float y1, float x2, float y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

float pointDanger(const float pt[], const int m, const int* x, const int* y, const int* r, const int* p)
{
	float ptDanger = 0;
	for(int k = 0; k < m; k++)
	{
		// if the position is in the influence radius of dangerous point, add the danger
		if((pt[0]-x[k])*(pt[0]-x[k]) + (pt[1]-y[k])*(pt[1]-y[k]) < r[k]*r[k])
		{
			// danger (with one dangerous point) = (degree of danger) * [(radius) - (distance)] / (radius)
			ptDanger += p[k] * (r[k] - ptDistance(pt[0], pt[1], x[k], y[k])) / r[k];
		}
	}
	return ptDanger;
}

float lineDanger(float start[], float end[], const int m, const int* x, const int* y, const int* r, const int* p, float firstD)
{
	float d = ptDistance(start[0], start[1], end[0], end[1]);
	int checkCnt = static_cast<int>(d - firstD) + 1;
	float** checkPt = new float* [checkCnt];
	for(int i = 0; i < checkCnt; i++)
	{
		checkPt[i] = new float [PAIR];
	}
	
	checkPt[0][0] = start[0] + (firstD/d)*(end[0] - start[0]);
	checkPt[0][1] = start[1] + (firstD/d)*(end[1] - start[1]);
	
	for(int i = 1; i < checkCnt; i++)
	{
		checkPt[i][0] = checkPt[i-1][0] + (end[0] - start[0])/d;
		checkPt[i][1] = checkPt[i-1][1] + (end[1] - start[1])/d;
	}
	
	float lnDanger = 0;
	for(int i = 0; i < checkCnt; i++)
	{
		lnDanger += pointDanger(checkPt[i], m, x, y, r, p);
	}
	
	for(int i = 0; i < checkCnt; i++)
	{
		delete [] checkPt[i];
	}
	delete [] checkPt;
	
	return lnDanger;
}

float totalDanger(float start[], float end[], float corner[][PAIR], int corCnt, const int m, const int* x, const int* y, const int* r, const int* p)
{
	float ttDanger = 0;
	
	ttDanger += lineDanger(start, corner[0], m, x, y, r, p, 1);
	float d = ptDistance(start[0], start[1], end[0], end[1]);
	float firstD = 1 - (d - static_cast<int>(d));
	for(int i = 0; i < corCnt-1; i++)
	{
		ttDanger += lineDanger(corner[i], corner[i+1], m, x, y, r, p, firstD);
		d = ptDistance(corner[i][0], corner[i][1], corner[i+1][0], corner[i+1][1]);
		firstD = 1 - (d - static_cast<int>(d));
	}
	ttDanger += lineDanger(corner[corCnt-1], end, m, x, y, r, p, firstD);
	
	return ttDanger;
} 
