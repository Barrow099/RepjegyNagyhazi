//
// Created by barrow099 on 10/24/18.
//
#include "intelligent_menu.h"
#include "menu.h"
#include <stdarg.h>
#include <stdlib.h>


//Question, options, last option MUST BE 'NULL'
int varg_options_menu(string question, ...) {
    va_list list;
    //assume max 10 items
    char* lst[10];
    va_start(list,question);
    char* arg = va_arg(list,char*);
    lst[0] = arg;
    int counter = 1;
    while(arg != 0) {
        arg = va_arg(list,char*);
        lst[counter++] = arg;
    }
    va_end(list);
    return menu_options(question,lst,counter-1);
}

void *varg_input_menu(string question,
                      bool (*pfValidator)(string *,string),
                      void *(*pfCreator)(string *), ...) {
    va_list list;
    //assume max 16 items
    char* lst[16];
    va_start(list,pfCreator);
    char* arg = va_arg(list,char*);
    lst[0] = arg;
    int counter = 1;
    while(arg != 0) {
        arg = va_arg(list,char*);
        lst[counter++] = arg;
    }
    va_end(list);
    return menu_input_list(question,pfValidator, pfCreator, lst, counter - 1);
}


int varg_options_menu_generator(string question,
                                void *data_list,
                                int number_of_items,
                                void *(*pDataItem)(void *, int),
                                string (*pToString)(void *)) {

    OptionNode* HEAD = malloc(sizeof(OptionNode));
    HEAD->nextNode = NULL;
    OptionNode* prevItem = HEAD;
    if(number_of_items == 0) {
        menu_message("Nincsenek adatok");
        return -1;
    }
    HEAD->value = pToString(pDataItem(data_list,0));
    for(int x = 1; x < number_of_items; x++) {
        OptionNode* item = malloc(sizeof(OptionNode));
        item->nextNode = NULL;
        item->value = pToString(pDataItem(data_list,x));
        prevItem->nextNode = item;
        prevItem = item;
    }
    return menu_options_list(question, HEAD);

}

int varg_options_yesno(string question) {
    return varg_options_menu(question,"Igen","Nem",NULL);
}
