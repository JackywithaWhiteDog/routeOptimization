#include <iostream>
using namespace std;

void bubbleSort(const int unsorted[], int sorted, int len);

int main()
{
	int len = 0;
	cin >> len;
	int* unsorted = new int[len];
	int* sorted = new int[len];

	bubbleSort(unsorted, sorted, len);

	delete[] unsorted;
	return 0;
}

void bubbleSort()