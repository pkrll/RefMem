/**
 * @file
 * @brief Common
 */

#ifndef __common_h__
#define __common_h__

#include <stdbool.h>
#include "../src/refmem.h"

typedef obj(*element_copy_fun)(obj);
typedef void(*element_free_fun)(obj);
typedef int(*element_comp_fun)(obj, obj);
typedef bool(*elem_apply_fun)(obj elem, void *data);

#endif
