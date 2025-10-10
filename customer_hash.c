#include "shop.h"

HashNode *customer_hash_table[HASH_SIZE] = {0}; // Define here

void cleanup_customer_hash_table() {
  free_hash_table(customer_hash_table); // Free the hash table
}