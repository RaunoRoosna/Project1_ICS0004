#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "Models.h"
#include <json-c/JSON.h>

int randr(int max, int min);
void generate_flight(struct Flight** flight);
void reserve_seat(struct Flight* flight, const char* seat);
int check_user(char* username);		
void clean_stdin(void);

