#include "shop.h"

int main(void) {
  int choice;
  while (1) {
    printf("\n--- Flower Shop Menu (Flowers Only) ---\n");
    printf("1 - List Flowers\n");
    printf("2 - Add Flower\n");
    printf("3 - Search Flower\n");
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
    default: printf("Invalid choice.\n");
    }
  }
  return 0;
}