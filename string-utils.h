//
// Created by barrow099 on 10/24/18.
//
/**
 * @file string-utils.h
 * Utility function for manipulating strings and string arrays
 * Mostly used by menu.h
 */

#ifndef NAGYHAZI_STRING_UTILS_H
#define NAGYHAZI_STRING_UTILS_H

#include "types.h"

/**
 * Function to determine the longest string's length in an array of char arrays (string array)
 * @param strs Pointer to the string array
 * @param num Number of items in the string array
 * @return The length of the longest string in the array
 */
int stra_max(char *strs[], int num);

/**
 * Allocates memory for a string array of `itemnum` items
 * @param itemnum Number of strings you want to store
 * @param item_length Maximum length of the string
 * @return Pointer to the string array
 */
string* allocate_string_array(int itemnum, int item_length);

/**
 * Free a string array allocated with allocate_string_array
 * @param stra Pointer to the string array
 * @param itemnum Number of items in the array
 */
void free_string_array(string* stra, int itemnum);
void strrmv(string, int);
void strtrm(string);

#endif //NAGYHAZI_STRING_UTILS_H
