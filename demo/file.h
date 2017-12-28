/**
 * @file
 * @brief File handling
 */

#ifndef __FILE_h__
#define __FILE_h__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef bool(*read_file_handler)(FILE *file, void *data);

/**
 * @brief Opens a file for read and calls the handler function with the
 *        newly opened file stream.
 *
 * @param  path    The path to the file to open.
 * @param  handler The handler function that will receive the file stream.
 * @param  data    Some data to pass along to the handler function.
 *
 * @return void
 */
bool read_file(char *path, read_file_handler handler, void *data);
/**
 * @brief Opens a file for write and calls the handler function with the
 *        newly opened file stream.
 *
 * @param  path    The path to the file to open.
 * @param  handler The handler function that will receive the file stream.
 * @param  data    Some data to pass along to the handler function.
 *
 * @return void
 */
bool save_file(char *path, read_file_handler handler, void *data);

#endif
