//
// Created by barrow099 on 11/24/18.
//

/**
 * @file Structs.h
 * A programohoz szükséges adatstruktúrák
 */

#ifndef NAGYHAZI_STRUCTS_H
#define NAGYHAZI_STRUCTS_H

#include "string-utils.h"
#include "GenericList.h"

/**
 * Menü típusok
 */
typedef enum Menus {
    NORMAL, VEGAN, LAKTOZ
} Menus;

/**
 * A program futás közbeni állapotát tárolja, a járatok és foglalások listáját
 */
typedef struct RuntimeState {
    GenericList *flight_list;
    GenericList *book_list;
} RuntimeState;

/**
 * Egy foglalás adatait tárol
 */
typedef struct FlightBook {
    string name;
    string email;
    string flightnum;
    Menus menu;
    int seat;
} FlightBook;


/**
 * Egy járat adatait tárolja
 */
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
