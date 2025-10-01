#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flowers.h"

#define DB_FILE "flowers.txt"

int get_next_id() {
  FILE *file = fopen(DB_FILE, "r");
  if (!file) return 1;
  int max_id = 0, id;
  char line[256];
  while(fgets(line, sizeof(line), file)) {
      sscanf(line, "%d,", &id); // sscanf gives largest id found
      if (id > max_id) max_id = id;
  }
  fclose(file);
  return max_id+1;
}

void add_flower() {
  char name[50];
  float price;
  int quantity;

  printf("Enter flower name: ");
  scanf("%49s", name); // 50 characters max, not to buffer oveflow
  printf("Enter price: ");
  scanf("%f", &price);
  printf("Enter quantity: ");
  scanf("%d", &quantity);

  int id = get_next_id();
  
  FILE *file = fopen(DB_FILE, "a");
  if(!file) return perror("could not open DB file");

  fprintf(file, "%d,%s,%.2f,%d,0\n", id, name, price, quantity);
  fclose(file);

  printf("flowers added successfully!\n");
}

void list_flowers() {
  FILE *file = fopen(DB_FILE, "r");
  if(!file) {
    printf("No flowrs found.\n");
    return;
  }

  char line[256];

  printf("\n---Flower List---\n");
  while (fgets(line, sizeof(line), file)) {
    int id, quantity, sold;
    char name[50];
    float price;
    sscanf(line, "%d,%49[^,],%f,%d,%d", &id, name, &price, &quantity, &sold);
    printf("Id:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", id, name, price, quantity, sold);
  }
  fclose(file);
}
