#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flowers.h"

int main(void) {
  int choice;

  while(1) {
    printf("\nEnter Numbers below to continue:\n");
    printf("1 - Existing Flowers\n");
    printf("2 - Add Flower\n");
    printf("3 - Search Flowers\n");
    printf("4 - Buy Flower\n");
    printf("5 - Exit\n");
    // printf("Your choice: ");
    scanf("%d", &choice);

    switch (choice) {
      case 1: list_flowers(); break;
      case 2: add_flower(); break;
      // case 3: ?; break;
      // case 4: ?; break;
      case 5: exit(0);
      default: printf("Invalid input. Try again.\n");
    }
  }
  return 0;
}
