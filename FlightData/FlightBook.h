//
// Created by barrow099 on 11/24/18.
//

#ifndef NAGYHAZI_FLIGHTBOOK_H
#define NAGYHAZI_FLIGHTBOOK_H

#include "Structs.h"



void fb_load_from_file(RuntimeState* pState);
void fb_save_to_file(RuntimeState* pState);
FlightBook *fb_alloc_book();
void fb_free_book(FlightBook *book);

#endif //NAGYHAZI_FLIGHTBOOK_H
