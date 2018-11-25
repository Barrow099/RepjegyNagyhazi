//
// Created by barrow099 on 11/24/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string-utils.h>
#include <Menu/menu.h>
#include <Menu/intelligent_menu.h>
#include "FlightBook.h"
#include "FlightData.h"

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


void *lbf_get_item_by_index(void *list, int index) {
    GenericList *lst = (GenericList *) list;
    return lst->get_item(lst, index);
}
string lbf_tostring_data(void *data) {
    FlightBook *fd = (FlightBook *) data;
    char *msg = malloc(sizeof(char) * 128);
    sprintf(msg, "%s - %s", fd->name, fd->flightnum);
    return msg;
}
string lbf_email(void* data) {
    return ((FlightBook*)data)->email;
}
string lbf_seatnum(void* data) {
    int i = (int) data;
    char* msg = malloc(sizeof(char) * 32);
    sprintf(msg,"Szek %d.",i);
    return msg;
}

void finalize_book(RuntimeState *pState) {
    string email = menu_input("Irja be az email cimet:");
    GenericList* filt1 = gl_filter_text(pState->book_list,&lbf_email, email,100);
    GenericList* filt = gl_create_list();
    GenericItem* item = filt1->HEAD;
    while(item) {
        FlightBook* book = item->value;
        if(book->seat == -1)
            filt->append(filt,item->value);
        item = item->next_item;
    }
    gl_delete_list(filt1);

    if(filt->size(filt) == 0) {
        menu_message("Nincs foglalas ilyen email cimmel");
    }else {
        int ch =varg_options_menu_generator("Valassza ki a veglegesiteni kivant repulest",filt,filt->size(filt),&lbf_get_item_by_index, &lbf_tostring_data);
        FlightBook *book = filt->get_item(filt,ch);
        if(book) {
            GenericList *seats = gl_create_list();
            FlightData* d = find_flight_by_flightnum(book->flightnum,pState);
            if(d) {
                GenericItem *rp = pState->book_list->HEAD;
                int counter = 1;
                while(rp && (counter < (d->seats))) {
                    FlightBook* boosk = (FlightBook*) rp->value;
                    if(boosk->seat != counter)
                        seats->append(seats,counter);
                    counter++;
                }
                int chs = varg_options_menu_generator("Valasszon szeket: ",seats, seats->size(seats),&lbf_get_item_by_index,&lbf_seatnum);
                int seat = (int) seats->get_item(seats, chs);
                book->seat = seat;
                int menu = varg_options_menu("Valasszon menut:","Alap","Vegan","Laktozmentes",NULL);
                book->menu = menu;
                d->seats_taken = d->seats_taken + 1;
                fb_save_to_file(pState);
                fd_save_to_file(pState);
                menu_message("Foglalas veglegesitve. Jo utat");
            } else {
                menu_message("Ervenytelen foglalas");
            }
        }
    }
    gl_delete_list(filt);

}

void book_flight(RuntimeState* pState) {
    string fn = menu_input("Adja meg a jaratszamot vagy hasznalja az intelligens jaratkeresot");
    FlightData* fd = NULL;
    GenericItem *head = pState->flight_list->HEAD;
    while (head) {
        if (streqq(((FlightData *) head->value)->fligth_number, fn) == 100) {
            fd = head->value;
        }
        head = head->next_item;
    }
    if(fd)
        do_book_flight(fd,pState);
    else
        menu_message("Nincs ilyen jarat");
}

bool validate_new_book(string* data, string err) {
    //Validate input
    char name[64], email[64];
    strcpy(name, data[0]);
    strcpy(email, data[1]);

    int temp;
    if (strlen(name) == 0 || strlen(email) == 0 ) {
        strcpy(err, "Nem lehet ures mezo");
        return false;
    }
    return true;
}

void* create_book_from_data(string* data) {
    char name[64], email[64];
    strcpy(name, data[0]);
    strcpy(email, data[1]);
    FlightBook* book = fb_alloc_book();
    strcpy(book->name, name);
    strcpy(book->email, email);
    return book;
}
void do_book_flight(FlightData *data, RuntimeState* pState) {
    char msg[192];
    sprintf(msg,"Repjegy foglalas a(z) %s jaratra", data->fligth_number);
    if(data->seats_taken == data->seats)
        menu_message("Erre a jaratra mar nincs jegy");
    else {
        FlightBook* book = varg_input_menu(msg,&validate_new_book,&create_book_from_data,"Nev: ", "Emai cim: ",NULL);
        if(!book) {
            menu_message("Error");
            return;
        }
        strcpy(book->flightnum, data->fligth_number);
        pState->book_list->append(pState->book_list,book);
        fb_save_to_file(pState);
        menu_message("Foglalas mentve.");
    }

}
