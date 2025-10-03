CC = gcc
CFLAGS = -Wall -Wextra -std=c11

SRC = flowers.c helpers.c main.c optimizations.c pot.c customer_menu.c customer_portal.c customer_shopping.c customer.c payment.c
OBJ = $(SRC:.c=.o)
TARGET = bin

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c shop.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

