#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Models.h"

char* seat_letter[] = { "A", "B", "C", "D", "E", "F" };

int randr(int max, int min) {
	int rnum = rand() % (max - min + 1) + min;
	return rnum;
}

void generate_flight(struct Flight** flight) {
	int available_seats = 7;
	char buf[20];
	*flight = malloc(sizeof(struct Flight));
	(*flight)->seats = malloc(available_seats * sizeof(char*));
	for (int i = 0; i < available_seats; i++) {
		(*flight)->seats[i] = malloc(sizeof(char*));
		snprintf(buf, 20, "%d%s", randr(9, 1), seat_letter[randr(5, 0)]);
		strcpy_s((*flight)->seats[i], sizeof((*flight)->seats[i]), buf);
		for (int j = 0; j < i; j++) {
			if (strcmp((*flight)->seats[i], (*flight)->seats[j]) == 0) {
				i--;
				break;
			}
		}
	}
	strcpy_s((*flight)->destination, 20, destination_loc);
	strcpy_s((*flight)->departure, 20, departure_loc);
	(*flight)->num_seats = available_seats;

}

void reserve_seat(struct Flight** flight, const char* seat) {
	for (int i = 0; i < (*flight)->num_seats; i++) {
		if ((*flight)->seats[i] != NULL && strcmp((*flight)->seats[i], seat) == 0) {
			printf("You have booked seat %s on flight from %s to %s\n", seat, (*flight)->departure, (*flight)->destination);
			(*flight)->num_seats--;
			for (int j = i; j < (*flight)->num_seats; j++) {
				(*flight)->seats[j] = (*flight)->seats[j + 1];
			}
			char* temp = realloc((*flight)->seats, ((*flight)->num_seats) * sizeof(char*));
			(*flight)->seats = temp;
			temp = NULL;
			break;
		}
	}
}