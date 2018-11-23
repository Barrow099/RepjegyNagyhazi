//
// Created by barrow099 on 11/13/18.
//
#include "GenericList.h"

int main_test() {
    GenericList *list = gl_create_list();
    int i1, i2;
    i1 = 1;
    i2 = 2;

    bool b1 = list->append(list,&i1);
    bool b2 = list->append(list,&i2);



    gl_delete_list(list);
    return 0;
}
