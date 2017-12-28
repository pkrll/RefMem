/**
 * @file
 * @brief Database data structure
 */

#ifndef __DATABASE_h__
#define __DATABASE_h__

#include <stdbool.h>
#include "item.h"

enum ItemDetails {
  NAME = 1,
  DESC = 2,
  COST = 4,
  SHELF = 8,
  AMOUNT = 16
};

typedef struct database database_t;
typedef struct database_iterator database_iter_t;

typedef bool(*database_apply_fun)(item_t *item, void *data);

/**
 * @brief  Allocates and returns a new, empty database.
 *
 * @note   If the `file_path` parameter is set, the database will
 *         attempt to load from that file. On exit, all changes
 *         will be saved to the same file.
 *
 * @param  file_path Path to a file containing the database.
 *
 * @return A pointer to a new database.
 */
database_t *database_new(char *file_path);
/**
 * @brief  Adds an item to the database.
 *
 * @param  database   The database.
 * @param  item       The item to add.
 *
 * @return True if item was successfully added, otherwise false.
 */
bool database_insert_item(database_t *database, item_t *item);
/**
 * @brief Updates an item in the database.
 *
 * @param  database The database.
 * @param  old_item The original item.
 * @param  new_item The updated item.
 *
 * @return True if update was successful, otherwise false.
 */
bool database_update_item(database_t *database, item_t *old_item, item_t *new_item);
/**
 * @brief  Removes an item from the database.
 *
 * @param  database A pointer to the database to manipulate.
 * @param  item     An item.
 *
 * @return True if item was successfully removed, otherwise false.
 */
bool database_remove_item(database_t *database, item_t *item);
/**
 * @brief  Checks if the database contains an item with
 *         the name provided.
 *
 * @note   If the database parameter is NULL, the return
 *         value will be true.
 *
 * @param  database A pointer to the database.
 * @param  name     The name of the item.
 *
 * @return True if there is an item with that name in
 *         the database, otherwise false.
 */
bool database_has_item(database_t *database, char *name);
/**
 * @brief  Returns an item from the database.
 *
 * @param  database A pointer to the database.
 * @param  name     The name of the item to retrieve.
 *
 * @return A pointer to the item, if it exists.
 */
item_t *database_get_item(database_t *database, char *name);
/**
 * @brief  Checks if the storage location inputed is valid for the
 *         item in current database.
 *
 * @note   If the database parameter is NULL, the return value will
 *         be true.
 *
 * @param  database A pointer to the database.
 * @param  item     Name of the item that is to be added to the shelf.
 * @param  shelf    The shelf name.
 *
 * @return True if storage location is valid to use, otherwise false.
 */
bool database_location_is_valid(database_t *database, char *item_name, char *shelf);
/**
 * @brief Undos the latest action to the database.
 *
 * @param  database A pointer to the database.
 *
 * @return True if successful, otherwise false.
 */
bool database_undo_action(database_t *database);
/**
 * @brief  Deletes the database and all its items.
 *
 * @param  database A pointer to the database.
 *
 * @return void
 */
void database_delete(database_t *database);
/**
 * @brief  Applies a function on all of the items in the database.
 *
 * @note   This function will abort if one of the calls returns false,
 *         but if at least one call did return true, the return value
 *         will be true.
 *
 * @param  database A pointer to the database.
 * @param  fun      The function to apply on all items.
 * @param  data     An extra argument passed to each call to fun (may be NULL).
 *
 * @return True if the function is successfully applied on at least some items, otherwise false.
 */
bool database_apply(database_t *database, database_apply_fun fun, void *data);
/**
 * @brief  Determines if the database is sorted.
 *
 * @note   If the database parameter is NULL, the result will be false.
 *
 * @param  database A pointer to the database.
 *
 * @return True if the database is sorted, otherwise false.
 */
bool database_is_sorted(database_t *database);
/**
 * @brief  Sorts the database.
 *
 * @note   If the database parameter is NULL, the result will be false.
 *
 * @param  database A pointer to the database.
 *
 * @return True if the sort is successful, otherwise false.
 */
bool database_sort(database_t *database);
/**
 * @brief  Creates and returns a new iterator for the database.
 *
 * @param  database A database.
 *
 * @return A new database iterator.
 */
database_iter_t *database_iterator(database_t *database);
/**
 * @brief  Checks if there are more items in the iterator list.
 *
 * @param  iter A database iterator
 *
 * @return True if there are at least one more item, otherwise false.
 */
bool database_iterator_has_next(database_iter_t *iter);
/**
 * @brief  Retrieves the next item in the iterator list.
 *
 * @param  iter A database iterator.
 *
 * @return An item.
 */
item_t *database_iterator_next(database_iter_t *iter);
/**
 * @brief  Retrieves an item at a specific index in the iterator list.
 *
 * @note   This function is useful for when listing items and wanting to
 *         select a specific item.
 *
 * @param  iter  A database iterator.
 * @param  index The index.
 *
 * @return An item if the index is valid, otherwise NULL.
 */
item_t *database_iterator_get(database_iter_t *iter, int index);
/**
 * @brief  Returns the number of items in the iterator.
 *
 * @note   This is the size of the database.
 *
 * @param  iter A database iterator.
 *
 * @return The number of items in the iterator.
 */
int database_iterator_size(database_iter_t *iter);
/**
 * @brief Deletes an iterator.
 *
 * @param  iter A database iterator.
 *
 * @return void
 */
void database_iterator_delete(database_iter_t *iter);

#endif
