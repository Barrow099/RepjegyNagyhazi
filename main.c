#include <stdio.h>
#include <string.h>
#include <Menu/intelligent_menu.h>
#include <Structs.h>
#include <c-econio/econio.h>
#include <stdlib.h>
#include <FlightData/FlightData.h>
#include <FlightData/FlightBook.h>
#include "Menu/menu.h"
#include "GenericList.h"
#include "string-utils.h"


#ifdef _WIN32
#include <windows.h>
#endif


void new_fligth();

void win_init();

void list_flights(RuntimeState *pState);

void remove_flight(RuntimeState *pState);

void find_flight(RuntimeState *pState);

void book_flight();

void do_book_flight(FlightData *data, RuntimeState* pState);

void finalize_book(RuntimeState *pState);

int main() {

    //Load stored data
    win_init();
    //TODO Remove debugger wait
    menu_message("Time to attach debugger");

    //Create runtime state to store runtime data
    RuntimeState runtimeState;

    fd_load_from_file(&runtimeState);
    fb_load_from_file(&runtimeState);
    econio_set_title("Repulojegyek | Nagyhazi");


    while (1) {
        int i = varg_options_menu("Kerem valasszon uzemmodot", "Felhasznaloi mod", "Adminisztratori mod", "Bezaras",
                                  NULL);
        if (i == 2) {
            return 0;
        } else if (i == 0) {
            //User mode
            int ch = varg_options_menu("Felhasznalo mod", "Jaratok keresese", "Repjegy foglalas",
                                       "Foglalas veglegesitese", "Vissza", NULL);
            switch (ch) {
                case 0:
                    find_flight(&runtimeState);
                    break;
                case 1:
                    book_flight(&runtimeState);
                    break;
                case 2:
                    finalize_book(&runtimeState);
                    break;
                default:
                    break;
            }
        } else if (i == 1) {
            while (1) {
                int ac = varg_options_menu("Adminisztratori menu", "Repules adatok", "Vissza", NULL);
                if (ac == 1) {
                    break;
                } else if (ac == 0) {
                    int fc = varg_options_menu("Repules adatok", "Repules adatok listazasa", "Uj repules hozzaadasa",
                                               "Repules torlese", "Vissza", NULL);

                    if (fc == 0) {
                        list_flights(&runtimeState);
                    } else if (fc == 1) {
                        new_fligth(&runtimeState);
                    } else if (fc == 2) {
                        remove_flight(&runtimeState);
                    }

                }
            }
        }
    }
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

    }

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
    varg_options_menu_generator("Menett jaratok. Nyomjon escapet a visszalepeshez", pState->flight_list,
                                pState->flight_list->size(pState->flight_list), &lsf_get_item_by_index,
                                &lsf_tostring_data);
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
    }
}

//Helper
