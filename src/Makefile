
build: test.o parseutils.o machinelang.o
	${CC} machinelang.o parseutils.o test.o -o asmc 

test.o: test.c ac1800.h
	${CC} -std=gnu89 -Wall \
   -c test.c -O3 -o test.o
 
parseutils.o: parseutils.c
	${CC} -std=gnu89 -Wall \
   -c parseutils.c -O3 -o parseutils.o
 

machinelang.o: machinelang.c ac1800.h
	${CC} -std=gnu89 -Wall -Wno-multichar \
  -c machinelang.c -O3 -o machinelang.o
run: build
	exec ./asmc

clean:
	rm parseutils.o test.o machinelang.o
