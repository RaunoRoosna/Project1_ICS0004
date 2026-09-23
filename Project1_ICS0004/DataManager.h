#pragma once
#include <json-c/JSON.h>
#include "Models.h"

json_object* serialise_flights_to_json(void);
void write_flights_to_file(void);
