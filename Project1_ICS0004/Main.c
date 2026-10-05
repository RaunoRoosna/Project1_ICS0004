#include "Logic.h"
#include "DataManager.h"
#include "UI.h"
#include <ctype.h>
int selected_flight;

int main() {
	srand(time(NULL));
	deserialise_flights_from_json();
	deserialise_reservations_from_json();

	int access = prompt_get_username_menu();
	switch (access) {

	case 11: // User access granted
	{
		char switch_action = prompt_get_reserve_view_menu();

		switch (switch_action) {
		case 'r':
			display_reservation_menu();
			break;
		case 'v':
			display_user_view_menu();
		}
		exit(0);
		break;
	}
	case 10: // Admin access 
		while (true) {
			char admin_action = prompt_get_view_modify_menu();

			switch (admin_action) {

			case 'v': // show all fligths in cached_flights array
				display_view_menu();
				continue;
			case 'm':
				display_modify_menu();
				continue;
			case 'e':
				exit(0);
			}
		}
	case 1:
		printf("No such user exists");
		exit(1);
	}
	
	for (int i = 0; i < current_flight->number_of_seats; i++) {
		free(current_flight->available_seats[i]);
	}
	free(current_flight->available_seats);
	return 0;
}

int prompt_get_username_menu(void) {
	int access;
	char username[20];
	bool validated_string = false;
	do {
		printf("Username: ");
		fgets(username, sizeof(username), stdin);
		validated_string = validate_string(username);
	} while (!validated_string);
	return check_user(username);
}

char prompt_get_reserve_view_menu(void) {
	char switch_action;
	do {
		printf("reserve a seat on a flight/view reservations(r/v):");
		switch_action = getchar();	
		clean_stdin();
	} while (switch_action != 'r' && switch_action != 'v');
	return switch_action;
}

char* prompt_get_destination_menu(void) {
	char destination_loc[LOCATION_LENGTH];
	bool correct_destination = false;
	do {
		printf("Destination (limited to 20 characters):"); 
		fgets(destination_loc, LOCATION_LENGTH, stdin);
		correct_destination = validate_alpha_string(destination_loc);
	} while (!correct_destination);
	return destination_loc;
}

char* prompt_get_departure_menu(void) {
	char departure_location[LOCATION_LENGTH];
	bool correct_departure = false;
	do {
		printf("Departure (limited to 20 characters):");
		fgets(departure_location, LOCATION_LENGTH, stdin);
		correct_departure = validate_alpha_string(departure_location);
	} while (!correct_departure);
	return departure_location;
}

void random_flight_generation(char* destination_loc, char* departure_loc) {
	generate_flight(&current_flight, destination_loc, departure_loc);
	show_flight(current_flight);
	reserve_seat(&current_flight);
	flight_to_cache();
}

void flight_menu(int array[], int* elements) {
	for (int i = 0; i < *elements; i++) {
		show_flight(cached_flights[array[i]]);
	}

	int selected_flight;
	bool correct_flight_number = false;
	do {
		//clean_stdin();
		printf("Choose flight (enter flight number):");
		if (!read_and_scan_int(&selected_flight)) {
			printf("NaN\n");
			continue;
		}
		for (int j = 0; j < *elements; j++) {
			if (selected_flight == cached_flights[array[j]]->flight_number) {
				correct_flight_number = false;
				reserve_seat(&cached_flights[array[j]]);
			}
		}
	} while (!correct_flight_number);
}

void reservation_menu(char* destination_loc, char* departure_loc) {
	int array[10] = { 0 };
	int* elements = malloc(sizeof(int));
	*elements = 0;
	find_existing_flights(array, elements, destination_loc, departure_loc);
	if (*elements == 0) {
		random_flight_generation(destination_loc, departure_loc);
	}
	else {
		flight_menu(array, elements);
	}
}

char prompt_get_view_modify_menu(void) {
	char admin_action;
	do {
		printf("view flights/modify flights (v/m):");
		admin_action = getchar();
		clean_stdin();
	} while (admin_action != 'v' && admin_action != 'm' && admin_action != 'e');
	return admin_action;
}

char prompt_get_all_search_menu(void) {
	char switch_action;
	do {
		printf("show all flights/search for flights (a/s):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'a' && switch_action != 's');
	return switch_action;
}

char prompt_get_destination_and_departure_or_flightnum_menu(void) {
	char switch_action;
	do {	
		printf("By destination and departure/by flight number (d/f):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'd' && switch_action != 'f');
	return switch_action;
}

int prompt_get_flightnum_menu(int selected_flight) {
	int index = -1;
	do {
		printf("Enter flight number:");
		if (!read_and_scan_int(&selected_flight)) {
			index = -1;
			continue;
		}
		index = find_flight(selected_flight);
	} while (index == -1);
	return index;
}

char prompt_get_add_delete_menu(void) {
	char switch_action;
	do {
		printf("add flight/delete flight (a/d):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'a' && switch_action != 'd');
	return switch_action;
}

void show_search_by_destination_and_departure_menu(void) { // rename
	char destination_loc[LOCATION_LENGTH];
	char departure_loc[LOCATION_LENGTH];
	do {
		strcpy_s(destination_loc, LOCATION_LENGTH, prompt_get_destination_menu());
		strcpy_s(departure_loc, LOCATION_LENGTH, prompt_get_departure_menu());
	} while (!strcmp(destination_loc, departure_loc));

	int array[10] = { 0 };
	int elements[1] = { 0 };
	find_existing_flights(array, elements, destination_loc, departure_loc);
	if (elements[0] == 0) {
		printf("No such flight exists\n");
	}
	else {
		for (int i = 0; i < elements[0]; i++) {
			show_flight(cached_flights[array[i]]);
		}
	}
}

void show_search_menu(void) {
	char filter = prompt_get_destination_and_departure_or_flightnum_menu();
	int index;

	switch (filter) {
	case 'd': // search by destination and departure
		show_search_by_destination_and_departure_menu();
		break;
	case 'f': // search by flight number
		index = prompt_get_flightnum_menu(selected_flight);
		show_flight(cached_flights[index]);
		break;
	}
}	

void display_modify_menu(void) {
	char switch_action = prompt_get_add_delete_menu();
	switch (switch_action) {
	case 'a':
		add_flight();
		flight_to_cache();
		write_flights_to_file();
		break;
	case 'd':
	{
		int index = prompt_get_flightnum_menu(selected_flight);
		printf("%d\n", index);
		cancel_flight(index);
		write_flights_to_file();
		write_reservations_to_file();
		break;
	}
	}
}

void display_view_menu(void) {
	char switch_action = prompt_get_all_search_menu();
	switch (switch_action) {
	case 'a':
		for (int i = 0; i < num_flights; i++) {
			show_flight(cached_flights[i]);
		}
		break;

	case 's':
		show_search_menu();
		break;
	}
}

void display_reservation_menu(void) {
	char destination_loc[LOCATION_LENGTH];
	char departure_loc[LOCATION_LENGTH];
	do {
		strcpy_s(destination_loc, LOCATION_LENGTH, prompt_get_destination_menu());
		strcpy_s(departure_loc, LOCATION_LENGTH, prompt_get_departure_menu());
	} while (!strcmp(destination_loc, departure_loc));

	reservation_menu(destination_loc, departure_loc);

	write_flights_to_file();
	write_reservations_to_file();
	free(current_flight);
}

void display_user_view_menu(void) {
	int* reservations = NULL;
	int* count = malloc(sizeof(int));
	error_handler(101, count, NULL);
	*count = 0;

	find_reservations(NULL, current_user.uid, &reservations, count);
	for (int i = 0; i < *count; i++) {
		show_reservation(cached_reservations[reservations[i]], 11);
	}
	if (reservations != NULL) {
		free(reservations);
	}
	if (count != NULL) {
		free(count);
	}
}

// make input_utils.c