#include "shop.h"

void add_pot() {
  Pot p;
  p.id = get_next_id(POTS_FILE);
  p.sold = 0;

  printf("Enter pot name: ");
  char name[50];
  if (fgets(name, sizeof(name), stdin) == NULL) {
    printf("Error reading input.\n");
    return;
  }
  name[strcspn(name, "\n")] = '\0';
  strncpy(p.name, name, sizeof(p.name) - 1);
  p.name[sizeof(p.name) - 1] = '\0';

  if (!get_valid_float("Enter price: ", &p.price))
    return;
  if (!get_valid_int("Enter quantity: ", &p.quantity))
    return;

  FILE *file = fopen(POTS_FILE, "a");
  if (!file) {
    perror("Could not open pots file");
    return;
  }
  print_pot(file, &p);
  fclose(file);
  printf("Pot added successfully!\n");
}

void list_pots() {
  FILE *file = fopen(POTS_FILE, "r");
  if (!file) {
    printf("No pots found.\n");
    return;
  }

  Pot p;
  char line[256];
  printf("\n--- Pots List ---\n");
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, &p)) {
      printf("ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", p.id, p.name, p.price, p.quantity, p.sold);
    }
  }
  fclose(file);
}

void delete_pot() {
  int id;
  if (!get_valid_int("Enter pot ID to delete: ", &id))
    return;

  FILE *file = fopen(POTS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Pot p;
  char line[256];
  int found = 0;
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, &p) && p.id == id) {
      found = 1;
      continue;
    }
    print_pot(temp, &p);
  }

  fclose(file);
  fclose(temp);

  if (found) {
    remove(POTS_FILE);
    rename("temp.txt", POTS_FILE);
    printf("Pot deleted successfully.\n");
  } else {
    remove("temp.txt");
    printf("Pot ID not found.\n");
  }
}

void update_pot() {
  int id;
  if (!get_valid_int("Enter the Pot ID to update: ", &id))
    return;

  FILE *file = fopen(POTS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Pot p;
  char line[256];
  int found = 0;
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, &p)) {
      if (p.id == id) {
        found = 1;
        printf("Enter new Name: ");
        char name[50];
        if (fgets(name, sizeof(name), stdin) == NULL) {
          printf("Error reading input.\n");
          fclose(file);
          fclose(temp);
          remove("temp.txt");
          return;
        }
        name[strcspn(name, "\n")] = '\0';
        strncpy(p.name, name, sizeof(p.name) - 1);
        p.name[sizeof(p.name) - 1] = '\0';

        if (!get_valid_float("Enter new Price: ", &p.price)) {
          fclose(file);
          fclose(temp);
          remove("temp.txt");
          return;
        }
        if (!get_valid_int("Enter new Quantity: ", &p.quantity)) {
          fclose(file);
          fclose(temp);
          remove("temp.txt");
          return;
        }
      }
      print_pot(temp, &p);
    }
  }

  fclose(file);
  fclose(temp);

  if (found) {
    remove(POTS_FILE);
    rename("temp.txt", POTS_FILE);
    printf("Pot updated successfully.\n");
  } else {
    remove("temp.txt");
    printf("Pot ID not found.\n");
  }
}

void search_pot() {
  int choice;
  if (!get_valid_int("Search by:\n1. ID\n2. Name\nChoose: ", &choice))
    return;
  if (choice != 1 && choice != 2) {
    printf("Invalid choice. Please enter 1 or 2.\n");
    return;
  }

  FILE *file = fopen(POTS_FILE, "r");
  if (!file) {
    printf("No pots found.\n");
    return;
  }

  Node *avl_root = NULL;
  Pot p;
  char line[256];
  int count = 0;
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, &p)) {
      Pot *p_copy = malloc(sizeof(Pot));
      if (!p_copy) {
        printf("Memory allocation failed for pot ID %d.\n", p.id);
        continue;
      }
      *p_copy = p;
      avl_root = insert_avl(avl_root, p_copy, p.id, p.name);
      if (!avl_root) {
        printf("Failed to insert pot ID %d into AVL tree.\n", p.id);
        free(p_copy);
      } else {
        count++;
      }
    }
  }
  fclose(file);
  printf("Loaded %d pots into AVL tree.\n", count);

  int found = 0;
  if (choice == 1) {
    int id;
    if (!get_valid_int("Enter Pot ID: ", &id)) {
      free_avl(avl_root);
      return;
    }

    Node *result = search_avl_by_id(avl_root, id);
    if (result) {
      Pot *p = (Pot *)result->data;
      printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", p->id, p->name, p->price, p->quantity, p->sold);
      found = 1;
    }
  } else {
    char name[50];
    printf("Enter Pot Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
      printf("Error reading input.\n");
      free_avl(avl_root);
      return;
    }
    name[strcspn(name, "\n")] = '\0';
    printf("Searching for name: '%s'\n", name);

    void inorder_search(Node * node, const char *search_name) {
      if (!node)
        return;
      inorder_search(node->left, search_name);
      if (stricmp(search_name, node->name) == 0) {
        Pot *p = (Pot *)node->data;
        printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", p->id, p->name, p->price, p->quantity, p->sold);
        found = 1;
      }
      inorder_search(node->right, search_name);
    }
    inorder_search(avl_root, name);
  }

  if (!found) {
    printf("Pot not found.\n");
  }

  free_avl(avl_root);
}

int parse_pot(const char *line, Pot *p) {
  return sscanf(line, "%d,%49[^,],%f,%d,%d", &p->id, p->name, &p->price, &p->quantity, &p->sold) == 5;
}

void print_pot(FILE *file, const Pot *p) {
  fprintf(file, "%d,%s,%.2f,%d,%d\n", p->id, p->name, p->price, p->quantity, p->sold);
}

int load_pot_by_id(int id, Pot *p) {
  FILE *file = fopen(POTS_FILE, "r");
  if (!file)
    return 0;

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, p) && p->id == id) {
      fclose(file);
      return 1;
    }
  }
  fclose(file);
  return 0;
}

void update_pot_stock(const Pot *updated) {
  FILE *file = fopen(POTS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Pot p;
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (parse_pot(line, &p)) {
      if (p.id == updated->id) {
        p = *updated;
      }
      print_pot(temp, &p);
    }
  }

  fclose(file);
  fclose(temp);
  remove(POTS_FILE);
  rename("temp.txt", POTS_FILE);
}
