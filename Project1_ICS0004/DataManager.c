
#include "DataManager.h"

json_object* serialise_flights_to_json(void) {
	if ((cached_flights == NULL && num_flights > 0) || num_flights < 0) {
		error_handler(100, NULL, 1000);
	}
	if (num_flights == 0) {
		return;
	}
	json_object* root = json_object_new_object();
	json_object* flights_array = json_object_new_array_ext(num_flights);
	json_object_object_add(root, "num_flights", json_object_new_int(num_flights));
	for (int i = 0; i < num_flights; i++) { 	// Serialise each flight
		error_handler(101, cached_flights[i], NULL);
		
		json_object* flight_obj = json_object_new_object();

		json_object_object_add(flight_obj, "destination", json_object_new_string(cached_flights[i]->destination));
		json_object_object_add(flight_obj, "departure", json_object_new_string(cached_flights[i]->departure));
		json_object_object_add(flight_obj, "num_seats", json_object_new_int(cached_flights[i]->number_of_seats));
		json_object_object_add(flight_obj, "flight_number", json_object_new_int(cached_flights[i]->flight_number));
		
		json_object* seats_array = json_object_new_array();
		if (cached_flights[i]->available_seats != NULL && cached_flights[i]->number_of_seats > 0) { 	// Check if seats exist before iterating
			for (int j = 0; j < cached_flights[i]->number_of_seats; j++) {
				if (cached_flights[i]->available_seats[j] != NULL) {
					json_object_array_add(seats_array, json_object_new_string(cached_flights[i]->available_seats[j]));
				} else {
					printf("Warning: Seat at index %d in flight %d is NULL.\n", j, i);
					json_object_array_add(seats_array, json_object_new_string(""));
				}
			}
		} else { 
			printf("Error: No available seats for flight %d.\n", i);
		}
		json_object_object_add(flight_obj, "seats", seats_array);
		json_object_array_add(flights_array, flight_obj);
	}
	json_object_object_add(root, "flights", flights_array);

	return root;
}

void write_flights_to_file(void) {
	json_object* flights_json = serialise_flights_to_json();
	// Open file for writing
	FILE* fp;
	fopen_s(&fp, "Resources/Flights.json", "w");
	error_handler(101, fp, NULL);
	// Write to file
	const char* json_string = json_object_to_json_string_ext(flights_json, JSON_C_TO_STRING_PRETTY);
	if (fprintf(fp, "%s\n", json_string) < 0) {
		printf("Error: Failed to write to file.\n");
	} else {
		printf("Flights successfully written to flights.json.\n");
	}
	fclose(fp);
	json_object_put(flights_json);
}

    
char* read_flights_from_file(void) {
	FILE* fp;
	fopen_s(&fp, "Resources/Flights.json", "r");
	error_handler(101, fp, NULL);

	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buffer = malloc(file_length + 1);
	error_handler(101, buffer, 1001);

	// Read file
	size_t read_size = fread(buffer, 1, file_length, fp);

	buffer[read_size] = '\0';
	fclose(fp);
	return buffer;
}


void deserialise_flights_from_json(void) {
	char* buffer = read_flights_from_file();
	json_object* root = json_tokener_parse(buffer);
	free(buffer);
	json_object* num_flights_obj = json_object_object_get(root, "num_flights");

	num_flights = json_object_get_int(num_flights_obj);

	// Allocate cached_flights array of pointers
	cached_flights = malloc(sizeof(struct Flight*) * num_flights);
	error_handler(101, cached_flights, 1002);
	json_object* flights_array = json_object_object_get(root, "flights");

	// Deserialize each flight
	for (int i = 0; i < num_flights; i++) {
		json_object* flight_obj = json_object_array_get_idx(flights_array, i);

		// Allocate individual Flight struct
		cached_flights[i] = malloc(sizeof(struct Flight));
		error_handler(101, cached_flights[i], NULL);

		json_object* dest_obj = json_object_object_get(flight_obj, "destination");
			strcpy_s(cached_flights[i]->destination, 20, json_object_get_string(dest_obj));

		json_object* dept_obj = json_object_object_get(flight_obj, "departure");
			strcpy_s(cached_flights[i]->departure, 20, json_object_get_string(dept_obj));

		json_object* seats_count_obj = json_object_object_get(flight_obj, "num_seats");
			cached_flights[i]->number_of_seats = json_object_get_int(seats_count_obj);

		json_object* flight_num_obj = json_object_object_get(flight_obj, "flight_number");
			cached_flights[i]->flight_number = json_object_get_int(flight_num_obj);

		json_object* seats_array = json_object_object_get(flight_obj, "seats");

		if (cached_flights[i]->number_of_seats == 0) {
			cached_flights[i]->available_seats = NULL;
			continue;
		}
			// Allocate seats
			cached_flights[i]->available_seats = malloc(cached_flights[i]->number_of_seats * sizeof(char*));
			error_handler(101, cached_flights[i]->available_seats, 1003);


			// Deserialize each seat
			for (int j = 0; j < cached_flights[i]->number_of_seats; j++) {
				json_object* seat_obj = json_object_array_get_idx(seats_array, j);
				cached_flights[i]->available_seats[j] = malloc(SEAT_LENGTH * sizeof(char));
				error_handler(101, cached_flights[i]->available_seats[j], 1004);
				strcpy_s(cached_flights[i]->available_seats[j], SEAT_LENGTH, json_object_get_string(seat_obj));
			}
	}
	json_object_put(root);
}


json_object* serialise_reservations_to_json(void) {
	if ((cached_reservations == NULL && num_reservations > 0) || num_reservations < 0) {
		error_handler(100, NULL, NULL);
	}

	if (num_reservations == 0) {
		return NULL;
	}
	json_object* root = json_object_new_object();
	json_object* reservations_array = json_object_new_array_ext(num_reservations);
	json_object_object_add(root, "num_reservations", json_object_new_int(num_reservations));
	for (int i = 0; i < num_reservations; i++) { 	// Serialise each reservation
		json_object* reservation_obj = json_object_new_object();

		json_object_object_add(reservation_obj, "uid", json_object_new_int(cached_reservations[i].uid));
		json_object_object_add(reservation_obj, "flight_number", json_object_new_int(cached_reservations[i].flight_number));
		json_object_object_add(reservation_obj, "seat", json_object_new_string(cached_reservations[i].seat));

		json_object_array_add(reservations_array, reservation_obj);
	}
	json_object_object_add(root, "reservations", reservations_array);
	return root;
}

void write_reservations_to_file(void) {
	json_object* reservations_json = serialise_reservations_to_json();


	// Open file for writing
	FILE* fp;
	fopen_s(&fp, "Resources/Reservations.json", "w");
	error_handler(101, fp, NULL);

	// Write to file
	const char* json_string = json_object_to_json_string_ext(reservations_json, JSON_C_TO_STRING_PRETTY);

	if (fprintf(fp, "%s\n", json_string) < 0) {
		printf("Error: Failed to write to file.\n");
	}
	else {
		printf("Reservations successfully written to reservations.json.\n");
	}

	fclose(fp);
	json_object_put(reservations_json);
}

char* read_reservations_from_file(void) {

	FILE* fp;
	fopen_s(&fp, "Resources/Reservations.json", "r");
	error_handler(101, fp, NULL);

	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* buffer = malloc(file_length + 1);
	error_handler(101, buffer, 1001);

	// Read file
	size_t read_size = fread(buffer, 1, file_length, fp);

	buffer[read_size] = '\0';
	fclose(fp);
	return buffer;
}


void deserialise_reservations_from_json(void) {
	char* buffer = read_reservations_from_file();
	json_object* root = json_tokener_parse(buffer);
	free(buffer);
	json_object* num_reservations_obj = json_object_object_get(root, "num_reservations");

	num_reservations = json_object_get_int(num_reservations_obj); 

	cached_reservations = malloc(((sizeof(struct Reservation)) + 1) * num_reservations);
	error_handler(101, cached_reservations, 1002);
	json_object* reservations_array = json_object_object_get(root, "reservations");

	for (int i = 0; i < num_reservations; i++) { // Deserialize each reservation
		json_object* reservation_obj = json_object_array_get_idx(reservations_array, i);

		json_object* uid_obj = json_object_object_get(reservation_obj, "uid");
		cached_reservations[i].uid = json_object_get_int(uid_obj);

		json_object* flight_num_obj = json_object_object_get(reservation_obj, "flight_number");
		cached_reservations[i].flight_number = json_object_get_int(flight_num_obj);

		json_object* seat_obj =json_object_object_get(reservation_obj, "seat");
		strcpy_s(cached_reservations[i].seat, SEAT_LENGTH, json_object_get_string(seat_obj));
	}

	json_object_put(root);
}