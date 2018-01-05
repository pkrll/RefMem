#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "action.h"

// -------------------------------
// Structs & Enums
// -------------------------------

struct action {

  action_type type;

  union {
    struct {
      obj saved;
    };

    struct {
      obj edited;
      obj original;
    };

  };
};

// -------------------------------
// Public
// -------------------------------

void action_destructor(obj object) {
  action_t *action = (action_t *)object;
  if (action->type == EDIT) {
    release(action->edited);
    release(action->original);
  } else if (action->type == ADD || action->type == REMOVE) {
    release(action->saved);
  }
}

action_t *action_new(action_type type) {
  action_t *action = allocate(sizeof(action_t), action_destructor);

  if (action) action->type = type;

  return action;
}

action_t *action_new_add(obj elem) {
  action_t *action = action_new(ADD);

  if (action) {
    action->saved = elem;
    retain(action->saved);
  }

  return action;
}

action_t *action_new_edit(obj original, obj edited) {
  action_t *action = action_new(EDIT);

  if (action) {
    action->edited = edited;
    action->original = original;
    retain(action->edited);
    retain(action->original);
  }

  return action;
}

action_t *action_new_remove(obj removed) {
  action_t *action = action_new(REMOVE);

  if (action) {
    action->saved = removed;
    retain(action->saved);
  }

  return action;
}

action_type action_get_type(action_t *action) {
  if (action) {
    return action->type;
  }

  return NOTHING;
}

obj action_get_saved(action_t *action) {
  assert(action);

  return action->saved;
}

obj action_get_edited(action_t *action) {
  assert (action);

  return action->edited;
}

obj action_get_original(action_t *action) {
  assert(action);

  return action->original;
}
