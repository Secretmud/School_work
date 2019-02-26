#include <stdio.h>


int main()
{
	int arr[8] = { 9, 8, 4, 2};
	int holder;
	printf("|SoA\t|AP\t|AV\t\t|Value\t|Hold\t|\n");
	printf("Pre sort\n");
	for (int x = 0; x < 4; x++)
		printf("| %d \t| %d \t| arr[%d] \t| %d \t|\t|\n", sizeof(arr)/sizeof(int), x, x, arr[x]);
	printf("Sort\n");
	for (int x = 0; x < 5; x++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (arr[i] > arr[i + 1]) {
				holder = arr[i];
				arr[i] = arr[i + 1];
				arr[i + 1] = holder;
				printf("| %d \t| %d \t| arr[%d] \t| %d \t| %d\t|\n", sizeof(arr)/sizeof(int), i, i, arr[i], holder);
			}
		}
	}
	printf("Post sort\n");
	for (int x = 0; x < 4; x++)
		printf("| %d \t| %d \t| arr[%d] \t| %d \t|\t|\n", sizeof(arr)/sizeof(int), x, x, arr[x]);
}
