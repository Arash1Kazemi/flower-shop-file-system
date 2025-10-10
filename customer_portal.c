#include "shop.h"

Customer *load_customer_by_id(int id) {
  // Use hash table for O(1) lookup
  return (Customer *)search_hash(customer_hash_table, id);
}

Customer *customer_login() {
  int id;
  if (!get_valid_int("Enter your Customer ID: ", &id))
    return NULL;

  Customer *c = load_customer_by_id(id);
  if (!c) {
    printf("Customer not found. Please register first.\n");
    return NULL;
  }

  printf("Welcome back, %s!\n", c->name);
  return c;
}

Customer *customer_register() {
  Customer *c = malloc(sizeof(Customer));
  if (!c) {
    printf("Memory allocation failed for customer.\n");
    return NULL;
  }
  c->id = get_next_id(CUSTOMERS_FILE);

  printf("Register New Customer\n");

  printf("Enter Name: ");
  fgets(c->name, sizeof(c->name), stdin);
  c->name[strcspn(c->name, "\n")] = '\0';

  printf("Enter Address: ");
  fgets(c->address, sizeof(c->address), stdin);
  c->address[strcspn(c->address, "\n")] = '\0';

  printf("Enter Phone: ");
  fgets(c->phone, sizeof(c->phone), stdin);
  c->phone[strcspn(c->phone, "\n")] = '\0';

  FILE *file = fopen(CUSTOMERS_FILE, "a");
  if (!file) {
    printf("Error opening customer file.\n");
    free(c);
    return NULL;
  }

  print_customer(file, c);
  fclose(file);

  insert_hash(customer_hash_table, c->id, c); // Add to hash table

  printf("Registration complete! Your Customer ID is %d.\n", c->id);
  return c;
}

Customer *customer_portal_login_register() {
  int choice;
  while (1) {
    printf("\n--- Customer Portal ---\n");
    printf("1 - Login\n");
    printf("2 - Register\n");
    printf("3 - Back\n");

    if (!get_valid_int("Your choice: ", &choice))
      continue;

    switch (choice) {
    case 1: {
      Customer *c = customer_login();
      if (c)
        return c;
      break;
    }
    case 2: {
      Customer *c = customer_register();
      if (c)
        return c;
      break;
    }
    case 3: return NULL;
    default: printf("Invalid choice.\n");
    }
  }
}

void delete_customer(int id) {
  delete_hash(customer_hash_table, id);
  FILE *file = fopen(CUSTOMERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error accessing files.\n");
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  char line[256];
  Customer c;
  while (fgets(line, sizeof(line), file)) {
    if (parse_customer(line, &c) && c.id != id) {
      print_customer(temp, &c);
    }
  }

  fclose(file);
  fclose(temp);
  remove(CUSTOMERS_FILE);
  rename("temp.txt", CUSTOMERS_FILE);

  printf("Customer ID %d deleted.\n", id);
}