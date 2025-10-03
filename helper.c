#include "shop.h"

int get_next_id(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file)
    return 1;

  int max_id = 0;
  char line[256];
  int id;

  while (fgets(line, sizeof(line), file)) {
    sscanf(line, "%d,", &id);
    if (id > max_id)
      max_id = id;
  }

  fclose(file);
  return max_id + 1;
}

int get_valid_int(const char *prompt, int *value) {
  char buffer[256];
  printf("%s", prompt);
  if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    return 0;

  buffer[strcspn(buffer, "\n")] = '\0';

  if (sscanf(buffer, "%d", value) != 1) {
    printf("Invalid input. Must be a number.\n");
    return 0;
  }
  return 1;
}

int get_valid_float(const char *prompt, float *value) {
  char buffer[256];
  printf("%s", prompt);
  if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    return 0;

  buffer[strcspn(buffer, "\n")] = '\0';

  if (sscanf(buffer, "%f", value) != 1) {
    printf("Invalid input. Must be a number.\n");
    return 0;
  }
  return 1;
}