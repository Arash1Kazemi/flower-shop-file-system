#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flowers.h"

#define DB_FILE "flowers.txt"

int get_next_id()
{
  FILE *file = fopen(DB_FILE, "r");
  if (!file)
    return 1;
  int max_id = 0, id;
  char line[256];
  while (fgets(line, sizeof(line), file))
  {
    sscanf(line, "%d,", &id); // sscanf gives largest id found
    if (id > max_id)
      max_id = id;
  }
  fclose(file);
  return max_id + 1;
}

void add_flower()
{
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
  if (!file)
    return perror("could not open DB file");

  fprintf(file, "%d,%s,%.2f,%d,0\n", id, name, price, quantity);
  fclose(file);

  printf("flowers added successfully!\n");
}

void list_flowers()
{
  FILE *file = fopen(DB_FILE, "r");
  if (!file)
  {
    printf("No flowrs found.\n");
    return;
  }

  char line[256];

  printf("\n---Flower List---\n");
  while (fgets(line, sizeof(line), file))
  {
    int id, quantity, sold;
    char name[50];
    float price;
    sscanf(line, "%d,%49[^,],%f,%d,%d", &id, name, &price, &quantity, &sold);
    printf("Id:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", id, name, price, quantity, sold);
  }
  fclose(file);
}

void delete_flower()
{
  int id;
  printf_s("Enter flower ID to delete: ");
  scanf_s("%d", id);

  FILE *file = fopen(DB_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");

  if (!file || !temp)
  {
    printf_s("Error in opening file.\n");
    return;
  }

  char line[250];
  int found = 0;

  while (fgets(line, sizeof(line), file))
  {
    int fid;
    sscanf_s(line, "%d", &fid);
    if (fid == id)
    {
      found = 1;
      continue;
    }
    fprintf_s(temp, "%s", line);
  }

  fclose(file);
  fclose(temp);

  if (found)
  {
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);
    printf_s("Flower dleated successfuly.\n");
  }
  else
  {
    remove("temp.txt");
    printf_s("Flower Id not found.\n");
  }
}

void update_flower()
{
  int id;
  printf_s("Enter the Flower ID to update");
  scanf_s("%d", id);

  FILE *file = fopen(DB_FILE, "w");
  FILE *temp = fopen("temp.txt", "w");

  if (!file)
  {
    printf_s("Error in opening file.\n");
    return;
  }

  char line[250];
  int found = 0;

  while (fgets(line, sizeof(line), file))
  {

    int fid, quantity, sold;
    char fname[50];
    float price;

    sscanf_s(line, "%d", &fid);
    if (fid == id)
    {
      found = 1;
      printf_s("Enter new Name: ");
      scanf_s("%49s", fname);
      printf_s("Enter new Price: ");
      scanf_s("%f", &price);
      printf_s("Enter new Quantity: ");
      scanf_s("%d", &quantity);
      fprintf_s(temp, "%d,%s,%.2f,%d,%d\n", fid, fname, price, quantity, sold);
    }
    else
    {
      fprintf_s(temp, "%s", line);
    }
  }
  fclose(file);
  fclose(temp);

  if (found)
  {
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);
    printf_s("Flower dleated successfuly.\n");
  }
  else
  {
    remove("temp.txt");
    printf_s("Flower ID not found.\n");
  }
}

void search_flower(){
  int id;
  char name[50];
  int found = 0;

  printf_s("Search by:\n1. ID\n2. Name\nChoose: ");
  int choice;
  scanf_s("%d", &choice);

  FILE *file = fopen(DB_FILE, "r");
  if (!file){
    printf_s("No flowers found.\n");
    return;
  }

  char line[256];
  if (choice == 1){
    printf_s("Enter Flower ID: ");
    scanf_s("%d", &id);
    while (fgets(line, sizeof(line), file)){
      int fid, quantity, sold;
      char fname[50];
      float price;
      sscanf_s(line, "%d,%49[^,],%f,%d,%d", &fid, fname, &price, &quantity, &sold);
      if (fid == id){
        printf_s("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", fid, fname, price, quantity, sold);
        found = 1;
        break;
      }
    }
  }
  else if (choice == 2){
    printf_s("Enter Flower Name: ");
    scanf_s("%49s", name);
    while (fgets(line, sizeof(line), file)){
      int fid, quantity, sold;
      char fname[50];
      float price;
      sscanf_s(line, "%d,%49[^,],%f,%d,%d", &fid, fname, &price, &quantity, &sold);
      if (strcmp(fname, name) == 0){
        printf_s("Found: ID:%d Name:%s Price:%.2f Quantity:%d Sold:%d\n", fid, fname, price, quantity, sold);
        found = 1;
      }
    }
  }
  else{
    printf_s("Invalid choice.\n");
  }

  if (!found)
    printf_s("Flower not found.\n");
    
  fclose(file);
}