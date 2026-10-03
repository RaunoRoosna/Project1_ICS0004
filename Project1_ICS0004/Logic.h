#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Models.h"
#include "UI.h"
#include <json-c/JSON.h>

int random_number_in_range(int max, int min);
void generate_flight(struct Flight** flight);
void reserve_seat(struct Flight* flight);
int check_user(char* username);		
void clean_stdin(void);
int find_flight(int departure);
void show_flight(struct Flight flight);
void flight_to_cache(void);
int error_handler(int code, char* ptr, int debug);
void reservation_to_cache(struct Flight* flight);
int find_flight(int flight_num);
void find_existing_flights(int* array, int* elements);
void cancel_flight(int index);
int find_reservation(int flight_number, char* seat,int uid);
void find_reservations(int flight_number, int uid, int** array, int* count);
void cancel_reservation(int flight_number, char* seat);
int read_and_scan_int( int* out);
void show_reservation(struct Reservation reservation);
void cancel_reservation_ui(void);
void add_flight(void);
bool scan_string(char* string);