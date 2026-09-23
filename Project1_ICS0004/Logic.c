
#include "Logic.h"


char* seat_letter[] = { "A", "B", "C", "D", "E", "F" };

int randr(int max, int min) { //return a random value from max to min
	int rnum = rand() % (max - min + 1) + min;
	return rnum;
}



void generate_flight(struct Flight** flight) { // Fills out a flight struct with random seat numbers and the destination and departure locations
	int available_seats = 7;
	char buf[20];
	*flight = malloc(sizeof(struct Flight));
	(*flight)->seats = malloc(available_seats * sizeof(char*));
	for (int i = 0; i < available_seats; i++) {
		(*flight)->seats[i] = malloc(sizeof(char*));
		snprintf(buf, 20, "%d%s", randr(9, 1), seat_letter[randr(5, 0)]);
		strcpy_s((*flight)->seats[i], sizeof((*flight)->seats[i]), buf);
		for (int j = 0; j < i; j++) {  // Check for duplicates
			if (strcmp((*flight)->seats[i], (*flight)->seats[j]) == 0) {
				i--;
				break;
			}
		}
	}
	strcpy_s((*flight)->destination, 20, destination_loc);
	strcpy_s((*flight)->departure, 20, departure_loc);
	(*flight)->num_seats = available_seats;
	(*flight)->flight_number = randr(9999, 1000);

}

void reserve_seat(struct Flight** flight, const char* seat) { // Removes the seat from the flight struct and reallocates the memory for the seats array
	for (int i = 0; i < (*flight)->num_seats; i++) {
		if ((*flight)->seats[i] != NULL && strcmp((*flight)->seats[i], seat) == 0) {
			printf("You have booked seat %s on flight from %s to %s\n", seat, (*flight)->departure, (*flight)->destination);

			(*flight)->num_seats--;
			for (int j = i; j < (*flight)->num_seats; j++) { // overrides deleted seat with the next seat in the array
				(*flight)->seats[j] = (*flight)->seats[j + 1];
			}
			char* temp = realloc((*flight)->seats, ((*flight)->num_seats) * sizeof(char*));
			(*flight)->seats = temp;
			temp = NULL;
			break;
		}
	}
}

int check_user(char* username) {
	if (strcmp(username, admin.username) == 0) {
		return 10;
	}
	else if (strcmp(username, user1.username) == 0) {
		return 11;
		current_user = &user1;
	}
	else if (strcmp(username, "exit") == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void clean_stdin(void) {
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
	
}