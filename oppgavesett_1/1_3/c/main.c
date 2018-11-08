#include <stdio.h>
#include <stdbool.h>

float kilogram_to_pound(int kg)
{
	return kg/0.454;
}

float pound_to_kilogram(int lbs)
{
	return lbs*0.454;
}

int main()
{
	float kg;
	float lbs;
	float ans;
	bool running = true;
	while(running){
		printf("This is a calculator that converts weights:\n\n\tMenu:\n\t1. KG -> LBS\n\t2. LBS -> KG\n\t3. quit\n\n");
		
		int choice;
		scanf("%d", &choice);

		if(choice == 1) {
			printf("Enter weight in KG");
			scanf("%f", &kg);
			ans = kilogram_to_pound(kg);
			printf("%f", ans);
		} else if(choice == 2) {
			printf("Enter weight in LBS");
			scanf("%f", &kg);
			ans = pound_to_kilogram(kg);
			printf("%f", ans);
		} else if(choice == 3) {
			printf("Later skater\n\n");
			running = false;
			return 0;
		} else {
			printf("You've to enter a valid key, this program only accepts integers");
		}

	}
}

