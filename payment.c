#include "shop.h"
#include <stdio.h>

void print_payment(FILE *file, const Payment *p) {
  fprintf(file, "%d,%d,%d,%s,%.2f\n", p->id, p->customer_id, p->seller_id, p->date, p->total_amount);
}

void print_payment_item(FILE *file, const PaymentItem *pi) {
  fprintf(file, "%d,%c,%d,%d\n", pi->payment_id, pi->item_type, pi->item_id, pi->quantity);
}

void view_receipts(Customer *c) {
  FILE *fp = fopen(PAYMENTS_FILE, "r");
  if (!fp) {
    printf("No payments found.\n");
    return;
  }

  Payment p;
  char line[256];
  printf("\n--- Receipts for %s ---\n", c->name);
  while (fgets(line, sizeof(line), fp)) {
    if (parse_payment(line, &p) && p.customer_id == c->id) {
      printf("Payment ID: %d, Date: %s, Total: %.2f\n", p.id, p.date, p.total_amount);
    }
  }
  fclose(fp);
}

int parse_payment(const char *line, Payment *p) {
  return sscanf(line, "%d,%d,%d,%10[^,],%f", &p->id, &p->customer_id, &p->seller_id, p->date, &p->total_amount) == 5;
}
