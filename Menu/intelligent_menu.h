//
// Created by barrow099 on 10/24/18.
//

#include <stdbool.h>
#include "../types.h"

#ifndef NAGYHAZI_VARG_MENU_H
#define NAGYHAZI_VARG_MENU_H

int varg_options_menu(string question,...);

void *varg_input_menu(string question, bool (*pFunction)(string *,string), void *(*pFunction1)(string *),...);

#endif //NAGYHAZI_VARG_MENU_H
