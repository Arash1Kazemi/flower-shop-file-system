#ifndef SHOP_H
#define SHOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLOWERS_FILE "Mackfile/flowers.txt"
#define POTS_FILE "Mackfile/pots.txt"
#define CUSTOMERS_FILE "Mackfile/customers.txt"
#define SELLERS_FILE "Mackfile/sellers.txt"
#define PAYMENTS_FILE "Mackfile/payments.txt"
#define PAYMENT_ITEMS_FILE "Mackfile/payment_items.txt"
#define HASH_SIZE 100

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

// BST for log(n) search
typedef struct Node {
  void *data;    // Generic to hold Flower, Pot, etc.
  int id;        // For comparison
  char name[50]; // For name-based BST
  struct Node *left;
  struct Node *right;
} Node;

// Hash table for O(1) lookup
typedef struct HashNode {
  int key;
  void *value; // Generic
  struct HashNode *next;
} HashNode;

// Helper prototypes
int get_next_id(const char *filename);
int get_valid_int(const char *prompt, int *value);
int get_valid_float(const char *prompt, float *value);

// Flower prototypes
void add_flower();
void list_flowers();
void delete_flower();
void update_flower();
void search_flower();  //*
int parse_flower(const char *line, Flower *f);
void print_flower(FILE *file, const Flower *f);
int load_flower_by_id(int id, Flower *f);
void update_flower_stock(const Flower *updated);

// Pot prototypes

// Customer prototypes

// Payment prototypes

// Report prototypes
void show_reports_menu();
void report_daily_sales();
void report_monthly_sales();
void report_best_selling();
void report_vip_customers();
void report_current_stock();

// Optimization prototypes
Node *insert_bst(Node *root, void *data, int id, const char *name);
Node *search_bst_by_id(Node *root, int id);
Node *search_bst_by_name(Node *root, const char *name);
void free_bst(Node *root);
void insert_hash(HashNode *table[], int key, void *value);
void *search_hash(HashNode *table[], int key);
void free_hash_table(HashNode *table[]);

#endif