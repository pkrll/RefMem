#ifndef __MACROS_h__
#define __MACROS_h__

#define Ask_name()   ask_question_string("Namn: ");
#define Ask_desc()   ask_question_string("Beskrivning: ");
#define Ask_cost()   ask_question_int("Pris: ");
#define Ask_shelf()  ask_question_shelf("Hylla: ");
#define Ask_amount() ask_question_int("Antal: ");

#define Ask_for_shelf_to_remove() ask_question_int("Vilken plats skall tas bort (0 för ingen)?\n") - 1;;

#define Show_main_menu() ask_question_menu("\nVälkommen till lagerhantering 2.0\n=================================\n\n[L]ägga till en vara\n[T]a bort en vara\n[R]edigera en vara\nÅn[g]ra senaste ändringen\nLista [h]ela varukatalogen\n[K]ontrollera databasens sortering\n[S]ortera databasen\n[A]vsluta\n\nVad vill du göra idag?: ", Main_Menu);
#define Show_edit_menu() ask_question_menu("\n[N]amn\n[B]eskrivning\n[P]ris\n[L]agerhylla och Antal\n[A]vbryt\n\nVälj rad eller [a]vbryt: ", Edit_Menu);

#define Ask_to_save() ask_question_menu("\nVill du spara varan?\n[J]a\n[N]ej\n[R]edigera\n\n", Save_Menu);

#define Print_name(a) printf("Namn: %s\n", a);
#define Print_desc(a) printf("Beskrivning: %s\n", a);
#define Print_cost(a) printf("Pris: %d\n", a);

#define Print_did_insert(a)     printf("\nVaran '%s' har lagts till i databasen!\n", a);
#define Print_did_not_insert(a) printf("\nVaran '%s' kunde inte läggas till i databasen!\n", a);
#define Print_did_update(a)     printf("\nVaran '%s' har uppdaterats!\n", a);
#define Print_did_not_update(a) printf("\nVaran '%s' kunde inte uppdateras!\n", a);
#define Print_did_remove()      printf("\nVaran har tagits bort!\n");
#define Print_item_exists(a)    printf("\n%s finns redan i databasen.\nAnvänder samma pris och beskrivning.\n\n", a);
#define Print_add_goods_title() printf("\nLägg till vara\n==============\n\n");
#define Print_shelf_occupied(a) printf("\nHyllplatsen '%s' är upptagen. Försök igen.\n", a);
#define Print_item_exists_on_shelves(a) printf("\n%s finns lagrad på följande platser:\n\n", a);

#define Print_wrong_choice() printf("\nFelaktigt val. Försök igen!\n");
#define Print_delimiter()    printf("\n================\n");

#define MAX_LIST_COUNT 20

#endif
