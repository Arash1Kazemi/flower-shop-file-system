#ifndef FLOWERS_H
#define FLOWERS_H

typedef struct {
  int id;
  char name[50];
  float price;
  int quantity;
  int sold;
}  Flower;

void list_flowers();
void add_flower();
void delete_flower();
void update_flower();
void search_flower();

// Helper funcs
int parse_flower(const char *line, Flower *f);
void print_flower(FILE * file, const Flower *f);

#endif