#pragma once
#include <stdlib.h>

struct Flight {
	char destination[20];
	char departure[20];
	//char airline[20];
	char** seats;		
	int num_seats;
	//int flight_number;
};


char destination_loc[20];
char departure_loc[20];
char selected_seat[20];
struct Flight* flight1;
struct Flight* flight2;

/*struct Reservation {
	char first_name[20];
	char last_name[20];
	struct Flight flight;
};*/
