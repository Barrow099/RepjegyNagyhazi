//
// Created by barrow099 on 10/24/18.
//

/**
 * @file intelligent_menu.h
 * A grafikus menürendszer használatának megkönnyítése
 * Használata opcionális
 */
#include <stdbool.h>
#include "string-utils.h"

#ifndef NAGYHAZI_VARG_MENU_H
#define NAGYHAZI_VARG_MENU_H

/**
 * Egy kérdés és változó mennyiségű válasz opció megjelenítése
 * @param question A kérdés
 * @param ... A válasz sztringek, ! az utolsónak NULLnak kell lennie
 * @return A kiválasztott válasz száma
 * @warning Az utolsónak paraméternek NULLnak kell lennie a helyes működéshez
 */
int varg_options_menu(string question, ...);

/**
 * IgenNem kérdés megjelenítése
 * @param question A kérdés
 * @return A válasz 0-igen, 1-nem
 */
int varg_options_yesno(string question);

/**
 * Egy kérdés és változó szövegbeviteli mező megjelenítés
 * @param question A kérdés
 * @param ... A válasz sztringek, ! az utolsónak NULLnak kell lennie
 * @warning Az utolsónak paraméternek NULLnak kell lennie a helyes működéshez
 * @param pValidate Függvény az adatok hitelesítéséhez aminek első paramétere egy sztring-tömb az adatokkal, a második egy sztring ahova hibaüzenetet lehet írni. A visszatérési érték hiba nélkül true
 * @param pCreateData Függvény a sztringtömbben kapott adatok egy db. void* pointerré alakítására. Pl. struktúrába töltés. A visszatérés a void*
 * @return A pCreateData által létrehozott adatpointer
 */
void *varg_input_menu(string question, bool (*pValidate)(string *, string), void *(*pCreateData)(string *), ...);

/**
 * Egy kérdés és egy listából előállított válaszoknak a megjelenítése
 * @param question A kérdés
 * @return A kiválasztott válasz száma
 * @param data_list  A lista
 * @param number_of_items A lista hossza
 * @param pDataItem Függvény egy indexedik elem lekérésére
 * @param pToString Függvény egy lekért elem szöveggé alakítáására
 * @return A kiválasztott elem indexe
 */
int varg_options_menu_generator(string question, void *data_list, int number_of_items,
                                void *(*pDataItem)(void *data_list, int index), string (*pToString)(void *data));

#endif //NAGYHAZI_VARG_MENU_H
