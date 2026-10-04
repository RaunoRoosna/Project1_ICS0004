#pragma once

int get_username_menu(void);
char get_reserve_view_menu(void);
char* get_destination_menu(void);
char* get_departure_menu(void);
void random_flight_generation(char* destination_loc, char* departure_loc);
void reservation_menu(char* destination_loc, char* departure_loc);
char get_view_modify_menu(void);
char get_all_search_menu(void);
char get_dnd_flightnum_menu(void);
int get_flightnum_menu(int selected_flight);
char get_add_delete_menu(void);
void case_dnd(void);
void display_reservation_menu(void);
void display_user_view_menu(void);
void display_modify_menu(void);
void display_view_menu(void);