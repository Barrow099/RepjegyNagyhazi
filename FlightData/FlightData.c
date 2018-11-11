//
// Created by barrow099 on 10/25/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <Menu/menu.h>
#include "FlightData.h"
#include "../GenericList.h"

GenericList* fligth_list;

void fd_load_from_file() {
    fligth_list = gl_create_list();
    FILE* file = fopen("data/flights.dat","rt");
    if(file) {
        char line[128];

        while(fgets(line,128,file)) {
            FlightData* fd =fd_alloc_flight();
            sscanf(line,"%s;%s;%s;%s;%s;%d",fd->from, fd->to, fd->date, fd->time,fd->fligth_number, &(fd->seats));
            fligth_list->append(fligth_list,fd);
        }
    }else{
        menu_message("Nincs mentett jarat");
    }
}

void fd_save_to_file() {
    FILE* file = fopen("data/flights.dat","wt");
    if(!file) {
        menu_message("Nem lehet elmenteni az adatokat");
        return;
    }

    GenericItem* itm = fligth_list->HEAD;
    while(itm) {
        FlightData* data = itm->value;
        fprintf(file,"%s;%s;%s;%s;%s;%d\n",data->from,data->to,data->date,data->time,data->fligth_number,data->seats);
        itm = itm->next_item;
    }

    fclose(file);
};

void fd_add_flight(FlightData* data) {
    fligth_list->append(fligth_list,data);
}

FlightData *fd_alloc_flight() {
    FlightData *data = malloc(sizeof(FlightData));
    data->from = malloc(64* sizeof(char));
    data->to = malloc(64* sizeof(char));
    data->date = malloc(64* sizeof(char));
    data->time = malloc(64* sizeof(char));
    data->fligth_number = malloc(64* sizeof(char));
}

void fd_free_flight(FlightData* flight) {
    free(flight->from);
    free(flight->to);
    free(flight->date);
    free(flight->time);
    free(flight->fligth_number);
    free(flight);

}
