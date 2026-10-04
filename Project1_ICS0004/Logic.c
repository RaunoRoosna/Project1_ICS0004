
#include "Logic.h"


char* seat_letter[] = { "A", "B", "C", "D", "E", "F" };

int random_number_in_range(int max, int min) { //return a random value from max to min
	return (rand() % (max - min + 1) + min);
}

void generate_flight(struct Flight** flight, char* destination_loc, char* departure_loc) { // Fills out a flight struct 
	int seats = 7;
	char buf[SEAT_LENGTH];
	*flight = malloc(sizeof(struct Flight));
	error_handler(101, *flight, 1);

	(*flight)->available_seats = malloc(seats * sizeof(char*)); // Array of pointers
	error_handler(101, (*flight)->available_seats, 2);

	for (int i = 0; i < seats; i++) {
		bool duplicate = true;
		while (duplicate) {
			snprintf(buf, SEAT_LENGTH, "%d%s", random_number_in_range(9, 1), seat_letter[random_number_in_range(5, 0)]);
			duplicate = false;
			for (int j = 0; j < i; j++) {  // Check for duplicates
				if (!strcmp(buf, (*flight)->available_seats[j])) {
					duplicate = true;
					break;
				}
			}
		}
		(*flight)->available_seats[i] = malloc(SEAT_LENGTH * sizeof(char));
		error_handler(101, (*flight)->available_seats[i], 3);
		strcpy_s((*flight)->available_seats[i], SEAT_LENGTH, buf);
	}
	strcpy_s((*flight)->destination, LOCATION_LENGTH, destination_loc);
	strcpy_s((*flight)->departure, LOCATION_LENGTH, departure_loc);
	(*flight)->number_of_seats = seats;

	bool flight_number_exists = false;
	int generated_flight_number;

	do {
		flight_number_exists = false;
		generated_flight_number = random_number_in_range(9999, 1000);
		if (cached_flights != NULL) {
			for (int k = 0; k < num_flights; k++) { //Checks if the flight number is already in use
				if (generated_flight_number == cached_flights[k]->flight_number) {
					flight_number_exists = true;
					break;
				}
			}
		}
	} while (flight_number_exists);

	(*flight)->flight_number = generated_flight_number;
}

void reserve_seat(struct Flight** flight) { // Removes the seat from flight struct and reallocates the memory for the seats array
	printf("Choose the seat you want to book: ");
	fgets(selected_seat, sizeof(selected_seat), stdin);
	selected_seat[strcspn(selected_seat, "\n")] = 0;

	for (int i = 0; i < (*flight)->number_of_seats; i++) {
		if ((*flight)->available_seats[i] != NULL && strcmp((*flight)->available_seats[i], selected_seat) == 0) {
			printf("You have booked seat %s on flight from %s to %s\n", selected_seat, (*flight)->departure, (*flight)->destination);
			(*flight)->number_of_seats--;
			reservation_to_cache(*flight);

			for (int j = i; j < (*flight)->number_of_seats; j++) { // overrides deleted seat with the next seat in the array
				(*flight)->available_seats[j] = (*flight)->available_seats[j + 1];
			}
			char** temp = realloc((*flight)->available_seats, ((*flight)->number_of_seats) * sizeof(char*));
			error_handler(101, temp, 4);
			(*flight)->available_seats = temp;
			temp = NULL;
			break;
		}
		else {

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
	if (flight == NULL || flight->available_seats == NULL) {
		printf("Error: Invalid flight data\n");
		return;
	}
	printf("----------------------\n");
	printf("Departure: %s\n", flight->departure);
	printf("Destination: %s\n", flight->destination);
	printf("Flight number: %d\n", flight->flight_number);
	printf("The available seats are: "); // Prints the available seats to the user
	for (int i = 0; i < flight->number_of_seats; i++) {
		printf("%s ", flight->available_seats[i]);
	}
	printf("\n");
	printf("----------------------\n");
}


void flight_to_cache(void){
	struct Flight** temporary_cached_flights = realloc(cached_flights, (num_flights + 1) * sizeof(struct Flight*));
	error_handler(101, temporary_cached_flights, 5);

	temporary_cached_flights[num_flights] = current_flight; 
	num_flights++;

	cached_flights = temporary_cached_flights;
	temporary_cached_flights = NULL;
}

void reservation_to_cache(struct Flight* flight) {
	struct Reservation* temporary_cached_reservations = realloc(cached_reservations, (num_reservations + 1) * (sizeof(struct Reservation) + 1));
	error_handler(101, temporary_cached_reservations, 6);

	temporary_cached_reservations[num_reservations].uid = current_user.uid; 
	temporary_cached_reservations[num_reservations].flight_number = flight->flight_number;
	strcpy_s(temporary_cached_reservations[num_reservations].seat, SEAT_LENGTH, selected_seat);
	num_reservations++;
	cached_reservations = temporary_cached_reservations;
	temporary_cached_reservations = NULL;
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

void find_existing_flights(int* array, int* elements, char* destination_loc, char* departure_loc) {
	int limit_found_flights = 10;
	for (int i = 0, j = 0; i < num_flights;) {
		if (!strcmp(destination_loc, cached_flights[i]->destination) && !strcmp(departure_loc, cached_flights[i]->departure)) {
			while (j < limit_found_flights -1) {
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
	if (num_flights == 0) {
		free(cached_flights[index]);
		free(cached_flights);
		return;
	}

	cached_flights[index] = cached_flights[num_flights];
	struct Flight** temporary_cached_flights = realloc(cached_flights, (sizeof(struct Flight*)) * num_flights);
	error_handler(101, temporary_cached_flights, NULL);
	cached_flights = temporary_cached_flights;
	printf("%p\n", cached_flights[index]);
	temporary_cached_flights = NULL;
}

void cancel_reservation(int flight_number, char* seat) {
	num_reservations--;
	int selected_reservation = find_reservation(flight_number, seat, NULL);

	cached_reservations[selected_reservation] = cached_reservations[num_reservations];
	struct Reservation* temporary_cached_reservations = realloc(cached_reservations, (sizeof(struct Reservation) + 1) * num_reservations);
	error_handler(101, temporary_cached_reservations, NULL);
	cached_reservations = temporary_cached_reservations;
	temporary_cached_reservations = NULL;
}
	
void cancel_reservation_ui(access){
	switch (access){
		int* reservations;
		int out = -1;
		int* count = malloc(sizeof(int));
		error_handler(101, count, NULL);
		int f_num;
		char seat[SEAT_LENGTH];
	case 11: //User
		do {
			printf("Enter flight number of reservation you want to cancel:\n");
			if (!read_and_scan_int(&f_num)) { 
				continue; 
			}
			find_reservations(f_num, current_user.uid, &reservations, &count);
			for (int i = 0; i < count; i++) {
				show_reservation(cached_reservations[reservations[i]], 11);
			}
			do {
				printf("Enter seat number of reservation you want to cancel:\n");
				fgets(seat, SEAT_LENGTH, stdin);
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

void find_reservations(int flight_number, int uid, int** array, int* count) {
	for (int i = 0; i < num_reservations; i++) {
		if (((flight_number == cached_reservations[i].flight_number) || (flight_number == NULL)) && ((uid == current_user.uid) || (uid == NULL))) {
			int* temp = realloc(*array, sizeof(int) * (*count + 1));
			*array = temp;
			temp = NULL;
			(*array)[*count] = i;
			(*count) = (*count) + 1;
		}
	}
}

void show_reservation(struct Reservation reservation, int access) {
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


void add_flight(void) {
	bool correct_flight_number;
	char* buf = malloc(20);
	current_flight = malloc(sizeof(struct Flight));
	error_handler(101, current_flight, NULL);

	char* destination_loc = get_destination_menu();
	strcpy_s(current_flight->destination, LOCATION_LENGTH, destination_loc);

	char* departure_loc = get_departure_menu();
	strcpy_s(current_flight->departure, LOCATION_LENGTH, departure_loc);

	do {
		printf("Enter flight number(4 digits):");	
		if (!read_and_scan_int(&current_flight->flight_number)) {
			printf("NaN\n");
			correct_flight_number = true;
			continue;
		}
		if (current_flight->flight_number < 1000 || current_flight->flight_number > 9999) {
			correct_flight_number = true;
			clean_stdin();
			continue;
		}
		correct_flight_number = false;
	} while (correct_flight_number == true);

	printf("Enter number of available seats: ");
	if (!read_and_scan_int(&current_flight->number_of_seats)) {
		printf("NaN\n");
	}
	printf("%d\n", current_flight->number_of_seats);

	current_flight->available_seats = malloc(sizeof(char*) * current_flight->number_of_seats);
	error_handler(101, current_flight->available_seats, 1005);

	for (int i = 0; i < current_flight->number_of_seats; i++) {
		current_flight->available_seats[i] = malloc(SEAT_LENGTH * sizeof(char));
		error_handler(101, current_flight->available_seats[i], NULL);
		printf("Enter seat %d: ", i + 1);
		fgets(buf, SEAT_LENGTH, stdin);
		buf[strcspn(buf, "\n")] = 0;
		strcpy_s(current_flight->available_seats[i], SEAT_LENGTH, buf);
	}
	free(buf);
}

int read_and_scan_int(int* out) {
	static char line[100];
	int chars = 0;
	if (!fgets(line, sizeof line, stdin) || sscanf_s(line, " %d %n", out, &chars) != 1 || line[chars] != 0)
		return 0;
	return 1;
}

bool scan_string(char* string) {
	if (strlen(string) <= 0) { return false; }
	if (!strchr(string, '\n')) {
		string[0] = '\0';
		clean_stdin();
		return false;
	}
	string[strcspn(string, "\n")] = '\0';
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\0') { return true; }
		if (!isalpha(string[i])) {
			string[0] = '\0';
			return false;
		}
	}
	return true;
}