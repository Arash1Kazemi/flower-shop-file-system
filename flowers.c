#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shop.h"


int parse_flower(const char *line, Flower *f) { 
  // Parse a flower from file to structuer
  return sscanf(line, "%d,%49[^,],%f,%d,%d", &f->id, f->name, &f->price,&f->quantity, &f->sold) == 5;
}

void print_flower(FILE *file, const Flower *f) {
  fprintf(file, "%d,%s,%.2f,%d,%d\n",f->id, f->name, f->price, f->quantity, f->sold);
}

int get_next_id() {
  FILE *file = fopen(FLOWERS_FILE, "r");
  if (!file)
    return 1;

  int max_id = 0;
  char line[256];
  Flower f;

  while (fgets(line, sizeof(line), file)) {
    if(parse_flower(line, &f) && f.id > max_id) {
      max_id = f.id;
    }
  }

  fclose(file);
  return max_id + 1;
}



void add_flower() {
  Flower f;
  f.id = get_next_id(FLOWERS_FILE);
  f.sold = 0;

  printf("Enter flower name: ");
  scanf("%49s", f.name); // 50 characters max, not to buffer oveflow

  if (!get_valid_float("Enter price: ", &f.price))
    return;
  if (!get_valid_int("Enter quantity: ", &f.quantity))
    return;
/*printf("Enter price: ");
  scanf("%f", &f.price);
  printf("Enter quantity: ");
  scanf("%d", &f.quantity); */

  FILE *file = fopen(FLOWERS_FILE, "a");
  if (!file) {
    perror("could not open DB file");
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
    if(parse_flower(line, &f)) {
      printf("Id:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", f.id, f.name, f.price, f.quantity, f.sold);
    }
  }
  fclose(file);
}

void delete_flower() {
  int id;
  printf("Enter flower ID to delete: ");
  scanf("%d", &id);

  FILE *file = fopen(FLOWERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    return;
  }

  Flower f;
  char line[250];
  int found = 0;

  while (fgets(line, sizeof(line), file)) {
    if(parse_flower(line, &f)) {
      if(f.id == id) {
        found = 1;
        continue;
      }
      print_flower(temp, &f);
    }
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
  printf("Enter the Flower ID to update: ");
  scanf("%d", &id);

  FILE *file = fopen(FLOWERS_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");
  if (!file || !temp) {
    printf("Error opening file.\n");
    return;
  }

  Flower f;
  char line[250];
  int found = 0;

  while (fgets(line, sizeof(line), file)) {
    if(parse_flower(line, &f)) {
      if(f.id == id) {
        found = 1;
        printf("Enter new Name: ");
        scanf("%49s", f.name);
        printf("Enter new Price: ");
        scanf("%f", &f.price);
        printf("Enter new Quantity: ");
        scanf("%d", &f.quantity);
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
  int choice = 0;
  int id = 0;
  char name[50];
  int found = 0;

  printf("Search by:\n1. ID\n2. Name\nChoose: ");
  if (scanf("%d", &choice) != 1) {
    while (getchar() != '\n');
    printf("Invalid input. input must be a number.\n");
    return;
  }
  getchar();

  FILE *file = fopen(FLOWERS_FILE, "r");
  if (!file) {
    printf("No flowers found.\n");
    return;
  }

  Flower f;
  char line[256];

  if (choice == 1) {
    printf("Enter Flower ID: ");
    if (scanf("%d", &id) != 1) {
      while (getchar() != '\n');
      printf("invalid input. Must be a number.\n");
      fclose(file);
      return;
    }
    getchar();

    while (fgets(line, sizeof(line), file)) {
      if (parse_flower(line, &f) && f.id == id) {
        printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", f.id, f.name, f.price, f.quantity, f.sold);
        found = 1;
        break;
      }
    }
  } else if (choice == 2) {
    printf("Enter Flower Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove trailing newline.

    while (fgets(line, sizeof(line), file)) {
      if (parse_flower(line, &f) && strcmp(f.name, name) == 0){
        printf("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", f.id, f.name, f.price, f.quantity, f.sold);
        found = 1;
      }
    }
  } else {
    printf("Invalid choice.\n");
  }

  if (!found)
    printf("Flower not found.\n");
  fclose(file);
}