/**
 * @file
 * @brief Action
 */

#ifndef __ACTION_h__
#define __ACTION_h__

#include "common.h"

typedef enum {
  NOTHING,
  ADD,
  REMOVE,
  EDIT
} action_type;

typedef struct action action_t;

/**
 * @brief  Returns a new action.
 *
 * @param  type The type of the action.
 *
 * @return A new action.
 */
action_t *action_new(action_type type);
/**
 * @brief Returns a new action with type ADD.
 *
 * @param  elem The element of the action.
 *
 * @return A new ADD action.
 */
action_t *action_new_add(obj elem);
/**
 * @brief  Returns a new action with type EDIT.
 *
 * @param  original The original element of the action.
 * @param  edited   The edited element of the action.
 *
 * @return A new EDIT action.
 */
action_t *action_new_edit(obj original, obj edited);
/**
 * @brief  Returns a new action with type REMOVE.
 *
 * @param  elem The element of the action.
 *
 * @return A new REMOVE action.
 */
action_t *action_new_remove(obj elem);
/**
 * @brief  Returns the type of the action.
 *
 * @param  action An action.
 *
 * @return The type of the action.
 */
action_type action_get_type(action_t *action);
/**
 * @brief  Returns the element of an ADD/REMOVE action.
 *
 * @param  action An action.
 *
 * @return Element of an ADD/REMOVE action.
 */
obj action_get_saved(action_t *action);
/**
 * @brief  Returns the edited lement of an EDIT action.
 *
 * @param  action An action.
 *
 * @return Edited element of an EDIT action.
 */
obj action_get_edited(action_t *action);
/**
 * @brief  Returns the original lement of an EDIT action.
 *
 * @param  action An action.
 *
 * @return Original element of an EDIT action.
 */
obj action_get_original(action_t *action);

#endif
