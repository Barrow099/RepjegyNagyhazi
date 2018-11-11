//
// Created by barrow099 on 10/25/18.
//



#ifndef NAGYHAZI_FLIGHTDATA_H
#define NAGYHAZI_FLIGHTDATA_H


#include "../types.h"

typedef struct FligthData {
    string from;
    string to;
    string fligth_number;
    string date;
    string time;
    int seats;
    int seats_taken;
} FlightData;

FlightData* fd_alloc_flight();

void fd_load_from_file();
void fd_add_fligth(string from, string to, string flight_num, int seats, int seats_taken);
void fd_add_flight(FlightData* data);

#endif //NAGYHAZI_FLIGHTDATA_H