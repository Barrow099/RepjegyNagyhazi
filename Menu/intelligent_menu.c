//
// Created by barrow099 on 10/24/18.
//
#include "intelligent_menu.h"
#include "menu.h"
#include <stdarg.h>


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

void *varg_input_menu(string question, bool (*pfValidator)(string *,string), void *(*pfCreator)(string *), ...) {
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
