#include "shop.h"

// AVL Tree Functions
static int max(int a, int b) { return (a > b) ? a : b; }

static int get_height(Node *node) { return node ? node->height : 0; }

static int get_balance(Node *node) { return node ? get_height(node->left) - get_height(node->right) : 0; }

static Node *rotate_right(Node *y) {
  Node *x = y->left;
  Node *T2 = x->right;
  x->right = y;
  y->left = T2;
  y->height = max(get_height(y->left), get_height(y->right)) + 1;
  x->height = max(get_height(x->left), get_height(x->right)) + 1;
  return x;
}

static Node *rotate_left(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;
  y->left = x;
  x->right = T2;
  x->height = max(get_height(x->left), get_height(x->right)) + 1;
  y->height = max(get_height(y->left), get_height(y->right)) + 1;
  return y;
}

Node *insert_avl(Node *root, void *data, int id, const char *name) {
  if (!root) {
    Node *new = malloc(sizeof(Node));
    if (!new)
      return NULL;
    new->data = data;
    new->id = id;
    strncpy(new->name, name, sizeof(new->name) - 1);
    new->name[sizeof(new->name) - 1] = '\0';
    new->left = new->right = NULL;
    new->height = 1;
    return new;
  }

  if (id < root->id) {
    root->left = insert_avl(root->left, data, id, name);
  } else if (id > root->id) {
    root->right = insert_avl(root->right, data, id, name);
  } else {
    return root; // Duplicate IDs not allowed
  }

  root->height = max(get_height(root->left), get_height(root->right)) + 1;
  int balance = get_balance(root);

  // Left Left
  if (balance > 1 && id < root->left->id) {
    return rotate_right(root);
  }
  // Right Right
  if (balance < -1 && id > root->right->id) {
    return rotate_left(root);
  }
  // Left Right
  if (balance > 1 && id > root->left->id) {
    root->left = rotate_left(root->left);
    return rotate_right(root);
  }
  // Right Left
  if (balance < -1 && id < root->right->id) {
    root->right = rotate_right(root->right);
    return rotate_left(root);
  }

  return root;
}

Node *search_avl_by_id(Node *root, int id) {
  if (!root || root->id == id)
    return root;
  if (id < root->id)
    return search_avl_by_id(root->left, id);
  return search_avl_by_id(root->right, id);
}

Node *search_avl_by_name(Node *root, const char *name) {
  if (!root)
    return NULL;
  int cmp = stricmp(name, root->name);
  if (cmp == 0)
    return root;
  if (cmp < 0)
    return search_avl_by_name(root->left, name);
  return search_avl_by_name(root->right, name);
}

void free_avl(Node *root) {
  if (!root)
    return;
  free_avl(root->left);
  free_avl(root->right);
  free(root->data);
  free(root);
}

void insert_hash(HashNode *table[], int key, void *value) {
  int idx = key % HASH_SIZE;
  int count = 0;
  HashNode *node = table[idx];
  while (node) {
    count++;
    node = node->next;
  }
  if (count > 10) {
    printf("Writhing: %d collisions at index %d for key %d\n", count, idx, key);
  }

  HashNode *new = malloc(sizeof(HashNode));
  if (!new) {
    printf("Memory allocation failed for hash node.\n");
    return;
  }
  new->key = key;
  new->value = value;
  new->next = table[idx];
  table[idx] = new;
}

void *search_hash(HashNode *table[], int key) {
  int idx = key % HASH_SIZE;
  HashNode *node = table[idx];
  while (node) {
    if (node->key == key)
      return node->value;
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
      free(temp->value);
      free(temp);
    }
    table[i] = NULL;
  }
}

void delete_hash(HashNode *table[], int key) {
  int idx = key % HASH_SIZE;
  HashNode *node = table[idx];
  HashNode *prev = NULL;
  while (node && node->key != key) {
    prev = node;
    node = node->next;
  }
  if (!node)
    return;
  if (prev) {
    prev->next = node->next;
  } else {
    table[idx] = node->next;
  }
  free(node->value);
  free(node);
}

void init_customer_hash_table() {
  FILE *file = fopen(CUSTOMERS_FILE, "r");
  if (!file) {
    printf("Error opening customers file for hash table initialization.\n");
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    Customer *c = malloc(sizeof(Customer));
    if (!c) {
      printf("Memory allocation failed for customer.\n");
      continue;
    }
    if (parse_customer(line, c)) {
      insert_hash(customer_hash_table, c->id, c);
    } else {
      free(c);
    }
  }
  fclose(file);
}

void save_customers_from_hash() {
  FILE *file = fopen(CUSTOMERS_FILE, "w");
  if (!file) {
    printf("Error opening customers file for save.\n");
    return;
  }

  for (int i = 0; i < HASH_SIZE; i++) {
    HashNode *node = customer_hash_table[i];
    while (node) {
      Customer *c = (Customer *)node->value;
      print_customer(file, c);
      node = node->next;
    }
  }

  fclose(file);
}
