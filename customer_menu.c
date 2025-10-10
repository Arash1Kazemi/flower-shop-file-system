#include "shop.h"
#include <time.h>

void customer_menu(Customer *c) {
  int choice;

  while (1) {
    printf("\n--- Customer Menu ---\n");
    printf("Welcome, %s!\n", c->name);
    printf("1 - List Flowers\n");
    printf("2 - List Pots\n");
    printf("3 - Buy Flower\n");
    printf("4 - Buy Pot\n");
    printf("5 - View Receipts / Purchase History\n");
    printf("6 - Logout\n");

    if (!get_valid_int("Your choice: ", &choice))
      continue;

    switch (choice) {
    case 1: list_flowers(); break;

    case 2: list_pots(); break;

    case 3: buy_flower(c); break;

    case 4: buy_pot(c); break;

    case 5: view_receipts(c); break;

    case 6: printf("logging out...\n"); return;

    default: printf("Invalid choice.\n");
    }
  }
}
