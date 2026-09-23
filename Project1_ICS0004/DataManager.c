#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <json-c/JSON.h>
#include "Models.h"

json_object* serialise_flights_to_json(void) {
	// Validate cached_flights
	if (cached_flights == NULL) {
		printf("Error: cached_flights is NULL. Cannot serialize.\n");
		return NULL;
	}

	if (num_flights <= 0) {
		printf("Error: num_flights is 0 or negative. Cannot serialize.\n");
		return NULL;
	}

	// Create root JSON object
	json_object* root = json_object_new_object();
	if (root == NULL) {
		printf("Error: Failed to create root JSON object.\n");
		return NULL;
	}

	json_object* flights_array = json_object_new_array_ext(100);
	if (flights_array == NULL) {
		printf("Error: Failed to create flights array.\n");
		json_object_put(root);
		return NULL;
	}

	json_object_object_add(root, "num_flights", json_object_new_int(num_flights));

	// Serialize each flight
	for (int i = 0; i < num_flights; i++) {
		json_object* flight_obj = json_object_new_object();
		if (flight_obj == NULL) {
			printf("Error: Failed to create flight object at index %d.\n", i);
			continue;
		}

		json_object_object_add(flight_obj, "destination", json_object_new_string(cached_flights[i].destination));
		json_object_object_add(flight_obj, "departure", json_object_new_string(cached_flights[i].departure));
		json_object_object_add(flight_obj, "num_seats", json_object_new_int(cached_flights[i].num_seats));
		json_object_object_add(flight_obj, "flight_number", json_object_new_int(cached_flights[i].flight_number));

		// Serialize seats
		json_object* seats_array = json_object_new_array();
		if (seats_array == NULL) {
			printf("Error: Failed to create seats array for flight %d.\n", i);
			json_object_put(flight_obj);
			continue;
		}

		// Check if seats exist before iterating
		if (cached_flights[i].seats != NULL) {
			for (int j = 0; j < cached_flights[i].num_seats; j++) {
				if (cached_flights[i].seats[j] != NULL) {
					json_object_array_add(seats_array, json_object_new_string(cached_flights[i].seats[j]));
				} else {
					printf("Warning: Seat at index %d in flight %d is NULL.\n", j, i);
					json_object_array_add(seats_array, json_object_new_string(""));
				}
			}
		} else {
			printf("Warning: seats array is NULL for flight %d.\n", i);
		}

		json_object_object_add(flight_obj, "seats", seats_array);
		json_object_array_add(flights_array, flight_obj);
	}

	json_object_object_add(root, "flights", flights_array);

	return root;
}

void write_flights_to_file(void) {
	// Validate cached_flights
	if (cached_flights == NULL) {
		printf("Error: No flights to write. cached_flights is NULL.\n");
		return;
	}

	// Serialize to JSON
	json_object* flights_json = serialise_flights_to_json();
	if (flights_json == NULL) {
		printf("Error: Failed to serialize flights to JSON.\n");
		return;
	}

	// Open file for writing
	FILE* fp;
	fopen_s(&fp, "flights.json", "w");
	if (fp == NULL) {
		printf("Error: Failed to open flights.json for writing.\n");
		json_object_put(flights_json);
		return;
	}

	// Write to file
	const char* json_string = json_object_to_json_string_ext(flights_json, JSON_C_TO_STRING_PRETTY);
	if (json_string == NULL) {
		printf("Error: Failed to convert JSON to string.\n");
		fclose(fp);
		json_object_put(flights_json);
		return;
	}

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
	fopen_s(&fp, "flights.json", "r");

	// Check if file opened successfully
	if (fp == NULL) {
		printf("Error: Failed to open flights.json for reading.\n");
		return NULL;
	}

	// Get file size
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// Check for valid file size
	if (file_length <= 0) {
		printf("Error: Invalid file size.\n");
		fclose(fp);
		return NULL;
	}

	// Allocate buffer
	char* buffer = malloc(file_length + 1);
	if (buffer == NULL) {
		printf("Error: Failed to allocate memory for file buffer.\n");
		fclose(fp);
		return NULL;
	}

	// Read file
	size_t read_size = fread(buffer, 1, file_length, fp);
	if (read_size != file_length) {
		printf("Warning: Read %zu bytes instead of %ld bytes.\n", read_size, file_length);
	}

	buffer[read_size] = '\0';
	fclose(fp);
	return buffer;
}


void deserialise_flights_from_json(void) {
	// Read file buffer
	char* buffer = read_flights_from_file();
	if (buffer == NULL) {
		printf("Error: Failed to read flights file.\n");
		return;
	}

	// Parse JSON
	json_object* root = json_tokener_parse(buffer);
	free(buffer);
	if (root == NULL) {
		printf("Error: Failed to parse JSON.\n");
		return;
	}

	// Get num_flights from JSON
	json_object* num_flights_obj = json_object_object_get(root, "num_flights");
	if (num_flights_obj == NULL) {
		printf("Error: 'num_flights' field not found in JSON.\n");
		json_object_put(root);
		return;
	}
	num_flights = json_object_get_int(num_flights_obj);

	// Allocate cached_flights array
	cached_flights = malloc((sizeof(struct Flight)) * num_flights);
	if (cached_flights == NULL) {
		printf("Error: Failed to allocate memory for flights.\n");
		json_object_put(root);
		return;
	}

	// Get flights array from JSON
	json_object* flights_array = json_object_object_get(root, "flights");
	if (flights_array == NULL) {
		printf("Error: 'flights' array not found in JSON.\n");
		free(cached_flights);
		json_object_put(root);
		return;
	}

	// Deserialize each flight
	for (int i = 0; i < num_flights; i++) {
		json_object* flight_obj = json_object_array_get_idx(flights_array, i);
		if (flight_obj == NULL) {
			printf("Error: Flight at index %d is NULL.\n", i);
			continue;
		}

		// Extract destination
		json_object* dest_obj = json_object_object_get(flight_obj, "destination");
		if (dest_obj != NULL) {
			strcpy_s(cached_flights[i].destination, 20, json_object_get_string(dest_obj));
		} else {
			printf("Warning: 'destination' not found for flight %d.\n", i);
			cached_flights[i].destination[0] = '\0';
		}

		// Extract departure
		json_object* dept_obj = json_object_object_get(flight_obj, "departure");
		if (dept_obj != NULL) {
			strcpy_s(cached_flights[i].departure, 20, json_object_get_string(dept_obj));
		} else {
			printf("Warning: 'departure' not found for flight %d.\n", i);
			cached_flights[i].departure[0] = '\0';
		}

		// Extract num_seats
		json_object* seats_count_obj = json_object_object_get(flight_obj, "num_seats");
		if (seats_count_obj != NULL) {
			cached_flights[i].num_seats = json_object_get_int(seats_count_obj);
		} else {
			printf("Warning: 'num_seats' not found for flight %d.\n", i);
			cached_flights[i].num_seats = 0;
		}

		// Extract flight_number
		json_object* flight_num_obj = json_object_object_get(flight_obj, "flight_number");
		if (flight_num_obj != NULL) {
			cached_flights[i].flight_number = json_object_get_int(flight_num_obj);
		} else {
			printf("Warning: 'flight_number' not found for flight %d.\n", i);
			cached_flights[i].flight_number = 0;
		}

		// Extract seats array
		json_object* seats_array = json_object_object_get(flight_obj, "seats");
		if (seats_array == NULL) {
			printf("Warning: 'seats' array not found for flight %d.\n", i);
			cached_flights[i].seats = NULL;
			continue;
		}

		// Allocate seats
		cached_flights[i].seats = malloc(cached_flights[i].num_seats * sizeof(char*));
		if (cached_flights[i].seats == NULL) {
			printf("Error: Failed to allocate memory for seats in flight %d.\n", i);
			continue;
		}

		// Deserialize each seat
		for (int j = 0; j < cached_flights[i].num_seats; j++) {
			json_object* seat_obj = json_object_array_get_idx(seats_array, j);
			if (seat_obj == NULL) {
				printf("Warning: Seat at index %d in flight %d is NULL.\n", j, i);
				cached_flights[i].seats[j] = malloc(seat_length);
				if (cached_flights[i].seats[j] != NULL) {
					strcpy_s(cached_flights[i].seats[j], seat_length, "");
				}
				continue;
			}

			cached_flights[i].seats[j] = malloc(seat_length);
			if (cached_flights[i].seats[j] == NULL) {
				printf("Error: Failed to allocate memory for seat %d in flight %d.\n", j, i);
				continue;
			}
			strcpy_s(cached_flights[i].seats[j], seat_length, json_object_get_string(seat_obj));
		}
	}
	json_object_put(root);
}



