all: plane planes

planes: planes.o plane.h
	gcc -o planes planes.o -lpthread -lrt

planes.o: planes.c plane.h
	-gcc -c planes.c -lpthread


plane: plane.o plane.h
	gcc -o plane plane.o -lpthread -lrt

plane.o: plane.c plane.h
	-gcc -c plane.c -lpthread


clean:
	-rm -f plane planes *.o
