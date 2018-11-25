//
// Created by barrow099 on 10/25/18.
//
/**
 * @file GenericList.h
 * Egy generikus (void*) linkelt lista megvalósítás
 */
#ifndef NAGYHAZI_GENERICLIST_H
#define NAGYHAZI_GENERICLIST_H

#include <stdbool.h>


/**
 * Egy listaelem tárolására szolgáló struktúra
 * Tartalmaz egy értékpointert és egy pointert a következő elemre
 */
typedef struct GenericItem {
    /**
     * Értékpointer
     */
    void* value;

    /**
     * Pointer a lista következő elemére
     */
    struct GenericItem* next_item;
} GenericItem;

/**
 * Egy generikus lista tárolására szolgáló struktúra
 * Tartalmaz egy pointert az első elemre és függvénypointereket a lista kezelésére az áttekinthetőség kedvéért
 */
typedef struct GenericList {
    GenericItem* HEAD;
    //Functions

    /**
     * Egy listaelem lekérése
     * @param lst A pointer a listára
     * @param index Az elem indexe
     * @return Egy void* az értékre vagy NULL
     */
    void *(*get_item)(struct GenericList * lst, int index);


    /**
     * Érték beszúrása a listába
     * @param list A pointer a listára
     * @param item Értékpointer
     * @param index A beszúrás helzének indexe
     * @return true, ha sikeres volt a beszúrás
     */
    bool (*insert)(struct GenericList* list, void* item, int index);


    /**
     * A lista méretének lekérése
     * @param list A lista
     * @return A lista mérete
     */
    int (*size)(struct GenericList* list);

    /**
     * Elem hozzáfűzése a lista végéhez
     * @param list Lista pointer
     * @param item Értékpointer
     * @return true, ha sikerült a hozzáfűzés
     */
    bool (*append)(struct GenericList* list, void* item);

    /**
     * Egy eghatározott listaelem törlése
     * @param list A lista
     * @param index A törölni kívánt elem indexe
     */
    void (*remove)(struct GenericList* list, int index);

} GenericList;

/**
 * üres lista létrehozása
 * @return Pointer a listára
 */
GenericList* gl_create_list();
/**
 * Egy listának és elemeinek felszabadítása
 * @param list Pointer a listára
 */
void gl_delete_list(GenericList* list);

/**
 * Egy generikus lista eleminek szűrése szöveggé alakítás után egyezési százalék alapján
 * @param list A lista
 * @param pDataToString Függvénypointer egy elem szöveggé alakítására
 * @param text A referenciaszöveg
 * @param threshold A százalékérték-küszöb
 * @return Egy új lista a szűrt elemekkel
 */
GenericList* gl_filter_text(GenericList *list, char* (*pDataToString)(void* data), char* text, int threshold);

#endif //NAGYHAZI_GENERICLIST_H
