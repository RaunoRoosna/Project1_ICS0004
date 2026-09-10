#pragma once

typedef struct {
	char destination;
	char departure;
	char airline;
	char seat_column[10];
	int seat_row[10];
}Flight;

/*struct Reservation {
	char first_name[];
	char last_name[];
	struct Flight flight;
};*/