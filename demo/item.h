/**
 * @file
 * @brief Item data structure
 */

#ifndef __item_h__
#define __item_h__

#include <stdbool.h>
#include "list.h"
#include "common.h"

typedef struct item item_t;

typedef bool(*item_shelf_apply_fun)(char *name, int amount, void *data);

/**
 * @brief Creates an returns an empty item.
 *
 * @return A pointer to a new item.
 */
item_t *item_new();
/**
 * @brief Copies an item.
 *
 * @note This performs a deep copy and will duplicate strings.
 *
 * @param  item The item to copy.
 *
 * @return A pointer to a new copy of the item.
 */
item_t *item_copy(item_t *item);
/**
 * @brief Returns the total amount of an item.
 *
 * @param  item An item.
 *
 * @return The total amount of an item.
 */
int item_total(item_t *item);
/**
 * @brief Sets the name of an item.
 *
 * The name parameter must be non-empty.
 *
 * @note The string passed along will be duplicated.
 *
 * @param  item The item to manipulate.
 * @param  name The new name of the item.
 *
 * @return True if the name is valid, otherwise
 *         false.
 */
bool item_set_name(item_t *item, char *name);
/**
 * @brief Retrieves the name of an item.
 *
 * @param  item An item.
 *
 * @return The name of an item.
 */
char *item_get_name(item_t *item);
/**
 * @brief Sets the description of an item.
 *
 * The desc parameter must be non-empty.
 *
 * @note The string passed along will be duplicated.
 *
 * @param  item The item to manipulate.
 * @param  name The new description.
 *
 * @return True if the description is valid,
 *         otherwise false.
 */
bool item_set_desc(item_t *item, char *desc);
/**
 * @brief Retrieves the description of an item.
 *
 * @param  item An item.
 *
 * @return The description of an item.
 */
char *item_get_desc(item_t *item);
/**
 * @brief Sets the price of an item.
 *
 * The price must be >= 0.
 *
 * @param  item The item to manipulate.
 * @param  price The new price.
 *
 * @return True if price is valid, otherwise false.
 */
bool item_set_cost(item_t *item, int cost);
/**
 * @brief Retrieves the price of an item.
 *
 * If the item == NULL, the return value will be 0.
 *
 * @param  item An item.
 *
 * @return The price of an item.
 */
int item_get_cost(item_t *item);
/**
 * @brief Adds a shelf to the item.
 *
 * The shelf will be added to the item along side the item's
 * previous shelves.
 *
 * @note   If the amount parameter is set to 0, the shelf will
 *         be available to other items.
 *
 * @param  item      A pointer to the item to update.
 * @param  shelf     The shelf name.
 * @param  amount    The item amount on that shelf.
 * @param  overwrite If true, the amount will be overwritten.
 *                   Otherwise, it will add it to the original
 *                   amount.
 *
 * @return void
 */
void item_set_shelves(item_t *item, char *shelf, int amount, bool overwrite);
/**
 * @brief Returns the total amount of an item.
 *
 * @param  item The item.
 *
 * @return The total amount.
 */
int item_total_amount(item_t *item);
/**
 * @brief Checksi f an item has any shelf associated with it.
 *
 * @param  item An item.
 *
 * @return True if the item has a shelf associated with it, otherwise false.
 */
bool item_has_shelves(item_t *item);
/**
 * @brief  Checks if an item is associated with a shelf.
 *
 * @param  item  An item.
 * @param  shelf The shelf to check.
 *
 * @return True if an item is associated with a shelf,
 *         otherwise false.
 */
bool item_has_shelf(item_t *item, char *shelf);
/**
 * @brief  Applies a function on all shelves of an item.
 *
 * @param  item An item.
 * @param  fun  The function to apply on all shelves.
 *
 * @return The OR-combined result of the function calls.
 */
bool item_apply_on_shelves(item_t *item, item_shelf_apply_fun fun, void *data);
/**
 * @brief  Returns the total number of shelves of an item.
 *
 * @note   This is not the same thing as the total amount of an item on a shelf.
 *
 * @param  item An item.
 *
 * @return The total number of shelves.
 */
int item_number_of_shelves(item_t *item);
/**
 * @brief Retrieves the all shelves of an item.
 *
 * @note  This function does not return the amount.
 *
 * @param  item The item.
 *
 * @return Names of all shelves of an item.
 */
char **item_get_shelves(item_t *item);

#endif
