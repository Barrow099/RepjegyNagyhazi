//
// Create by barrow099 on ??.??
//

/**
 * @file main.c
 *  Main modul
 *  A progarm fő működését és a menürendszer egy részének funkcióit biztosítja
 */
#include <intelligent_menu.h>
#include <Structs.h>
#include <c-econio/econio.h>
#include <stdlib.h>
#include <FlightData.h>
#include <FlightBook.h>


#ifdef _WIN32
#include <windows.h>
#endif



/**
 * Windowsos működéshez szükséges feladatok elvégzése
 */
void win_init();

/**
 * Main belépési pont
 * @return
 */
int main() {

    //Load stored data
    win_init();

    //menu_message("Time to attach debugger");

    //Create runtime state to store runtime data
    RuntimeState runtimeState;

    fd_load_from_file(&runtimeState);
    fb_load_from_file(&runtimeState);
    econio_set_title("Repulojegyek | Nagyhazi");


    while (1) {
        int i = varg_options_menu("Kerem valasszon uzemmodot", "Felhasznaloi mod", "Adminisztratori mod", "Bezaras",
                                  NULL);
        if (i == 2) {
            return 0;
        } else if (i == 0) {
            //User mode
            int ch = varg_options_menu("Felhasznalo mod", "Jaratok keresese", "Repjegy foglalas",
                                       "Foglalas veglegesitese", "Vissza", NULL);
            switch (ch) {
                case 0:
                    find_flight(&runtimeState);
                    break;
                case 1:
                    book_flight(&runtimeState);
                    break;
                case 2:
                    finalize_book(&runtimeState);
                    break;
                default:
                    break;
            }
        } else if (i == 1) {
            while (1) {
                int ac = varg_options_menu("Adminisztratori menu", "Repules adatok", "Vissza", NULL);
                if (ac == 1) {
                    break;
                } else if (ac == 0) {
                    int fc = varg_options_menu("Repules adatok", "Repules adatok listazasa", "Uj repules hozzaadasa",
                                               "Repules torlese", "Vissza", NULL);

                    if (fc == 0) {
                        list_flights(&runtimeState);
                    } else if (fc == 1) {
                        new_fligth(&runtimeState);
                    } else if (fc == 2) {
                        remove_flight(&runtimeState);
                    }

                }
            }
        }
        else
            break;
    }
    fb_save_to_file(&runtimeState);
    fd_save_to_file(&runtimeState);
    gl_delete_list(runtimeState.book_list);
    gl_delete_list(runtimeState.flight_list);
    return 0;
}





//Helper
