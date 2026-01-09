CC = gcc
CFLAGS = -Wall -Wextra -O2

cliff_prod: cliff_prod.c
	$(CC) $(CFLAGS) -o cliff_prod cliff_prod.c

clean:
	rm -f cliff_prod
