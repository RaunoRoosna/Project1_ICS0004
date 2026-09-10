#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Models.h"

char* seat_letter[] = { "A", "B", "C", "D", "E", "F" };

int randr(int max, int min) {
	int rnum = rand() % (max - min + 1) + min;
	return rnum;
}

void generate_flight(Flight flight) {
	int available_seats = randr(9, 0);
	for (int i = 0; i < available_seats; i++) {
		flight.seat_row[i] = randr(9, 0);
		flight.seat_column[i] = seat_letter[randr(5, 0)];
	}

}

