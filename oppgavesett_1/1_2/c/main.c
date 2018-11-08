#include <stdio.h>
#include <math.h>
#include <float.h>

#define pi 3.14159265358979323846

int main()
{
	float radius;
	float area;
	printf("Enter the radius of the circle: \n\t");
	scanf("%f", &radius); 
	area = 2.0*pi*radius;
	printf("The area of the circle is %f \n", area);
	return 0;
}
