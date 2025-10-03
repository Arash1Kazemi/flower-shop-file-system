#include "shop.h"

Node *insert_bst(Node *root, void *data, int id, const char *name) {
  if (!root) {
    Node *new = malloc(sizeof(Node));
    if (!new)
      return NULL;
    new->data = data; // Store pointer to Flower
    new->id = id;
    strncpy(new->name, name, sizeof(new->name) - 1);
    new->name[sizeof(new->name) - 1] = '\0';
    new->left = new->right = NULL;
    return new;
  }
  if (id < root->id) {
    root->left = insert_bst(root->left, data, id, name);
  } else {
    root->right = insert_bst(root->right, data, id, name);
  }
  return root;
}

Node *search_bst_by_id(Node *root, int id) {
  if (!root || root->id == id)
    return root;
  if (id < root->id)
    return search_bst_by_id(root->left, id);
  return search_bst_by_id(root->right, id);
}

Node *search_bst_by_name(Node *root, const char *name) {
  if (!root)
    return NULL;
  int cmp = strcmp(name, root->name);
  if (cmp == 0)
    return root;
  if (cmp < 0)
    return search_bst_by_name(root->left, name);
  return search_bst_by_name(root->right, name);
}

void free_bst(Node *root) {
  if (!root)
    return;
  free_bst(root->left);
  free_bst(root->right);
  free(root->data); // Free the Flower struct
  free(root);
}

void insert_hash(HashNode *table[], int key, void *value) {
  int idx = key % HASH_SIZE;
  HashNode *new = malloc(sizeof(HashNode));
  if (!new)
    return;
  new->key = key;
  new->value = value; // Store pointer to Flower
  new->next = table[idx];
  table[idx] = new;
}

void *search_hash(HashNode *table[], int key) {
  int idx = key % HASH_SIZE;
  HashNode *node = table[idx];
  while (node) {
    if (node->key == key)
      return node->value; // Return Flower pointer
    node = node->next;
  }
  return NULL;
}

void free_hash_table(HashNode *table[]) {
  for (int i = 0; i < HASH_SIZE; i++) {
    HashNode *node = table[i];
    while (node) {
      HashNode *temp = node;
      node = node->next;
      free(temp->value); // Free the Flower struct
      free(temp);
    }
    table[i] = NULL;
  }
}