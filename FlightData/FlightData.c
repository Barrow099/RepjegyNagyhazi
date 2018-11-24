//
// Created by barrow099 on 10/25/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <Menu/menu.h>
#include <string-utils.h>
#include "FlightData.h"
#include "../GenericList.h"



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
