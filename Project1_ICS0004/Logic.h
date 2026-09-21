#pragma once

int randr(int max, int min);
void generate_flight(struct Flight** flight);
void reserve_seat(struct Flight* flight, const char* seat);

