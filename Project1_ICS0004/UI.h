#pragma once

int prompt_get_username_menu(void);
char prompt_get_reserve_view_menu(void);
char* prompt_get_destination_menu(void);
char* prompt_get_departure_menu(void);
void random_flight_generation(char* destination_loc, char* departure_loc);
void reservation_menu(char* destination_loc, char* departure_loc);
char prompt_get_view_modify_menu(void);
char prompt_get_all_search_menu(void);
char prompt_get_destination_and_departure_or_flightnum_menu(void);
int prompt_get_flightnum_menu(int selected_flight);
char prompt_get_add_delete_menu(void);
void show_search_by_destination_and_departure_menu(void);
void display_reservation_menu(void);
void display_user_view_menu(void);
void display_modify_menu(void);
void display_view_menu(void);