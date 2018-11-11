#include <stdio.h>
#include <string.h>
#include <Menu/intelligent_menu.h>
#include <FlightData/FlightData.h>
#include <c-econio/econio.h>
#include <stdlib.h>
#include "Menu/menu.h"
#include "GenericList.h"

#ifdef _WIN32
#include <windows.h>
#endif


void new_fligth();

void win_init();

int main() {

    //Load stored data
    win_init();
    fd_load_from_file();
    econio_set_title("Repulojegyek | Nagyhazi");


    //GenericList* list = gl_create_list();
    //list->insert(list,&"ALMA",0);
    //list->insert(list,&"Bana",1);
    //gl_delete_list(list);

    menu_message("Time to attach debugger");
    while(1) {
        int i = varg_options_menu("Kerem valasszon uzemmodot", "Felhasznaloi mod", "Adminisztratori mod", "Bezaras",
                                  NULL);
        if (i == 2) {
            return 0;
        } else if (i == 0) {
            //User mode
            varg_options_menu("NINCS IMPLEMENTALVA", "Vissza", NULL);
        } else if (i == 1) {
            while(1) {
                int ac = varg_options_menu("Adminisztratori menu", "Repules adatok", "Repter adatok", "Vissza", NULL);
                if (ac == 2) {
                    break;
                }else if(ac == 0) {
                    int fc = varg_options_menu("Repules adatok","Repules adatok listazasa","Uj repules hozzaadasa","Meglevo repules modositasa","Repules torlese","Vissza",NULL);
                    if(fc  == 1) {
                        new_fligth();
                    }
                }
            }
        }
    }
}

void win_init() {
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif
}

bool validate_new_flight(string* res, string error_msg) {
    //Validate input
    char from[64], to[64], date[64], time[64], id[64], seats[64];
    strcpy(from,res[0]);
    strcpy(to,res[1]);
    strcpy(date,res[2]);
    strcpy(time,res[3]);
    strcpy(id,res[4]);
    strcpy(seats,res[5]);

    int temp;
    if(strlen(from) == 0 || strlen(to) == 0 || strlen(date) == 0 || strlen(time) == 0 || strlen(id) == 0 || strlen(seats) == 0) {
        strcpy(error_msg, "Nem lehet ures mezo");
        return false;
    }
    if(sscanf(date,"%4d.%d.%d",&temp,&temp,&temp) != 3) {
        strcpy(error_msg, "Erevnytelen datumformatum. Helyes formatum: YYYY.MM.DD");
        return false;
    }
    if(sscanf(time,"%d:%d",&temp,&temp) != 2) {
        strcpy(error_msg, "Erevnytelen idoformatum. Helyes formatum: HH:MM");
        return false;
    }
    return true;

}
void* create_flight_from_data(string* data) {

    FlightData* flight  = fd_alloc_flight();
    strcpy(flight->from,data[0]);
    strcpy(flight->to,data[1]);
    strcpy(flight->date,data[2]);
    strcpy(flight->time,data[3]);
    strcpy(flight->fligth_number,data[4]);
    sscanf(data[5],"%d",&(flight->seats));
    return flight;

}

void new_fligth() {
    FlightData* f = varg_input_menu("Új járat", &validate_new_flight, &create_flight_from_data,"Honnan:","Hová:","Dátum:","Idő:","Azonosító:","Ülések száma:",0);
    printf("%s",f->from);
}

//Helper
