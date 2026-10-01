#pragma once
#include "Logic.h"

json_object* serialise_flights_to_json(void);
void write_flights_to_file(void);
char* read_flights_from_file(void);
void deserialise_flights_from_json(void);

json_object* serialise_reservations_to_json(void);
void write_reservations_to_file(void);
char* read_reservations_from_file(void);
void deserialise_reservations_from_json(void);