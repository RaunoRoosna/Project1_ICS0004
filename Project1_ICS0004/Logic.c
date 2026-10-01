
#include "Logic.h"


char* seat_letter[] = { "A", "B", "C", "D", "E", "F" };

int randr(int max, int min) { //return a random value from max to min
	int rnum = rand() % (max - min + 1) + min;
	return rnum;
}

void generate_flight(struct Flight** flight) { // Fills out a flight struct 
	int available_seats = 7;
	char buf[20];
	*flight = malloc(sizeof(struct Flight));
	error_handler(101, *flight, 1);
	(*flight)->seats = malloc(available_seats * sizeof(char*)); // Array of pointers
	error_handler(101, (*flight)->seats, 2);
	for (int i = 0; i < available_seats; i++) {
		(*flight)->seats[i] = malloc(seat_length);
		error_handler(101, (*flight)->seats[i], 3);
		snprintf(buf, 20, "%d%s", randr(9, 1), seat_letter[randr(5, 0)]);
		strcpy_s((*flight)->seats[i], seat_length, buf);
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

	bool temp = false; // Sets unique flight num
	int tempnum;
	do {
		tempnum = randr(9999, 1000);
		for (int k = 0; k < num_flights; k++) {
			if (tempnum == cached_flights[k]->flight_number) {
				temp = true;
				break;
			}
		}
	} while (temp);
	(*flight)->flight_number = tempnum;
}

void reserve_seat(struct Flight** flight) { // Removes the seat from the flight struct and reallocates the memory for the seats array
	printf("Choose the seat you want to book: ");
	fgets(selected_seat, sizeof(selected_seat), stdin);
	selected_seat[strcspn(selected_seat, "\n")] = 0;
	for (int i = 0; i < (*flight)->num_seats; i++) {
		if ((*flight)->seats[i] != NULL && strcmp((*flight)->seats[i], selected_seat) == 0) {
			printf("You have booked seat %s on flight from %s to %s\n", selected_seat, (*flight)->departure, (*flight)->destination);
			(*flight)->num_seats--;
			reservation_to_cache(*flight);
			for (int j = i; j < (*flight)->num_seats; j++) { // overrides deleted seat with the next seat in the array
				(*flight)->seats[j] = (*flight)->seats[j + 1];
			}
			char** temp = realloc((*flight)->seats, ((*flight)->num_seats) * sizeof(char*));
			error_handler(101, temp, 4);
			(*flight)->seats = temp;
			temp = NULL;
			break;
		}
	}
}

int check_user(char* username) { // checks if the user exists
	if (strcmp(username, admin.username) == 0) {
		return 10;
	}
	else if (strcmp(username, user1.username) == 0) {
		current_user = user1;
		return 11;
	}
	else if (strcmp(username, "exit") == 0) {
		return 0;
	}
	else { return 1; }
}

void clean_stdin(void) { // cleans stdin buffer
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
	
}

void show_flight(struct Flight* flight) { // Shows the flight information to the user
	printf("----------------------\n");
	printf("Departure: %s\n", flight->departure);
	printf("Destination: %s\n", flight->destination);
	printf("Flight number: %d\n", flight->flight_number);
	printf("The available seats are: "); // Prints the available seats to the user
	for (int i = 0; i < flight->num_seats; i++) {
		printf("%s ", flight->seats[i]);
	}
	printf("\n");
	printf("----------------------\n");
}


void flight_to_cache(void){
	struct Flight** temp = realloc(cached_flights, (num_flights + 1) * sizeof(struct Flight*));
	error_handler(101, temp, 5);

	temp[num_flights] = malloc(sizeof(struct Flight));
	error_handler(101, temp[num_flights], 5);

	strcpy_s(temp[num_flights]->destination, location_length, current_flight->destination);
	strcpy_s(temp[num_flights]->departure, location_length, current_flight->departure);
	temp[num_flights]->num_seats = current_flight->num_seats;
	temp[num_flights]->flight_number = current_flight->flight_number;
	temp[num_flights]->seats = realloc(current_flight->seats, current_flight->num_seats * sizeof(char*));
	error_handler(101, temp[num_flights]->seats, 6);
	num_flights++;
	free(current_flight);
	cached_flights = temp;
	temp = NULL;
}

void reservation_to_cache(struct Flight* flight) {
	struct Reservation* temp = realloc(cached_reservations, (num_reservations + 1) * sizeof(struct Reservation));
	error_handler(101, temp, 6);
	temp[num_reservations].uid = current_user.uid; 
	temp[num_reservations].flight_number = flight->flight_number;
	strcpy_s(temp[num_reservations].seat, seat_length, selected_seat);
	num_reservations++;
	cached_reservations = temp;
	temp = NULL;
}

int error_handler(int code, char* ptr, int debug) {
	switch (code) {

	case 100: //Unexpected behaviour
		printf("100");
		exit(100);

	case 101: //Memory allocation fail 
		if (ptr == NULL) {
			printf("%d\n", debug);
			printf("101");
			exit(101);
		}
	}
}

void find_existing_flights(int* array, int* elements) {
	for (int j = 0, i = 0; i < num_flights;) {
		if (!strcmp(destination_loc, cached_flights[i]->destination) && !strcmp(departure_loc, cached_flights[i]->departure)) {
			while (j < 9) {
				array[j] = i;
				j++;
				i++;
				*elements = j;
				break;
			}
		}
		else { i++; }
	}
}

int find_flight(int flight_num) { // finds a flight in the cached_flights array by flight number and returns the index of the flight
	for (int i = 0; i < num_flights; i++) {
		if (cached_flights[i]->flight_number == flight_num) { return i; }
	}

	return -1;
}

void cancel_flight(int index) {
	num_flights--;

	cached_flights[index] = cached_flights[num_flights];
	struct Flight* temp = realloc(cached_flights, (sizeof(struct Flight)) * num_flights);
	error_handler(101, temp, NULL);
	cached_flights = temp;
	temp = NULL;
}

void cancel_reservation(int flight_number, char* seat) {
	num_reservations--;
	int reserv = find_reservation(flight_number, seat, NULL);

	cached_reservations[reserv] = cached_reservations[num_reservations];
	struct Reservation* temp = realloc(cached_reservations, (sizeof(struct Reservation)) * num_reservations);
	cached_reservations = temp;
	temp = NULL;
}

void cancel_reservation_ui(void){
	switch (access){
		int* reservations;
		int out = -1;
		int* count = malloc(sizeof(int));
		int f_num;
		char seat[seat_length];
	case 11: //User
		do {
			printf("Enter flight number of reservation you want to cancel:\n");
			if (!read_and_scan_int(&f_num)) { 
				continue; 
			}
			find_reservations(f_num, current_user.uid, &reservations, &count);
			for (int i = 0; i < count; i++) {
				show_reservation(cached_reservations[reservations[i]]);
			}
			do {
				printf("Enter seat number of reservation you want to cancel:\n");
				fgets(seat, seat_length, stdin);
				seat[strcspn(seat, "\n")] = 0;
				out = find_reservation(f_num, seat, current_user.uid);
			} while (out == -1);
			cancel_reservation(f_num, seat);

		} while (out == -1);


		break;
	case 10: //Admin
		
		break;
	}
}

int find_reservation(int flight_number, char* seat, int uid) {
	for (int i = 0; i < num_reservations; i++) {
		if ((cached_reservations[i].flight_number == flight_number) && (!strcmp(cached_reservations[i].seat, seat)) && ((cached_reservations[i].uid == uid) || uid == NULL)) {
			return i; 
		}
	}
	return -1;
}

void find_reservations(int flight_number, int uid, int* array, int* count) {
	*count = 0;
	for (int i = 0; i < num_reservations; i++) {
		if (((flight_number == cached_reservations[i].flight_number) || (flight_number == NULL)) && ((uid == current_user.uid) || (uid == NULL))) {
			int* temp = realloc(array, sizeof(int) * (*count + 1));
			array = temp;
			temp = NULL;
			array[*count] = i;
			printf("array: %d\n", array[*count]);
			*count = *count + 1;
		}
	}
}

void show_reservation(struct Reservation reservation) {
	switch (access) {
	case 11:
		printf("----------------------\n");
		printf("flight number: %d\n", reservation.flight_number);
		printf("seat: %s\n", reservation.seat);
		printf("----------------------\n");
		break;

	case 10:
		printf("----------------------\n");
		printf("flight number: %d\n", reservation.flight_number);
		printf("user id: %d\n", reservation.uid);
		printf("seat: %s\n", reservation.seat);
		printf("----------------------\n");
		break;
	}
}


void manual_add_flight() {
	printf("Destination:");
	

}

int read_and_scan_int(int* out) {
	static char line[20];
	int chars = 0;
	if (!fgets(line, sizeof line, stdin) || sscanf_s(line, " %d %n", out, &chars) != 1 || line[chars] != 0)
		return 0;
	return 1;
}

