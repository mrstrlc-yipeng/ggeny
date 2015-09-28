CC=gcc

all: ggeny tests

ggeny: ggeny.o hashids.o
	$(CC) -o $@ $^ -lm

tests: tests.o hashids.o
	$(CC) -o $@ $^ -lm

ggeny.o: src/ggeny.c hashids.o
	$(CC) -c -o $@ $<

tests.o: src/tests.c hashids.o
	$(CC) -c -o $@ $<

hashids.o: lib/hashids/hashids.c lib/hashids/hashids.h
	$(CC) -c -o $@ $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ggeny tests
