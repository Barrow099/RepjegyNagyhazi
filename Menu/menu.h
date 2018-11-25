//
// Created by barrow099 on 10/9/18.
//



#ifndef NAGYHAZI_MENU_H
#define NAGYHAZI_MENU_H

#include <stdbool.h>
#include <string-utils.h>

typedef struct OptionNode {
    struct OptionNode* nextNode;
    string value;
} OptionNode;

int menu_options(string question, string* options, int num);
int menu_options_list(string question, OptionNode* listHead);
void menu_message(string options);
string menu_input(string question);
void* menu_input_list(string question, bool (*pfV)(string*,string), void*(*pvC)(string*), string* options, int num);

#endif //NAGYHAZI_MENU_H
