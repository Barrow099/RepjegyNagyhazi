//
// Created by barrow099 on 11/24/18.
//
/**
 * @file FlightBook.h
 * Foglalással kapcsolatos műveletek
 */

#ifndef NAGYHAZI_FLIGHTBOOK_H
#define NAGYHAZI_FLIGHTBOOK_H

#include "Structs.h"


/**
 * A mentett foglalások betöltése fájlból
 * @param pState Futtatási adatok, ide olvassa be a mentett adatoka
 */
void fb_load_from_file(RuntimeState* pState);
/**
 * Foglalások mentés fájlba
 * @param pState Futtatási adatok
 */
void fb_save_to_file(RuntimeState* pState);
/**
 * Egz új foglalásstruktúra létrehozása
 * @return A létrehozott adatra pointer
 */
FlightBook *fb_alloc_book();

/**
 * Egz foglalás felszabadítása a memóriából
 * @param book
 */
void fb_free_book(FlightBook *book);

/**
 * Egz foglalás megvalósításásnak levezénylése grafikusan
 * @param pState Futtatási adatok
 */
void book_flight(RuntimeState* pState);

/**
 * Egy foglalás megvalósítása grafikusan
 * @param data A járat amire foglalni akarunk
 * @param pState Futtatási adatok
 */
void do_book_flight(FlightData *data, RuntimeState* pState);

/**
 * Egy foglalás véglegesítése
 * @param pState Futtatási adatok
 */
void finalize_book(RuntimeState *pState);


#endif //NAGYHAZI_FLIGHTBOOK_H
