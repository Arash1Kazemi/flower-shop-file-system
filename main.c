#include "shop.h"

int main(void) {
  init_customer_hash_table(); // Initialize hash table
  int main_choice, sub_choice;

  while (1) {
    printf("\n=== Main Menu ===\n");
    printf("1 - Flowers\n");
    printf("2 - Pots\n");
    printf("3 - Customers\n");
    printf("4 - Exit\n");
    printf("5 - Customer Management\n");

    if (!get_valid_int("Your choice: ", &main_choice))
      continue;

    switch (main_choice) {

    case 1: // flowers
      while (1) {
        printf("\n--- flower menu ---\n");
        printf("1 - list flowers\n");
        printf("2 - add flower\n");
        printf("3 - search flower\n");
        printf("4 - update flower\n");
        printf("5 - delete flower\n");
        printf("6 - back\n");

        if (!get_valid_int("your choice: ", &sub_choice))
          continue;

        if (sub_choice == 6)
          break;

        switch (sub_choice) {
        case 1: list_flowers(); break;
        case 2: add_flower(); break;
        case 3: search_flower(); break;
        case 4: update_flower(); break;
        case 5: delete_flower(); break;
        default: printf("invalid choice.\n");
        }
      }
      break;

    case 2: // pots
      while (1) {
        printf("\n--- pot menu ---\n");
        printf("1 - list pots\n");
        printf("2 - add pot\n");
        printf("3 - search pot\n");
        printf("4 - update pot\n");
        printf("5 - delete pot\n");
        printf("6 - back\n");

        if (!get_valid_int("your choice: ", &sub_choice))
          continue;

        if (sub_choice == 6)
          break;

        switch (sub_choice) {
        case 1: list_pots(); break;
        case 2: add_pot(); break;
        case 3: search_pot(); break;
        case 4: update_pot(); break;
        case 5: delete_pot(); break;
        default: printf("invalid choice.\n");
        }
      }
      break;

    case 3: { // customer portal
      Customer *c = customer_portal_login_register();
      if (!c)
        break;

      while (1) {
        printf("\n--- Customer Menu ---\n");
        printf("1 - Buy Flower\n");
        printf("2 - Buy Pot\n");
        printf("3 - View Receipts\n");
        printf("4 - Logout\n");

        if (!get_valid_int("Your choice: ", &sub_choice))
          continue;
        if (sub_choice == 4)
          break;

        switch (sub_choice) {
        case 1: buy_flower(c); break;
        case 2: buy_pot(c); break;
        case 3: view_receipts(c); break;
        default: printf("Invalid choice.\n");
        }
      }
      break;
    }

    case 4:
      cleanup_customer_hash_table(); // Cleanup hash table before exit
      exit(0);

    case 5: // Customer Management submenu using hash table
      while (1) {
        printf("\n--- Customer Management ---\n");
        printf("1 - List Customers\n");
        printf("2 - Add Customer\n");
        printf("3 - Search Customer (by ID)\n");
        printf("4 - Update Customer\n");
        printf("5 - Delete Customer\n");
        printf("6 - Back\n");

        if (!get_valid_int("Your choice: ", &sub_choice))
          continue;

        if (sub_choice == 6)
          break;

        switch (sub_choice) {
        case 1: list_customers(); break;
        case 2: add_customer(); break;
        case 3: search_customer(); break;
        case 4: update_customer(); break;
        case 5: delete_customer(); break;
        default: printf("Invalid choice.\n");
        }
      }
      break;

    default: printf("Invalid choice.\n");
    }
  }
  return 0;
}