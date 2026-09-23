#include "Logic.h"
#include "Models.h"
#include "DataManager.h"
char user_action;
char admin_action;


int main() {
	srand(time(NULL));
	
	do{
		printf("Username: ");
		fgets(username, sizeof(username), stdin);
		username[strcspn(username, "\n")] = 0;
		access = check_user(username);
		//printf("Access level: %d\n", access);
	} while (access == 1);			
	
	switch (access) {

	case 11: // User access granted

		
		do {
			printf("reserve/view (r/v):");
			user_action = getchar();
			clean_stdin();
		} while (user_action != 'r' && user_action != 'v');

		switch (user_action) {

		case 'r':
			printf("Destination:"); // Get the destination location from the user and store it in the destination_loc variable
			fgets(destination_loc, sizeof(destination_loc), stdin);
			destination_loc[strcspn(destination_loc, "\n")] = 0;

			if (strlen(destination_loc) != 0) {
				printf("Departure:"); // Get the departure location from the user and store it in the departure_loc variable
				fgets(departure_loc, sizeof(departure_loc), stdin);
				departure_loc[strcspn(departure_loc, "\n")] = 0;
			}
			else {
				printf("Destination cannot be empty. Exiting program.\n");
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

				for (int i = 0; i < flight1->num_seats; i++) {
					free(flight1->seats[i]);
				}
				free(flight1->seats);
				free(flight1);
			}
			else {
				return 0;
			}
			break;

		case 'v':

			break;

		default:
			return 1;
			break;
		}
		
		break;
		


		

	case 10: // Admin access 
		
		do {
			printf("view/modify (v/m):");
			admin_action = getchar();
			clean_stdin();
		} while (admin_action != 'v' && admin_action != 'm' && admin_action != 'e');

		switch (admin_action) {
		case 'v':
			// View flights
			break;
		case 'm':
			// Modify flights
			break;
		case 'e':
			return 0;
			break;
		default:
			return 1;
			break;
		}
		break;
		

	case 0: // Exit
		printf("Exiting program.\n");
		break;
	default:
		return 0;
	}
return 0;
}



