CC=gcc

all: ggeny

ggeny: ggeny.o hashids.o
	$(CC) -o $@ $^ -lm

ggeny.o: src/ggeny.c ggenyrand.o ggenygraph.o ggenyio.o hashids.o
	$(CC) -c -o $@ $<

ggenyrand.o: src/ggenyrand.c src/ggenyrand.h
	$(CC) -c -o $@ $^

ggenygraph.o: src/ggenygraph.c src/ggenygraph.h
	$(CC) -c -o $@ $^

ggenyio.o: src/ggenyio.c src/ggenyio.h
	$(CC) -c -o $@ $^

hashids.o: lib/hashids/hashids.c lib/hashids/hashids.h
	$(CC) -c -o $@ $<

cutest.o: lib/cutest/CuTest.c lib/cutest/CuTest.h
	$(CC) -c -o $@ $<

clean:
	rm -rf *.o

mrproper: clean
	rm -rf ggeny
