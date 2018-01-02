#include "file.h"
#include <errno.h>


// -------------------------------
// Declarations
// -------------------------------

static FILE *open_file(char *path, char *mode);

// -------------------------------
// Public
// -------------------------------

bool read_file(char *path, read_file_handler handler, void *data) {
  FILE *file = open_file(path, "r");

  if (file) {
    handler(file, data);
    fclose(file);
  } else {
    // int errnum = errno;
    // fprintf(stderr, "Value of errno: %d\n", errno);
    // perror("Error printed by perror");
    // fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
    // exit(EXIT_FAILURE);
  }

  return true;
}

bool save_file(char *path, read_file_handler handler, void *data) {
  FILE *file = open_file(path, "w");

  if (file) {
    handler(file, data);
    fclose(file);
  }

  return true;
}

// -------------------------------
// Private
// -------------------------------


static FILE *open_file(char *path, char *mode) {
  if (path && mode) {
    FILE *file = fopen(path, mode);

    return file;
  }

  return NULL;
}
