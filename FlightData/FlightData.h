//
// Created by barrow099 on 10/25/18.
//



#ifndef NAGYHAZI_FLIGHTDATA_H
#define NAGYHAZI_FLIGHTDATA_H


#include <GenericList.h>
#include "../types.h"
#include "Structs.h"


FlightData* fd_alloc_flight();

void fd_load_from_file(RuntimeState *pState);
void fd_add_flight(RuntimeState *pState,FlightData* data);
void fd_save_to_file(RuntimeState *pState);

void list_flights(RuntimeState *pState);

void remove_flight(RuntimeState *pState);

void find_flight(RuntimeState *pState);
void new_fligth(RuntimeState *pState);


FlightData* find_flight_by_flightnum(string fn, RuntimeState* pState);

#endif //NAGYHAZI_FLIGHTDATA_H