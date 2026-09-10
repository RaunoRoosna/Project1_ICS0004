#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Logic.h"
#include "Models.h"

int available_seats;
char destination_loc;
char departure_loc;
int temp;

int main() {
	srand(time(NULL));
	char destination_loc[20];
	char departure_loc[20];
	int available_seats = randr(9, 0);
	Flight flight1 = { 0 };

	printf("Destination:");
	fgets(destination_loc, sizeof(destination_loc), stdin);
	destination_loc[strcspn(destination_loc, "\n")] = 0;
	if (strlen(destination_loc) != 0) {
		printf("Departure:");
		fgets(departure_loc, sizeof(departure_loc), stdin);  
		departure_loc[strcspn(departure_loc, "\n")] = 0;
	}
	else {
		return 0;
	}

	if (strlen(departure_loc) != 0) {
		generate_flight(flight1);
		printf("The available seats are: ");
	}
	else {
		return 0;
	}
return 0;
}