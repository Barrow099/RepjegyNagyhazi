//
// Created by barrow099 on 10/24/18.
//
/**
 * @file string-utils.h
 * Sztring manipulálási segédfüggvények gzűjteménye
 * Nagzrészben a grafikus menürendszer segédfüggvényei
 */

#ifndef NAGYHAZI_STRING_UTILS_H
#define NAGYHAZI_STRING_UTILS_H

typedef char* string;

/**
 * Egy sztring-tömbbről megmondja a leghosszabb elem hosszát
 * @param strs Sztring tömb pointer
 * @param num Sztring tömb eleminek száma
 * @return A lehosszabb elem hossza
 */
int stra_max(char *strs[], int num);

/**
 * Lefoglalja a megfelelő méretű memóriaterületet egz itemnum elemű sztringtömb számára
 * @param itemnum A tömb eleminek száma
 * @param item_length Egy elem maximális hossza
 * @return Pointer a sztringtömbre
 */
string* allocate_string_array(int itemnum, int item_length);

/**
 * Felszabadít egz sztringtömböt és elemeit
 * @param stra Pointer a tömbre
 * @param itemnum A tömb elemeinek száma
 */
void free_string_array(string* stra, int itemnum);

/**
 * Szóköz karakterek eltávolítása sztringekből
 * @param str A sztring
 */
void strtrm(string str);

/**
 * Megszámolja egy bizonyos karakter előfordulásinak számát egy sztringben
 * @param str A sztring
 * @param charToFind A karakter
 * @return Az előfordulások száma
 */
int strcnt(string str, char charToFind);

/**
 * Eltávolít egy bizonyos karaktert a sztringből
 * @param str A sztring
 * @param charToRemove Az eltávolítani kívánt karakter
 * @return Az új szöveg a karakterek nélkül
 */
string strrmc(string str, char charToRemove);

/**
 * Kiszámolja két sztring hasonlóságának százalékát
 * @param str A sztring
 * @param ref A referenciaszöveg
 * @return A hasonlóság százaléka
 */
int streqq(string str, string ref);

/**
 * Kicserél minden nagybetűt kisbetűre egz sztringben
 * @warning Ez a függvény módosítja a kapott sztringet
 * @param str A sztring
 */
void strlow(string str);
#endif //NAGYHAZI_STRING_UTILS_H
