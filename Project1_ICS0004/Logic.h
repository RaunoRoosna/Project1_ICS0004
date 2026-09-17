#pragma once

int randr(int max, int min);
void generate_flight(struct Flight **flight);
void reserve_seat(struct Flight* flight, const char* seat);

void save_flight_to_json(struct Flight* flight, const char* filename);
struct Flight* load_flight_from_json(const char* filename);
void json_flight_cleanup(struct Flight* flight);