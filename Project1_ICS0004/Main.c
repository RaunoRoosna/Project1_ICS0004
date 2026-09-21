#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Logic.h"
#include "Models.h"
#include "DataManager.h"
#include <json-c/JSON.h>


int main() {
	srand(time(NULL));
	



	printf("Destination:"); // Get the destination location from the user and store it in the destination_loc variable
	fgets(destination_loc, 20, stdin);
	destination_loc[strcspn(destination_loc, "\n")] = 0;

	if (strlen(destination_loc) != 0) {
		printf("Departure:"); // Get the departure location from the user and store it in the departure_loc variable
		fgets(departure_loc, 20, stdin);  
		departure_loc[strcspn(departure_loc, "\n")] = 0;
	}
	else {
		return 0;
	}

	if (strlen(departure_loc) != 0) { // Generate a flight and reserve a seat
		generate_flight(&flight1); // Generates flight

		printf("The available seats are: "); // Prints the available seats to the user
		for (int i = 0; i < flight1->num_seats; i++) {
			printf("%s ", flight1->seats[i]);
		}
		printf("\n");

		printf("Choose the seat you want to book: ");
		fgets(selected_seat, sizeof(selected_seat), stdin);	
		selected_seat[strcspn(selected_seat, "\n")] = 0;
		reserve_seat(&flight1, selected_seat); // Reserves the seat the user selected and delets it from the flight struct

		//save_flight_to_json(flight1, "flight1.json");

		/*for (int i = 0; i < flight1->num_seats; i++) {
			printf("%s ", flight1->seats[i]);
		}*/

		for (int i = 0; i < flight1->num_seats; i++) {
			free(flight1->seats[i]);
		}
		free(flight1->seats);
		free(flight1);
	}
	else {
		return 0;
	}
	return 0;
}



