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
void add_customer() {
  // Similar to customer_register, but void for admin use
  Customer *c = malloc(sizeof(Customer));
  if (!c) {
    printf("Memory allocation failed for customer.\n");
    return;
  }
  c->id = get_next_id(CUSTOMERS_FILE);

  printf("Add New Customer\n");

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
    return;
  }

  print_customer(file, c);
  fclose(file);

  insert_hash(customer_hash_table, c->id, c); // O(1) insert into hash

  printf("Customer added successfully! ID: %d\n", c->id);
}

void list_customers() {
  printf("\n--- Customers List ---\n");
  for (int i = 0; i < HASH_SIZE; i++) {
    HashNode *node = customer_hash_table[i];
    while (node) {
      Customer *c = (Customer *)node->value;
      printf("ID:%d Name:%s Address:%s Phone:%s\n", c->id, c->name, c->address, c->phone);
      node = node->next;
    }
  }
}

void search_customer() {
  int id;
  if (!get_valid_int("Enter Customer ID to search: ", &id))
    return;

  Customer *c = load_customer_by_id(id); // O(1) hash lookup
  if (!c) {
    printf("Customer not found.\n");
    return;
  }

  printf("Found: ID:%d Name:%s Address:%s Phone:%s\n", c->id, c->name, c->address, c->phone);
}

void update_customer() {
  int id;
  if (!get_valid_int("Enter Customer ID to update: ", &id))
    return;

  Customer *c = load_customer_by_id(id); // O(1) hash lookup
  if (!c) {
    printf("Customer not found.\n");
    return;
  }

  char buffer[100];
  printf("Current Name: %s\nEnter new Name (enter to keep): ", c->name);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  if (strlen(buffer) > 0)
    strncpy(c->name, buffer, sizeof(c->name) - 1);

  printf("Current Address: %s\nEnter new Address (enter to keep): ", c->address);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  if (strlen(buffer) > 0)
    strncpy(c->address, buffer, sizeof(c->address) - 1);

  printf("Current Phone: %s\nEnter new Phone (enter to keep): ", c->phone);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strcspn(buffer, "\n")] = '\0';
  if (strlen(buffer) > 0)
    strncpy(c->phone, buffer, sizeof(c->phone) - 1);

  save_customers_from_hash(); // Persist changes to file
  printf("Customer updated successfully.\n");
}

void delete_customer() {
  int id;
  if (!get_valid_int("Enter Customer ID to delete: ", &id))
    return;

  if (!search_hash(customer_hash_table, id)) {
    printf("Customer not found.\n");
    return;
  }

  delete_hash(customer_hash_table, id); // O(1) delete from hash
  save_customers_from_hash();           // Persist to file
  printf("Customer ID %d deleted.\n", id);
}