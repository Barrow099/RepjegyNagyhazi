//
// Created by barrow099 on 10/25/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <Menu/menu.h>
#include <string-utils.h>
#include <Menu/intelligent_menu.h>
#include <string.h>
#include "FlightData.h"
#include "FlightBook.h"
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>


void fd_load_from_file(RuntimeState *pState) {
    GenericList* fligth_list = gl_create_list();
    FILE* file = fopen("data/flights.dat","rt");
    if(file) {
        char line[128];

        while(fgets(line,128,file)) {
            FlightData* fd =fd_alloc_flight();
            string l = strrmc(line, '\n');
            sscanf(l,"%[^;];%[^;];%[^;];%[^;];%[^;];%d;%d",fd->from, fd->to, fd->date, fd->time,fd->fligth_number, &(fd->seats), &(fd->seats_taken));
            free(l);
            fligth_list->append(fligth_list,fd);
        }
    }else{
        menu_message("Nincs mentett jarat");
    }
    pState->flight_list = fligth_list;
}

void fd_save_to_file(RuntimeState *pState) {
    FILE* file = fopen("data/flights.dat","wt");
    if(!file) {
        menu_message("Nem lehet elmenteni az adatokat");
        return;
    }

    GenericItem* itm = pState->flight_list->HEAD;
    while(itm) {
        FlightData* data = itm->value;
        fprintf(file,"%s;%s;%s;%s;%s;%d;%d\n",data->from,data->to,data->date,data->time,data->fligth_number,data->seats,data->seats_taken);
        itm = itm->next_item;
    }

    fclose(file);
};


FlightData *fd_alloc_flight() {
    FlightData *data = malloc(sizeof(FlightData));
    data->from = malloc(64* sizeof(char));
    data->to = malloc(64* sizeof(char));
    data->date = malloc(64* sizeof(char));
    data->time = malloc(64* sizeof(char));
    data->fligth_number = malloc(64* sizeof(char));
    data->seats = 0;
    data->seats_taken = 0;
}

void fd_add_flight(RuntimeState *pState, FlightData *data) {
    pState->flight_list->append(pState->flight_list,data);
}

void fd_free_flight(FlightData* flight) {
    free(flight->from);
    free(flight->to);
    free(flight->date);
    free(flight->time);
    free(flight->fligth_number);
    free(flight);
}

FlightData* find_flight_by_flightnum(string fn, RuntimeState *pState) {
    FlightData* fd = NULL;
    GenericItem *head = pState->flight_list->HEAD;
    while (head) {
        if (streqq(((FlightData *) head->value)->fligth_number, fn) == 100) {
            fd = head->value;
        }
        head = head->next_item;
    }
    return fd;
}



string fd_honnan(void *data) {
    FlightData *d = (FlightData *) data;
    return d->from;
}
string fd_hova(void *data) {
    FlightData *d = (FlightData *) data;
    return d->to;
}
string fd_flightnum(void *data) {
    FlightData *d = (FlightData *) data;
    return d->fligth_number;
}

void *lsf_get_item_by_index(void *list, int index) {
    GenericList *lst = (GenericList *) list;
    return lst->get_item(lst, index);
}
string lsf_tostring_data(void *data) {
    FlightData *fd = (FlightData *) data;
    char *msg = malloc(sizeof(char) * 128);
    sprintf(msg, "<%s>: %s -> %s, %s %s,%d", fd->fligth_number, fd->from, fd->to, fd->date, fd->time, fd->seats);
    return msg;
}

FlightData *do_find(RuntimeState *pState, string message, string (*pFunction)(void *)) {
    string lst = menu_input(message);
    GenericList *filteredList = gl_filter_text(pState->flight_list, pFunction, lst, 75);
    if (filteredList->size(filteredList) == 0) {
        menu_message("Nincs talalat");
        return NULL;
    }
    int flt = varg_options_menu_generator("Talalatok. Nyomjon entert a visszalepeshez", filteredList,
                                          filteredList->size(filteredList), &lsf_get_item_by_index, &lsf_tostring_data);
    char msg[192];
    sprintf(msg, "Akar jegyet foglalni a %s jaratra?",
            ((FlightData *) filteredList->get_item(filteredList, flt))->fligth_number);
    int ch = varg_options_yesno(msg);
    if (ch == 0) {
        return ((FlightData *) filteredList->get_item(filteredList, flt));
    }
    return NULL;
}


void find_flight(RuntimeState *pState) {
    int ch = varg_options_menu("Kereses alapja:", "Honnan", "Hova", "Jaratszam", "Vissza", NULL);
    FlightData *flight = NULL;
    if (ch == 0) {
        flight = do_find(pState, "Honnan: ", &fd_honnan);
    } else if (ch == 1) {
        flight = do_find(pState, "Hova: ", &fd_hova);
    } else if (ch == 2) {
        flight = do_find(pState, "Jaratszam: ", &fd_flightnum);
    }

    if (flight) {
        do_book_flight(flight,pState);
    }

}

void list_flights(RuntimeState *pState) {
    int ch = varg_options_menu_generator("Menett jaratok. Nyomjon escapet a visszalepeshez", pState->flight_list,
                                         pState->flight_list->size(pState->flight_list), &lsf_get_item_by_index,
                                         &lsf_tostring_data);
    if(ch == -1)
        return;
    FlightData* flight = pState->flight_list->get_item(pState->flight_list, ch);
    if(flight) {
        int used_seats = flight->seats_taken;
        int normal, vegan, laktoz;
        normal = 0;
        laktoz = 0;
        vegan = 0;
        GenericItem* fBook = pState->book_list->HEAD;
        while (fBook) {
            FlightBook* book = fBook->value;
            if(strcmp(book->flightnum, flight->fligth_number) == 0 && book->seat != -1) {
                switch (book->menu) {
                    case 0:
                        normal++;
                        break;
                    case 1:
                        vegan++;
                        break;
                    case 2:
                        laktoz++;
                        break;
                }
            }
            fBook = fBook->next_item;
        }
        char msg[192];
        sprintf(msg,"%d hely kelt el. %d normal, %d vegan es %d laktozmentes menu kell",used_seats, normal,vegan,laktoz);
        menu_message(msg);
    }
}

void remove_flight(RuntimeState *pState) {
    int fid = varg_options_menu_generator("Menett jaratok. ENTER: Torles, ESC: VISSZA", pState->flight_list,
                                          pState->flight_list->size(pState->flight_list), &lsf_get_item_by_index,
                                          &lsf_tostring_data);
    if (fid == -1)
        return;
    pState->flight_list->remove(pState->flight_list, fid);
    fd_save_to_file(pState);
    menu_message("Jarat sikeresen eltavolitva");
}

void win_init() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

#endif


    DIR* dir = opendir("data");
    if (dir)
    {
        /* Directory exists. */
        closedir(dir);
    }
    else if (ENOENT == errno)
    {
#ifdef _WIN32
        CreateDirectory ("data", NULL);
#else
        mkdir("data", 0777);
#endif
    }
}

bool validate_new_flight(string *res, string error_msg) {
    //Validate input
    char from[64], to[64], date[64], time[64], id[64], seats[64];
    strcpy(from, res[0]);
    strcpy(to, res[1]);
    strcpy(date, res[2]);
    strcpy(time, res[3]);
    strcpy(id, res[4]);
    strcpy(seats, res[5]);

    int temp;
    if (strlen(from) == 0 || strlen(to) == 0 || strlen(date) == 0 || strlen(time) == 0 || strlen(id) == 0 ||
        strlen(seats) == 0) {
        strcpy(error_msg, "Nem lehet ures mezo");
        return false;
    }
    if (sscanf(date, "%4d.%d.%d", &temp, &temp, &temp) != 3) {
        strcpy(error_msg, "Erevnytelen datumformatum. Helyes formatum: YYYY.MM.DD");
        return false;
    }
    if (sscanf(time, "%d:%d", &temp, &temp) != 2) {
        strcpy(error_msg, "Erevnytelen idoformatum. Helyes formatum: HH:MM");
        return false;
    }
    return true;

}

void *create_flight_from_data(string *data) {

    FlightData *flight = fd_alloc_flight();
    strcpy(flight->from, data[0]);
    strcpy(flight->to, data[1]);
    strcpy(flight->date, data[2]);
    strcpy(flight->time, data[3]);
    strcpy(flight->fligth_number, data[4]);
    sscanf(data[5], "%d", &(flight->seats));
    return flight;

}

void new_fligth(RuntimeState *pState) {
    FlightData *f = varg_input_menu("Új járat", &validate_new_flight, &create_flight_from_data, "Honnan:", "Hová:",
                                    "Dátum:", "Idő:", "Azonosító:", "Ülések száma:", 0);

    bool isOK = true;
    GenericItem *head = pState->flight_list->HEAD;
    while (head) {
        if (streqq(((FlightData *) head->value)->fligth_number, f->fligth_number) == 100) {
            isOK = false;
        }
        head = head->next_item;
    }
    if (!isOK) {
        menu_message("Ilyen jaratszammal mar van jarat");
    } else {
        fd_add_flight(pState, f);
        fd_save_to_file(pState);
        menu_message("Jarat elmentve");
    }
}
