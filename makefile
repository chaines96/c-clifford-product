clifford_calc: main.o clifford_prod.o 
	gcc main.o clifford_prod.o -o clifford_calc

main.o: main.c clifford_prod.h
	gcc -Wall -Wextra -g -c main.c

cliff_prod.o: clifford_prod.c clifford_prod.h
	gcc -Wall -Wextra -g -c clifford_prod.c

clean:
	rm -f clifford_calc *.core *.o