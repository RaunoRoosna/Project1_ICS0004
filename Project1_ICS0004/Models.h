#pragma once
#include <stdlib.h>

#define SEAT_LENGTH 6
#define LOCATION_LENGTH 20

struct Flight {
	char destination[LOCATION_LENGTH];
	char departure[LOCATION_LENGTH];
	char** available_seats;		
	int number_of_seats;
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
	char seat[SEAT_LENGTH];
};

char destination_loc[LOCATION_LENGTH];
char departure_loc[LOCATION_LENGTH];
char selected_seat[SEAT_LENGTH];
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

