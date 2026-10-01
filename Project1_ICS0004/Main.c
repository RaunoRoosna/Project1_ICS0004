#include "Logic.h"
#include "DataManager.h"
char user_action;
char admin_action;
char switch_action;
int selected_flight;

void get_username_ui(void);
void get_reserve_view_ui(void);
void get_destination_ui(void);
void get_departure_ui(void);
void random_flight_generation(void);
void get_reservation_ui(void);
void get_view_modify_ui(void);
void get_all_search_ui(void);
void get_dnd_flightnum_ui(void);
int get_flightnum_ui(int selected_flight);
void get_add_delete_ui(void);



int main() {
srand(time(NULL));
deserialise_flights_from_json();
deserialise_reservations_from_json();

show_reservation(cached_reservations[0]);

get_username_ui();

switch (access) {

case 11: // User access granted

	get_reserve_view_ui();

	switch (user_action) {
		int* reservations = NULL;
		int* count = malloc(sizeof(int));
	case 'r':
		get_destination_ui();
		get_departure_ui();

		get_reservation_ui();

		write_flights_to_file();
		write_reservations_to_file();
		break;
	case 'v':
		find_reservations(NULL, current_user.uid, &reservations, &count);
		printf("00\n");
		for (int i = 0; i < count; i++) {
			printf("01\n");
			show_reservation(cached_reservations[reservations[i]]);
		}
		break;

	break;

	case 10: // Admin access 
		while (true) {

			get_view_modify_ui();

			switch (admin_action) {
				int index;
				
			case 'v': // show all fligths in cached_flights array
				get_all_search_ui();

				switch (switch_action) {
				case 'a':
					for (int i = 0; i < num_flights; i++) {
						show_flight(*cached_flights[i]);
					}
					continue;

				case 's':
					get_dnd_flightnum_ui();

					switch (switch_action) {
					case 'd':
						get_destination_ui();
						get_departure_ui();

						int array[10] = { 0 };
						int elements[1] = { 0 };
						find_existing_flights(array, elements);
						if (elements == 0) {
							printf("No such flight exists");
						}
						else {
							for (int i = 0; i < elements[0]; i++) {
								show_flight(*cached_flights[array[i]]);
							}
						}
						continue;

					case 'f':
						index = get_flightnum_ui(selected_flight);
						show_flight(*cached_flights[index]);
						continue;
					}
				}
			case 'm':
				get_add_delete_ui();

				switch (switch_action) {
				case 'a':
					continue;

				case 'd':
					index = get_flightnum_ui(selected_flight);
					cancel_flight(index);;
					continue;
				}
			case 'e':
				exit(0);
			case 0: // Exit
				printf("Exiting program.\n");
				break;
			}
		}
	}
}

	for (int i = 0; i < current_flight->num_seats; i++) {
		free(current_flight->seats[i]);
	}
free(current_flight->seats);
return 0;
}




void get_username_ui(void) {
	do {
		printf("Username: ");
		fgets(username, sizeof(username), stdin);
		username[strcspn(username, "\n")] = 0;
		access = check_user(username);
	} while (access == 1);
}
void get_reserve_view_ui(void) {
	do {
		printf("reserve/view (r/v):");
		user_action = getchar();
		clean_stdin();
	} while (user_action != 'r' && user_action != 'v');
}

void get_destination_ui(void) {
	do {
		printf("Destination:"); // Get the destination location from the user and store it in the destination_loc variable
		fgets(destination_loc, location_length, stdin);
		destination_loc[strcspn(destination_loc, "\n")] = 0;
	} while (strlen(destination_loc) == 0);
}

void get_departure_ui(void) {
	do {
		printf("Departure:"); // Get the departure location from the user and store it in the departure_loc variable
		fgets(departure_loc, sizeof(departure_loc), stdin);
		departure_loc[strcspn(departure_loc, "\n")] = 0;
	} while (strlen(departure_loc) == 0);
}

void random_flight_generation(void) {
	generate_flight(&current_flight);
	show_flight(*current_flight);
	reserve_seat(&current_flight);
	flight_to_cache();
}

void get_flight_ui(int array[], int* elements) {
	for (int i = 0; i < *elements; i++) {
		show_flight(*cached_flights[array[i]]);
	}

	int selected_flight;
	bool out = true;
	do {
		printf("Choose flight (enter flight number):");
		if (!read_and_scan_int(&selected_flight)) {
			printf("NaN\n");
			continue;
		}
		for (int j = 0; j < *elements; j++) {
			
			if (selected_flight == cached_flights[array[j]]->flight_number) {
				out = false;
				reserve_seat(cached_flights[array[j]]);
			}
		}
		
	} while (out);
}

void get_reservation_ui(void) {
	int array[10] = { 0 };
	int* elements = malloc(sizeof(int));
	*elements = 0;
	find_existing_flights(array, elements);
	if (*elements == 0) {
		random_flight_generation();
	}
	else {
		get_flight_ui(array, elements);
	}
}

void get_view_modify_ui(void) {
	do {
		printf("all/search (a/s):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'a' && switch_action != 's');
}

void get_all_search_ui(void) {
	do {
		printf("all/search (a/s):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'a' && switch_action != 's');
}

void get_dnd_flightnum_ui(void) {
	do {
		printf("By destination and departure or by flight number (d/f):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'd' && switch_action != 'f');
}

int get_flightnum_ui(int selected_flight) {
	int index;
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

void get_add_delete_ui(void) {
	do {
		printf("add/delete (a/d):");
		switch_action = getchar();
		clean_stdin();
	} while (switch_action != 'a' && switch_action != 'd');
}