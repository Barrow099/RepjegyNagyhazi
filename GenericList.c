//
// Created by barrow099 on 10/25/18.
//

#include <stdlib.h>
#include <stdbool.h>
#include "GenericList.h"


GenericItem* private_get_item(GenericItem* list, int index);
GenericItem* private_create_item(void* value);
int gl_private_list_size(GenericItem *list);

void *gl_get_item(GenericList *list, int index);
int gl_list_size(GenericList *list);
bool gl_insert(GenericList *list, void *item, int index);
void gl_append(GenericList* list, void* item);

GenericList *gl_create_list() {
    GenericList *l = malloc(sizeof(GenericList));
    l->HEAD = NULL;
    l->get_item = &gl_get_item;
    l->size = &gl_list_size;
    l->insert = &gl_insert;
    l->append = &gl_append;
    return l;
}

void *gl_get_item(GenericList *list, int index) {
    return private_get_item(list->HEAD,index)->value;
}

bool gl_insert(GenericList *list, void *item, int index) {
    if(index == 0) {
        list->HEAD = private_create_item(item);
        list->HEAD->value = item;
        return true;
    }else {
        if(index >= gl_list_size(list)) return false;
        GenericItem* before = private_get_item(list->HEAD, index-1);
        GenericItem* after = before->next_item;
        GenericItem* itm = private_create_item(item);
        before->next_item = itm;
        itm->next_item = after;
        return true;
    }

}

void gl_append(GenericList* list, void* item) {
    list->insert(list,item, list->size(list) > 1 ?  list->size(list) - 1 : list->size(list) == 1 ? 1 : 0);
}

int gl_list_size(GenericList *list) {
    return gl_private_list_size(list->HEAD);
}
int gl_private_list_size(GenericItem *list) {
    int count = 1;
    GenericItem* item = list;
    if(!item)
        return 0;
    while(item->next_item) {
        item = item->next_item;
        count++;
    }

    return count;
}


void gl_delete_list(GenericList* list) {
    for(int i = gl_list_size(list); i >= 0; i++) {
        GenericItem* item = private_get_item(list->HEAD,i);
        free(item);
    }
}

GenericItem* private_get_item(GenericItem* list, int index) {
    if(gl_private_list_size(list) >= index) return NULL;

    GenericItem* item = list;
    for(int i = 1; i < index; i++) {
        item = item->next_item;
    }
    return item;
}

GenericItem* private_create_item(void* value) {
    GenericItem* itm = malloc(sizeof(GenericItem));
    itm->value = value;
    return itm;
}