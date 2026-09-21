#include <stdlib.h>
#include <stdio.h>
#include <json-c/JSON.h>
#include "Models.h"



/*void save_flight_to_json(struct Flight* flight, const char* filename) {
    if (flight == NULL) return;

    // Create JSON object
    json_object* root = json_object_new_object();

    // Add flight data
    json_object_object_add(root, "destination", json_object_new_string(flight->destination));
    json_object_object_add(root, "departure", json_object_new_string(flight->departure));
    json_object_object_add(root, "num_seats", json_object_new_int(flight->num_seats));

    // Add seats array
    json_object* seats_array = json_object_new_array();
    for (int i = 0; i < flight->num_seats; i++) {
        json_object_array_add(seats_array, json_object_new_string(flight->seats[i]));
    }
    json_object_object_add(root, "seats", seats_array);

    // Write to file
    FILE* fp;
    fopen_s(&fp, filename, "w");
    if (fp != NULL) {
        fprintf(fp, "%s\n", json_object_to_json_string_ext(root, JSON_C_TO_STRING_PRETTY));
        fclose(fp);
    }

    json_object_put(root);  // Free JSON object
}*/

struct Flight* load_flight_from_json(const char* filename) {
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
}



serialise_flight_to_json(struct Flight* flight) {
	json_object* root = json_object_new_object();
    json_object
}