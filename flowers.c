#include "shop.h"

void add_flower() {
  Flower f;
  f.id = get_next_id(FLOWERS_FILE);
  f.sold = 0;

  printf("Enter flower name: ");
  char name[50];
  if (fgets(name, sizeof(name), stdin) == NULL) {
    printf("Error reading input.\n");
    return;
  }
  name[strcspn(name, "\n")] = '\0';
  strncpy(f.name, name, sizeof(f.name) - 1);
  f.name[sizeof(f.name) - 1] = '\0';

  if (!get_valid_float("Enter price: ", &f.price))
    return;
  if (!get_valid_int("Enter quantity: ", &f.quantity))
    return;

  FILE *file = fopen(FLOWERS_FILE, "a");
  if (!file) {
    perror("Could not open flowers file");
    return;
  }
  print_flower(file, &f);
  fclose(file);
  printf("Flower added successfully!\n");
}

void list_flowers() {
  FILE *file = fopen(FLOWERS_FILE, "r");
  if (!file) {
    printf("No flowers found.\n");
    return;
  }

  Flower f;
  char line[256];
  printf("\n--- Flowers List ---\n");
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, &f)) {
      printf("ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", f.id, f.name, f.price, f.quantity, f.sold);
    }
  }
  fclose(file);
}

void delete_flower() {
  int id;
  if (!get_valid_int("Enter flower ID to delete: ", &id))
    return;

  FILE *file = fopen(FLOWERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Flower f;
  char line[256];
  int found = 0;
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, &f) && f.id == id) {
      found = 1;
      continue;
    }
    print_flower(temp, &f);
  }

  fclose(file);
  fclose(temp);

  if (found) {
    remove(FLOWERS_FILE);
    rename("temp.txt", FLOWERS_FILE);
    printf("Flower deleted successfully.\n");
  } else {
    remove("temp.txt");
    printf("Flower ID not found.\n");
  }
}

void update_flower() {
  int id;
  if (!get_valid_int("Enter the Flower ID to update: ", &id))
    return;

  FILE *file = fopen(FLOWERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Flower f;
  char line[256];
  int found = 0;
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, &f)) {
      if (f.id == id) {
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
        strncpy(f.name, name, sizeof(f.name) - 1);
        f.name[sizeof(f.name) - 1] = '\0';

        if (!get_valid_float("Enter new Price: ", &f.price)) {
          fclose(file);
          fclose(temp);
          remove("temp.txt");
          return;
        }
        if (!get_valid_int("Enter new Quantity: ", &f.quantity)) {
          fclose(file);
          fclose(temp);
          remove("temp.txt");
          return;
        }
      }
      print_flower(temp, &f);
    }
  }

  fclose(file);
  fclose(temp);

  if (found) {
    remove(FLOWERS_FILE);
    rename("temp.txt", FLOWERS_FILE);
    printf("Flower updated successfully.\n");
  } else {
    remove("temp.txt");
    printf("Flower ID not found.\n");
  }
}

void search_flower() {
  int choice;
  if (!get_valid_int("Search by:\n1. ID\n2. Name\nChoose: ", &choice))
    return;
  if (choice != 1 && choice != 2) {
    printf("Invalid choice. Please enter 1 or 2.\n");
    return;
  }

  FILE *file = fopen(FLOWERS_FILE, "r");
  if (!file) {
    printf("No flowers found.\n");
    return;
  }

  Node *bst_root = NULL;
  HashNode *hash_table[HASH_SIZE] = {0};
  Flower f;
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, &f)) {
      Flower *f_copy = malloc(sizeof(Flower));
      if (!f_copy)
        continue;
      *f_copy = f;
      bst_root = insert_bst(bst_root, f_copy, f.id, f.name);
      insert_hash(hash_table, f.id, f_copy);
    }
  }
  fclose(file);

  int found = 0;
  if (choice == 1) {
    int id;
    if (!get_valid_int("Enter Flower ID: ", &id)) {
      free_bst(bst_root);
      free_hash_table(hash_table);
      return;
    }

    Flower *result = search_hash(hash_table, id);
    if (result) {
      printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", result->id, result->name, result->price,
             result->quantity, result->sold);
      found = 1;
    }
  } else {
    char name[50];
    printf("Enter Flower Name: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
      printf("Error reading input.\n");
      free_bst(bst_root);
      free_hash_table(hash_table);
      return;
    }
    name[strcspn(name, "\n")] = '\0';

    Node *result = search_bst_by_name(bst_root, name);
    if (result) {
      Flower *f = (Flower *)result->data;
      printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", f->id, f->name, f->price, f->quantity, f->sold);
      found = 1;
    }
  }

  if (!found) {
    printf("Flower not found.\n");
  }

  free_bst(bst_root);
  free_hash_table(hash_table);
}

int parse_flower(const char *line, Flower *f) {
  return sscanf(line, "%d,%49[^,],%f,%d,%d", &f->id, f->name, &f->price, &f->quantity, &f->sold) == 5;
}

void print_flower(FILE *file, const Flower *f) {
  fprintf(file, "%d,%s,%.2f,%d,%d\n", f->id, f->name, f->price, f->quantity, f->sold);
}

int load_flower_by_id(int id, Flower *f) {
  FILE *file = fopen(FLOWERS_FILE, "r");
  if (!file)
    return 0;

  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, f) && f->id == id) {
      fclose(file);
      return 1;
    }
  }
  fclose(file);
  return 0;
}

void update_flower_stock(const Flower *updated) {
  FILE *file = fopen(FLOWERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    if (file)
      fclose(file);
    if (temp)
      fclose(temp);
    return;
  }

  Flower f;
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    if (parse_flower(line, &f)) {
      if (f.id == updated->id) {
        f = *updated;
      }
      print_flower(temp, &f);
    }
  }

  fclose(file);
  fclose(temp);
  remove(FLOWERS_FILE);
  rename("temp.txt", FLOWERS_FILE);
}