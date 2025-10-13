#ifndef SHOP_H
#define SHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLOWERS_FILE "flowers.txt"
#define POTS_FILE "pots.txt"
#define CUSTOMERS_FILE "customers.txt"
#define SELLERS_FILE "sellers.txt"
#define PAYMENTS_FILE "payments.txt"
#define PAYMENT_ITEMS_FILE "payment_items.txt"
#define HASH_SIZE 100000

#if defined(_WIN32) || defined(_WIN64)
#define stricmp _stricmp // windows
#else
#include <strings.h> // linux
#define stricmp strcasecmp
#endif

typedef struct {
  int id;
  char name[50];
  float price;
  int quantity;
  int sold;
} Flower;

typedef struct {
  int id;
  char name[50];
  float price;
  int quantity;
  int sold;
} Pot;

typedef struct {
  int id;
  char name[50];
  char address[100];
  char phone[20];
} Customer;

typedef struct {
  int id;
  char name[50];
} Seller;

typedef struct {
  int id;
  int customer_id;
  int seller_id;
  char date[11]; // YYYY-MM-DD
  float total_amount;
} Payment;

typedef struct {
  int payment_id;
  char item_type; // 'F' for Flower, 'P' for Pot
  int item_id;
  int quantity;
} PaymentItem;

// AVL tree for log(n) search
typedef struct Node {
  void *data;    // Generic to hold Flower, Pot, etc.
  int id;        // For comparison
  char name[50]; // For name-based search
  int height;    // For AVL balancing
  struct Node *left;
  struct Node *right;
} Node;

typedef struct HashNode {
  int key;
  void *value; // Generic
  struct HashNode *next;
} HashNode;

// Declare global hash table for customers
extern HashNode *customer_hash_table[HASH_SIZE];

// Helper prototypes
int get_next_id(const char *filename);
int get_valid_int(const char *prompt, int *value);
int get_valid_float(const char *prompt, float *value);

// Flower prototypes
void add_flower();
void list_flowers();
void delete_flower();
void update_flower();
void search_flower();
int parse_flower(const char *line, Flower *f);
void print_flower(FILE *file, const Flower *f);
int load_flower_by_id(int id, Flower *f);
void update_flower_stock(const Flower *updated);

// Pot prototypes
void add_pot();
void list_pots();
void delete_pot();
void update_pot();
void search_pot();
int parse_pot(const char *line, Pot *p);
void print_pot(FILE *file, const Pot *p);
int load_pot_by_id(int id, Pot *p);
void update_pot_stock(const Pot *updated);

// Customer prototypes
void add_customer();
void list_customers();
void delete_customer();
void update_customer();
void search_customer();

int parse_customer(const char *line, Customer *c);
void print_customer(FILE *file, const Customer *c);
void customer_menu(Customer *c);
void get_current_date(char *buffer, int size);
void buy_flower(Customer *c);
void buy_pot(Customer *c);
Customer *load_customer_by_id(int id);
Customer *customer_login();
Customer *customer_register();
Customer *customer_portal_login_register();
void view_receipts(Customer *c);

// Payment prototypes
int parse_payment(const char *line, Payment *p);
void print_payment(FILE *file, const Payment *p);
int parse_payment_item(const char *line, PaymentItem *pi);
void print_payment_item(FILE *file, const PaymentItem *pi);

// Hash table prototypes
void init_customer_hash_table();
void cleanup_customer_hash_table(); // Added for cleanup
void insert_hash(HashNode *table[], int key, void *value);
void *search_hash(HashNode *table[], int key);
void free_hash_table(HashNode *table[]);
void delete_hash(HashNode *table[], int key);
void save_customers_from_hash(); // Added for persisting hash to file

// AVL tree prototypes
Node *insert_avl(Node *root, void *data, int id, const char *name);
Node *search_avl_by_id(Node *root, int id);
Node *search_avl_by_name(Node *root, const char *name);
void free_avl(Node *root);

#endif
