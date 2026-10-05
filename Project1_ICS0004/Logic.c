
#include "Logic.h"


char* SEAT_LETTERS[] = { "A", "B", "C", "D", "E", "F" };

int random_number_in_range(int max, int min) { //return a random value from max to min
	return (rand() % (max - min + 1) + min);
}

void generate_flight(struct Flight** flight, char* destination_loc, char* departure_loc) { // Fills out a flight struct 
	int seats = 7;
	char buf[SEAT_LENGTH];
	*flight = malloc(sizeof(struct Flight));
	error_handler(101, *flight, 15);

	(*flight)->available_seats = malloc(seats * sizeof(char*)); // Array of pointers
	error_handler(101, (*flight)->available_seats, 18);
	for (int i = 0; i < seats; i++) {
		bool duplicate;
		do{
			duplicate = false;
			snprintf(buf, SEAT_LENGTH, "%d%s", random_number_in_range(9, 1), SEAT_LETTERS[random_number_in_range(5, 0)]);
			for (int j = 0; j < i; j++) {  // Check for duplicates
				if (!strcmp(buf, (*flight)->available_seats[j])) {
					duplicate = true;
					break;
				}
			}
		}while (duplicate);
		(*flight)->available_seats[i] = malloc(SEAT_LENGTH * sizeof(char));
		error_handler(101, (*flight)->available_seats[i], 32);
		strcpy_s((*flight)->available_seats[i], SEAT_LENGTH, buf);
	}
	strcpy_s((*flight)->destination, LOCATION_LENGTH, destination_loc);
	strcpy_s((*flight)->departure, LOCATION_LENGTH, departure_loc);
	(*flight)->number_of_seats = seats;
	bool flight_number_exists;
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
	char selected_seat[SEAT_LENGTH +1];
	bool unput_valid = false;
	bool seat_exists = false;
	int i;
	do {
		printf("Choose the seat you want to book: ");
		fgets(selected_seat, SEAT_LENGTH +1, stdin);
		if (strlen(selected_seat) < SEAT_LENGTH -1) {
			continue;
		}
		unput_valid = validate_string(selected_seat);

		for (i = 0; i < (*flight)->number_of_seats; i++) {
			if (!strcmp((*flight)->available_seats[i], selected_seat)) {
				seat_exists = true;
				break;
			}
		}
	} while (!unput_valid || !seat_exists);

	printf("You have booked seat %s on flight from %s to %s\n", selected_seat, (*flight)->departure, (*flight)->destination);
	(*flight)->number_of_seats--;
	reservation_to_cache((*flight)->flight_number, selected_seat);

	for (int j = i; j < (*flight)->number_of_seats; j++) { // overrides deleted seat with the next seat in the array
		(*flight)->available_seats[j] = (*flight)->available_seats[j + 1];
	}
	char** temporary_available_seats = realloc((*flight)->available_seats, ((*flight)->number_of_seats) * sizeof(char*));
	error_handler(101, temporary_available_seats, 86);
	(*flight)->available_seats = temporary_available_seats;
	temporary_available_seats = NULL;
}

int check_user(char* username) { // checks if the user exists
	if (!strcmp(username, admin.username)) {
		return 10;
	}
	else if (!strcmp(username, user1.username)) {
		current_user = user1;
		return 11;
	}
	else if (!strcmp(username, "exit")) {
		exit(0);
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
	if (flight == NULL) {
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


void flight_to_cache(void){ // remake to take input struct flight
	struct Flight** temporary_cached_flights = realloc(cached_flights, (num_flights + 1) * sizeof(struct Flight*));
	error_handler(101, temporary_cached_flights, 132);

	temporary_cached_flights[num_flights] = current_flight; 
	num_flights++;

	cached_flights = temporary_cached_flights;
	temporary_cached_flights = NULL;
}

void reservation_to_cache(int flight_number, char* seat) {
	struct Reservation* temporary_cached_reservations = realloc(cached_reservations, (num_reservations + 1) * (sizeof(struct Reservation) + 1));
	error_handler(101, temporary_cached_reservations, 143);

	temporary_cached_reservations[num_reservations].uid = current_user.uid; //uus funktsioon selleks
	temporary_cached_reservations[num_reservations].flight_number = flight_number;
	strcpy_s(temporary_cached_reservations[num_reservations].seat, SEAT_LENGTH, seat); 
	num_reservations++;
	cached_reservations = temporary_cached_reservations;
	temporary_cached_reservations = NULL;
}

int error_handler(int code, char* ptr, int debug) {
	switch (code) {

	case 100: //Unexpected behaviour
		printf("???");
		exit(100);

	case 101: //Memory allocation fail 
		if (ptr == NULL) {
			printf("%d\n", debug);
			printf("Memory allocation failed\n");
			exit(101);
		}
	}
}

void find_existing_flights(int* array, int* elements, char* destination_loc, char* departure_loc) {
	int limit_found_flights = 10;
	int result_offset = 0;
	for (int i = 0; i < num_flights; i++) {
		if (!strcmp(destination_loc, cached_flights[i]->destination) && !strcmp(departure_loc, cached_flights[i]->departure)) {
			if (result_offset < limit_found_flights -1) {
				array[result_offset++] = i;
				*elements = result_offset;
			}
		}
	}
}

int find_flight(int flight_num) { // finds a flight in the cached_flights array by flight number and returns the index of the flight
	for (int i = 0; i < num_flights; i++) {
		if (cached_flights[i]->flight_number == flight_num) { return i; }
	}
	return -1;
}

void cancel_flight(int index) { //check index

	num_flights--;
	if (num_flights <= 0) {
		free(cached_flights[index]);
		free(cached_flights);
		return;
	}

	cancel_reservations(cached_flights[index]->flight_number);

	cached_flights[index] = cached_flights[num_flights];
	struct Flight** temporary_cached_flights = realloc(cached_flights, (sizeof(struct Flight*)) * num_flights);
	error_handler(101, temporary_cached_flights, 202);
	cached_flights = temporary_cached_flights;
	printf("%p\n", cached_flights[index]);
	temporary_cached_flights = NULL;
}

void cancel_reservation(int flight_number, char* seat) {
	int selected_reservation = find_reservation(flight_number, seat, NULL);
	if (selected_reservation == -1) {
		printf("Reservation not found.\n");
		return;
	}
	num_reservations--;
	if (num_reservations == 0) {
		free(cached_reservations);
		cached_reservations = NULL;
		return;
	}

	cached_reservations[selected_reservation] = cached_reservations[num_reservations];
	struct Reservation* temporary_cached_reservations = realloc(cached_reservations, (sizeof(struct Reservation) + 1) * num_reservations);
	error_handler(101, temporary_cached_reservations, 219);
	cached_reservations = temporary_cached_reservations;
	temporary_cached_reservations = NULL;
}

void cancel_reservations(int flight_number) { // cancel all reservations for a flight
	for (int i = 0; i < num_reservations; i++) {
		if (cached_reservations[i].flight_number == flight_number) {
			cancel_reservation(flight_number, cached_reservations[i].seat);
			i--; // Adjust index after removal
		}
	}
}
	
void cancel_reservation_ui(int access){
	int* reservations;
	int reservation_index = -1;
	int* count = malloc(sizeof(int));
	error_handler(101, count, 237);
	int f_num;
	char seat[SEAT_LENGTH];
	do {
		printf("Enter flight number of reservation you want to cancel:\n");
		if (!read_and_scan_int(&f_num)) { 
			continue; 
		}
		find_reservations(f_num, current_user.uid, &reservations, &count);
		for (int i = 0; i < *count; i++) {
			show_reservation(cached_reservations[reservations[i]], access);
		}
		do {
			printf("Enter seat number of reservation you want to cancel:\n");
			fgets(seat, SEAT_LENGTH, stdin);
			if (!validate_string(seat)) { continue; }
			reservation_index = find_reservation(f_num, seat, current_user.uid);
		} while (reservation_index == -1);
		cancel_reservation(f_num, seat);

	} while (reservation_index == -1);
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
	default:
		printf("Error: Invalid access level\n");
		break;
	}
}


void add_flight(void) {
	bool correct_flight_number = false;
	char* buf = malloc(20);
	current_flight = malloc(sizeof(struct Flight));
	error_handler(101, current_flight, 308);

	char* destination_loc = prompt_get_destination_menu();
	strcpy_s(current_flight->destination, LOCATION_LENGTH, destination_loc);

	char* departure_loc = prompt_get_departure_menu();
	strcpy_s(current_flight->departure, LOCATION_LENGTH, departure_loc);

	do {
		printf("Enter flight number(4 digits):");	
		if (!read_and_scan_int(&current_flight->flight_number)) {
			printf("NaN\n");
			correct_flight_number = false;
			continue;
		}
		if (current_flight->flight_number < 1000 || current_flight->flight_number > 9999) {
			correct_flight_number = false;
			printf("Error: Invalid flight number. Please enter a 4-digit flight number.\n");
			clean_stdin();
			continue;	
		}
		if (cached_flights != NULL) {
			for (int i = 0; i < num_flights; i++) { //Checks if the flight number is already in use
				if (current_flight->flight_number == cached_flights[i]->flight_number) {
					correct_flight_number = false;
					printf("Error: Flight number already exists. Please enter a different flight number.\n");
					break;
				}
			}
		}
		correct_flight_number = true;
	} while (!correct_flight_number);

	printf("Enter number of available seats: ");
	if (!read_and_scan_int(&current_flight->number_of_seats)) {
		printf("NaN\n");
	}

	current_flight->available_seats = malloc(sizeof(char*) * current_flight->number_of_seats);
	error_handler(101, current_flight->available_seats, 1005);

	for (int i = 0; i < current_flight->number_of_seats; i++) {
		current_flight->available_seats[i] = malloc(SEAT_LENGTH * sizeof(char));
		error_handler(101, current_flight->available_seats[i], 351);
		printf("Enter seat %d: ", i + 1);
		fgets(buf, SEAT_LENGTH +1, stdin);
		if (!validate_string(buf)) {
			i--; // Retry entering seat at the same position
			continue;
		}
		bool duplicate = false;
		for (int j = 0; j < i; j++) {  // Check for duplicates
			if (!strcmp(buf, current_flight->available_seats[j])) {
				duplicate = true;
				break;
			}
		}
		if (duplicate) {
			printf("Error: Duplicate seat. Please enter a different seat.\n");
			i--; // Retry entering seat at the same position
			continue;
		}
		strcpy_s(current_flight->available_seats[i], SEAT_LENGTH, buf);
	}
	free(buf);
}

int read_and_scan_int(int* out) {
	static char line[100];
	int chars = 0;
	if (!fgets(line, sizeof line, stdin) || sscanf_s(line, " %d %n", out, &chars) != 1 || line[chars] != 0) {
		if (strlen(line) > 22) {
			clean_stdin();
		}
		return 0;
	}
	if (strlen(line) > 22) {
		clean_stdin();
	}
	return 1;
}

bool validate_alpha_string(char* string) {
	if (!strchr(string, '\n')) {
		string[0] = '\0';
		printf("Error: Invalid input. Input exceeded maximum length.\n");
		clean_stdin();
		return false;
	}
	string[strcspn(string, "\n")] = '\0';
	if (strlen(string) <= 0) { 
		printf("Error: Invalid input. Please enter a string.\n");
		return false; 
	}
	for (int i = 0; i < strlen(string); i++) {
		if (string[i] == '\0') { 
			return true;
		}
		if (!isalpha(string[i])) {
			string[0] = '\0';
			printf("Error: Invalid input. Please enter a string containing only letters.\n");
			return false;
		}
	}
	return true;
}

bool validate_string(char* string) {
	if (!strchr(string, '\n')) {
		string[0] = '\0';
		clean_stdin();
		return false;
	}
	string[strcspn(string, "\n")] = '\0';
	if (strlen(string) <= 0) {
		printf("Error: Invalid input. Please enter a string.\n");
		return false;
	}
	return true;
}

//ternary operators