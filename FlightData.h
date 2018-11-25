//
// Created by barrow099 on 10/25/18.
//
/**
 * @file FlightData.h
 * Járat adatokkalm kapcsdlatos műveletek
 */


#ifndef NAGYHAZI_FLIGHTDATA_H
#define NAGYHAZI_FLIGHTDATA_H


#include <GenericList.h>
#include "string-utils.h"
#include "Structs.h"

/**
 * Egy járat adatstruktúra lefoglalása
 * @return Pointer a struktúrára
 */
FlightData* fd_alloc_flight();

/**
 * A mentett járatok adatainak betöltése fájlból
 * @param pState A futtáskor tárolandó adatok, ide olvassa be a járatokat
 */
void fd_load_from_file(RuntimeState *pState);
/**
 * Egy járat hozzáadása a mentett járatokhoz
 * @param pState A futtáskor tárolandó adatok, ide teszi be az új járatot
 * @param data Az új járat
 */
void fd_add_flight(RuntimeState *pState,FlightData* data);
/**
 * A tárolt járatok elmentése
 * @param pState Futtatási adatok
 */
void fd_save_to_file(RuntimeState *pState);

/**
 * A járatok kilistázása grafikusan
 * @param pState Futtatási adatok
 */
void list_flights(RuntimeState *pState);

/**
 * Egy járat törlésének lebonzolítása grafikusan
 * @param pState Futtatási adatok
 */
void remove_flight(RuntimeState *pState);

/**
 * Egy járat keresésének lebonyolítása grafikusan
 * @param pState Futtatási adatok
 */
void find_flight(RuntimeState *pState);
/**
 * Egy új járat hozzáadása grafikusan
 * @param pState
 */
void new_fligth(RuntimeState *pState);

/**
 * Egz járat megtalálása járatszám alapján
 * @param fn A keresett járatszám
 * @param pState Futtatási adatok
 * @return A megtalált járat vagy NULL
 */
FlightData* find_flight_by_flightnum(string fn, RuntimeState* pState);

#endif //NAGYHAZI_FLIGHTDATA_H