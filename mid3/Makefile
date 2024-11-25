SHELL := /bin/bash

p1: p1.c
	gcc -g -O3 -o $@ p1.c

p2: p2.c
	gcc -g -O3 -o $@ p2.c

q21: 
	for((i=0;i<100;i+=4)); do gcc -g -O3 -o p2 -DDS=$$i p2.c; ./p2; done
	./p2

q22:
	for((i=0;i<100;i+=4)); do gcc -g -DUSE_MUTEX -O3 -o p2 -DDS=$$i p2.c; ./p2; done
	./p2

q3: p3.c
	gcc -g -DRECV -O3 -o $@ p3.c
q31: p3.c
	gcc -g -O3 -o $@ p3.c
q32: p3.c
	gcc -g -DREVRECV -O3 -o $@ p3.c

clean: 
	rm p1 p2