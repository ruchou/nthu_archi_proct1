TARGET=make

$(TARGET):  main.o
	gcc simulator.o -lm  -o single_cycle


main.o: 
	gcc -c  simulator.c -lm 

clean:
	rm -f *.o single_cycle 


