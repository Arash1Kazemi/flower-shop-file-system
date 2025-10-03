#include "shop.h"

int parse_customer(const char *line, Customer *c) {
  return sscanf(line, "%d,%49[^,],%99[^,],%19[^\n]", &c->id, c->name, c->address, c->phone) == 4;
}

void print_customer(FILE *file, const Customer *c) {
  fprintf(file, "%d,%s,%s,%s\n", c->id, c->name, c->address, c->phone);
}

