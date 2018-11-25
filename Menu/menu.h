//
// Created by barrow099 on 10/9/18.
//
/**
 * @file Grafikus menürendszer
 * Egy parancssoros grafikus menürendszer megvalósítása
 */


#ifndef NAGYHAZI_MENU_H
#define NAGYHAZI_MENU_H

#include <stdbool.h>
#include <string-utils.h>

/**
 * Linkelt lista a listás opciómegjelenítéshez
 * @warning Belső használatra
 */
typedef struct OptionNode {
    /**
     * A következő elem
     */
    struct OptionNode* nextNode;
    /**
     * sztroing érték
     */
    string value;
} OptionNode;

/**
 * Opciólista megjelenítése
 * @param question Kérdés
 * @param options Sztringtömbben az opciók
 * @param num Az opciók száma
 * @return A kiválasztott opció indexe
 */
int menu_options(string question, string* options, int num);

/**
 * Opciólista megjelenítése. Az intelligent_menu használata ajánlott
 * @param question A kérdés
 * @param listHead Linkelt listás opciók első eleme
 * @return A kiválasztott elem indexe
 */
int menu_options_list(string question, OptionNode* listHead);

/**
 * Üzenet megjelenítése a felhasználónak
 * @param message Az üzenet
 */
void menu_message(string message);

/**
 * Egyszerű kérdés feltétele
 * @param question A kérdés
 * @return sztring válasz
 */
string menu_input(string question);

/**
 * Egy kérdés és változó szövegbeviteli mező megjelenítés
 * @param question A kérdés
 * @param options sztringtömb a mezőnevekkel
 * @param num A mezőnevek száma
 * @param pfV Függvény az adatok hitelesítéséhez aminek első paramétere egy sztring-tömb az adatokkal, a második egy sztring ahova hibaüzenetet lehet írni. A visszatérési érték hiba nélkül true
 * @param pvC Függvény a sztringtömbben kapott adatok egy db. void* pointerré alakítására. Pl. struktúrába töltés. A visszatérés a void*
 * @return A pvC által létrehozott adatpointer
 */
void* menu_input_list(string question, bool (*pfV)(string*,string), void*(*pvC)(string*), string* options, int num);

#endif //NAGYHAZI_MENU_H
