#include <iostream>
#include <math.h>
using namespace std;

int main()
{
	float radius;
	float area;
	cout << "Enter the radius of your circle" << "\n";
	cin >> radius;
	area = 2.0*M_PI*radius;
	cout << "The area is: " << area << "\n";
	return 0;
}
