#include "flowers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  int choice;

  while (1) {
    printf("\nEnter Numbers below to continue:\n");
    printf("1 - Existing Flowers\n");
    printf("2 - Add Flower\n");
    printf("3 - Search Flowers\n");
    printf("4 - Update Flower\n");
    printf("5 - Delete Flower\n");
    printf("6 - Exit\n");
    if (!get_valid_int("Your choice: ", &choice))
      continue;

    switch (choice) {
    case 1: list_flowers(); break;
    case 2: add_flower(); break;
    case 3: search_flower(); break;
    case 4: update_flower(); break;
    case 5: delete_flower(); break;
    case 6: exit(0);
    default: printf("Invalid input. Try again.\n");
    }
  }
  return 0;
}