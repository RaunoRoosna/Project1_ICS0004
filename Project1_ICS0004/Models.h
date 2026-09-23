#pragma once
#include <stdlib.h>

#define seat_length 3

struct Flight {
	char destination[20];
	char departure[20];
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
	char* seat;
};

char destination_loc[20];
char departure_loc[20];
char selected_seat[20];
char username[20];
struct Flight* flight1;
struct Flight* flight2;
struct Flight* cached_flights;
struct Reservation cached_reservations[];
struct User* current_user;
struct User admin;
struct User user1;
int access;
int num_flights;
