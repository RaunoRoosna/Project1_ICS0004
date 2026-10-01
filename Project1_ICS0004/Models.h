#pragma once
#include <stdlib.h>

#define seat_length 3
#define location_length 20

struct Flight {
	char destination[location_length];
	char departure[location_length];
	char** seats;		
	int num_seats;
	int flight_number;
};

struct User {
	char first_name[20];
	char last_name[20];
	char username[20];
	int uid;
	char* password;
};

struct Reservation {
	int uid;
	int flight_number;
	char seat[seat_length];
};

char destination_loc[location_length];
char departure_loc[location_length];
char selected_seat[seat_length];
char username[20];
int access;

int num_flights;
int num_reservations;

struct Flight** cached_flights;
struct Reservation* cached_reservations;

struct Flight* current_flight;
struct User current_user;
struct Reservation current_reservation;

struct User admin;
struct User user1;

