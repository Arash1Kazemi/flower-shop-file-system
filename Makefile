CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJ = flowers.o helpers.o main.o optimizations.o pot.o customer_menu.o customer_portal.o customer_shopping.o customer.o payment.o customer_hash.o
TARGET = bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c shop.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
