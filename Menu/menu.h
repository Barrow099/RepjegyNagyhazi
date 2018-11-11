//
// Created by barrow099 on 10/9/18.
//



#ifndef NAGYHAZI_MENU_H
#define NAGYHAZI_MENU_H
#include "../types.h"
#include <stdbool.h>

int menu_options(string question, string* options, int num);
void menu_message(string options);
void* menu_input_list(string question, bool (*pfV)(string*,string), void*(*pvC)(string*), string* options, int num);

#endif //NAGYHAZI_MENU_H
