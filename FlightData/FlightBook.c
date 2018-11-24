//
// Created by barrow099 on 11/24/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string-utils.h>
#include <Menu/menu.h>
#include "FlightBook.h"

FlightBook *fb_alloc_book() {
    FlightBook *b = malloc(sizeof(FlightBook));
    b->email = malloc(sizeof(char) * 128);
    b->name = malloc(sizeof(char) * 128);
    b->flightnum = malloc(sizeof(char) * 128);
    b->seat = -1;
    b->menu = NORMAL;
    return b;
}

void fb_free_book(FlightBook *book) {
    free(book->flightnum);
    free(book->name);
    free(book->email);
    free(book);
}

void fb_load_from_file(RuntimeState *pState) {
    GenericList* fligth_list = gl_create_list();
    FILE* file = fopen("data/book.dat","rt");
    if(file) {
        char line[128];

        while(fgets(line,128,file)) {
            FlightBook* fb =fb_alloc_book();
            string l = strrmc(line, '\n');
            sscanf(l,"%[^;];%[^;];%[^;];%d;%d",fb->name, fb->email, fb->flightnum, &fb->seat,&fb->menu);
            free(l);
            fligth_list->append(fligth_list,fb);
        }
    }else{
        menu_message("Nincs mentett foglalas");
    }
    pState->book_list = fligth_list;
}

void fb_save_to_file(RuntimeState *pState) {
    FILE* file = fopen("data/book.dat","wt");
    if(!file) {
        menu_message("Nem lehet elmenteni az adatokat");
        return;
    }

    GenericItem* itm = pState->book_list->HEAD;
    while(itm) {
        FlightBook* fb = itm->value;
        fprintf(file,"%s;%s;%s;%d;%d\n",fb->name, fb->email, fb->flightnum, fb->seat,fb->menu);
        itm = itm->next_item;
    }

    fclose(file);
}
