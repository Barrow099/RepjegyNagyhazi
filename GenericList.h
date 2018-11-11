//
// Created by barrow099 on 10/25/18.
//
/**
 * @file GenericList.h
 * Implements a generic linked list
 */
#ifndef NAGYHAZI_GENERICLIST_H
#define NAGYHAZI_GENERICLIST_H

#include <stdbool.h>

/**
 * Struct to store an item.
 * It has a generic pointer value and a pointer to the next item
 */
typedef struct GenericItem {
    /**
     * Pointer to the item value
     */
    void* value;

    /**
     * Pointer to the next item in the list
     */
    struct GenericItem* next_item;
} GenericItem;

/**
 * Struct to store a generic list
 * It has an item pointer to the list head and function pointers to utility functions
 */
typedef struct GenericList {
    GenericItem* HEAD;
    //Functions

    /**
     * Funtion to get an item from a list
     * @param lst A pointer to a generic list
     * @param index Index of the item
     * @return A generic pointer to the value
     */
    void *(*get_item)(struct GenericList * lst, int index);


    /**
     * Function to insert a value to a generic list
     * @param list A pointer to a generic list
     * @param item Pointer to the value you want to insert
     * @param index Index of the place where the item will be placed
     * @return True if the insertion was successful
     */
    bool (*insert)(struct GenericList* list, void* item, int index);


    /**
     * Gets the size of a generic list
     * @param list The list
     * @return The size
     */
    int (*size)(struct GenericList* list);

    /**
     * Appends item to the end of the list
     * @param list Pointer to the list you want to append
     * @param item Pointer to the item
     */
    void (*append)(struct GenericList* list, void* item);

} GenericList;

/**
 * Create a empty list
 * @return Pointer to the list
 */
GenericList* gl_create_list();

/**
 * Delete and free a list and its items
 * @param list Pointer to the list you want to delete
 */
void gl_delete_list(GenericList* list);

#endif //NAGYHAZI_GENERICLIST_H
