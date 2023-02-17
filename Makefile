all: runAll

runAll: barber

phsp: barber.c
	gcc barber.c -o barber
	./run

clean:
	rm -rf *o hello
