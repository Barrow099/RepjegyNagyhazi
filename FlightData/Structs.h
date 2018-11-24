//
// Created by barrow099 on 11/24/18.
//

#ifndef NAGYHAZI_STRUCTS_H
#define NAGYHAZI_STRUCTS_H

#include <GenericList.h>

typedef enum Menus {
    NORMAL, VEGAN, LAKTOZ
} Menus;

typedef struct RuntimeState {
    GenericList *flight_list;
    GenericList *book_list;
} RuntimeState;

typedef struct FlightBook {
    string name;
    string email;
    string flightnum;
    Menus menu;
    int seat;
} FlightBook;

typedef struct FlightData {
    string from;
    string to;
    string fligth_number;
    string date;
    string time;
    int seats;
    int seats_taken;
} FlightData;
#endif //NAGYHAZI_STRUCTS_H
