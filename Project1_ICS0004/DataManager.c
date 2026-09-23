#include <stdlib.h>
#include <stdio.h>
#include <json-c/JSON.h>
#include "Models.h"




/*struct Flight* load_flight_from_json(const char* filename) {
    FILE* fp;
    fopen_s(&fp, filename, "r");
    if (fp == NULL) return NULL;

    // Read file into buffer
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(fsize + 1);
    fread(buffer, fsize, 1, fp);
    fclose(fp);
    buffer[fsize] = '\0';

    // Parse JSON
    json_object* root = json_tokener_parse(buffer);
    free(buffer);

    if (root == NULL) return NULL;

    // Create Flight struct
    struct Flight* flight = malloc(sizeof(struct Flight));

    // Extract data
    json_object* dest = json_object_object_get(root, "destination");
    json_object* dept = json_object_object_get(root, "departure");
    json_object* num = json_object_object_get(root, "num_seats");
    json_object* seats_array = json_object_object_get(root, "seats");

    strcpy_s(flight->destination, 20, json_object_get_string(dest));
    strcpy_s(flight->departure, 20, json_object_get_string(dept));
    flight->num_seats = json_object_get_int(num);

    // Allocate and populate seats
    flight->seats = malloc(flight->num_seats * sizeof(char*));
    for (int i = 0; i < flight->num_seats; i++) {
        json_object* seat = json_object_array_get_idx(seats_array, i);
        flight->seats[i] = malloc(20);
        strcpy_s(flight->seats[i], 20, json_object_get_string(seat));
    }

    json_object_put(root);
    return flight;
}*/



json_object* serialise_flights_to_json(void) {
    json_object* root = json_object_new_object();
    json_object* flights_array = json_object_new_array_ext(100);
	json_object_object_add(root, "num_flights", json_object_new_int(num_flights));

    for (int i = 0; i < num_flights; i++) {
        json_object* flight_obj = json_object_new_object();

        json_object_object_add(flight_obj, "destination", json_object_new_string(cached_flights[i].destination));
        json_object_object_add(flight_obj, "departure", json_object_new_string(cached_flights[i].departure));
        json_object_object_add(flight_obj, "num_seats", json_object_new_int(cached_flights[i].num_seats));
        json_object_object_add(flight_obj, "flight_number", json_object_new_int(cached_flights[i].flight_number));

        json_object* seats_array = json_object_new_array();
        for (int j = 0; j < cached_flights[i].num_seats; j++) {
            json_object_array_add(seats_array, json_object_new_string( cached_flights[i].seats[j]));
        }
        json_object_object_add(flight_obj, "seats", seats_array);

        json_object_array_add(flights_array, flight_obj);
    }

    json_object_object_add(root, "flights", flights_array);

    return root;
}

void write_flights_to_file(void) {
	json_object* flights_json = serialise_flights_to_json();
	FILE* fp;
	fopen_s(&fp, "flights.json", "w");
	if (fp != NULL) {
		fprintf(fp, "%s\n", json_object_to_json_string_ext(flights_json, JSON_C_TO_STRING_PRETTY));
	}
    else {
		printf("Error opening file.\n");
        return 1;
    }
    fclose(fp);
	json_object_put(flights_json);
}

    
char read_flights_from_file(void) {
    FILE* fp;
    fopen_s(&fp, "flights.json", "r");
    fseek(fp, 0, SEEK_END);
	long file_lenght = ftell(fp);
	fseek(fp, 0, SEEK_SET);
    char* buffer = malloc(file_lenght + 1);

    if (fp != NULL) {
		fgets(buffer, file_lenght, fp);
    }
    else {
		printf("error opening file.\n");
    }
	buffer[file_lenght+1] = '\0';
    fclose(fp);
    return buffer;
}

void deserialise_flights_from_json(void) {

	cached_flights = malloc((sizeof(struct Flight)) * num_flights);
	json_object* root = json_tokener_parse(read_flights_from_file());

    json_object_object_get_ex();    
	json_object_array_get_idx();
}