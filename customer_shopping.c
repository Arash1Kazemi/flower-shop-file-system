#include "shop.h"
#include <time.h>

void get_current_date(char *buffer, int size) {
  time_t t = time(NULL);
  strftime(buffer, size, "%y-%m-%d", localtime(&t));
}

void buy_flower(Customer *c) {
  int flower_id, quantity;
  list_flowers();

  if (!get_valid_int("Enter Flower ID to buy: ", &flower_id))
    return;
  if (!get_valid_int("Enter Quantity: ", &quantity))
    return;

  Flower f;
  if (!load_flower_by_id(flower_id, &f)) {
    printf("Flower not found.\n");
    return;
  }

  if (quantity <= 0 || quantity > f.quantity) {
    printf("Invalid quantity. Only %d available.\n", f.quantity);
    return;
  }

  f.quantity -= quantity;
  f.sold += quantity;
  update_flower_stock(&f);

  Payment payment;
  payment.id = flower_id;
  payment.customer_id = c->id;
  payment.seller_id = 1;
  get_current_date(payment.date, sizeof(payment.date));
  payment.total_amount = f.price * quantity;

  FILE *fp = fopen(PAYMENTS_FILE, "a");
  if (fp) {
    print_payment(fp, &payment);
    fclose(fp);
  }

  PaymentItem pi;
  pi.payment_id = flower_id;
  pi.item_type = 'F';
  pi.item_id = f.id;
  pi.quantity = quantity;

  fp = fopen(PAYMENT_ITEMS_FILE, "a");
  if (fp) {
    print_payment_item(fp, &pi);
    fclose(fp);
  }

  printf("\n--- Receipt ---\n");
  printf("Payment ID: %d\nCustomer: %s\nDate: %s\n", payment.id, c->name, payment.date);
  printf("--------------------\n");
  printf("%dx %s @ $%.2f each\n", quantity, f.name, f.price);
  printf("--------------------\n");
  printf("Total: $%.2f\n", payment.total_amount);
  printf("--------------------\n");
}

void buy_pot(Customer *c) {
  int pot_id, quantity;
  list_pots();

  if (!get_valid_int("Enter Pot ID to buy: ", &pot_id))
    return;
  if (!get_valid_int("Enter Quantity: ", &quantity))
    return;

  Pot p;
  if (!load_pot_by_id(pot_id, &p)) {
    printf("Pot not found.\n");
    return;
  }

  if (quantity <= 0 || quantity > p.quantity) {
    printf("Invalid quantity. Only %d available.\n", p.quantity);
    return;
  }

  p.quantity -= quantity;
  p.sold += quantity;
  update_pot_stock(&p);

  Payment payment;
  payment.id = pot_id;
  payment.customer_id = c->id;
  payment.seller_id = 1;
  get_current_date(payment.date, sizeof(payment.date));
  payment.total_amount = p.price * quantity;

  FILE *fp = fopen(PAYMENTS_FILE, "a");
  if (fp) {
    print_payment(fp, &payment);
    fclose(fp);
  }

  PaymentItem pi;
  pi.payment_id = pot_id;
  pi.item_type = 'P';
  pi.item_id = p.id;
  pi.quantity = quantity;

  fp = fopen(PAYMENT_ITEMS_FILE, "a");
  if (fp) {
    print_payment_item(fp, &pi);
    fclose(fp);
  }

  printf("\n--- Receipt ---\n");
  printf("Payment ID: %d\nCustomer: %s\nDate: %s\n", payment.id, c->name, payment.date);
  printf("--------------------\n");
  printf("%dx %s @ $%.2f each\n", quantity, p.name, p.price);
  printf("--------------------\n");
  printf("Total: $%.2f\n", payment.total_amount);
  printf("--------------------\n");
}

