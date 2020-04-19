CFLAGS=-std=c++17 -ggdb
LDFLAGS=

all: kicad-fbp

kicad-fbp: ./tinyxml2/libtinyxml2.a main.o datastructure.o parser.o dot.o fbp.o froud.o 
	g++ main.o datastructure.o parser.o dot.o fbp.o froud.o  ./tinyxml2/libtinyxml2.a -o kicad-fbp $(LDFLAGS)

main.o: main.cpp datastructure.h parser.h dot.h fbp.h froud.h
	g++ main.cpp -c -o main.o $(CFLAGS)

parser.o: parser.cpp parser.h
	g++ parser.cpp -c -o parser.o $(CFLAGS)

datastructure.o: datastructure.cpp datastructure.h
	g++ datastructure.cpp -c -o datastructure.o $(CFLAGS)

dot.o: dot.cpp dot.h
	g++ dot.cpp -c -o dot.o $(CFLAGS)

fbp.o: fbp.cpp fbp.h
	g++ fbp.cpp -c -o fbp.o  $(CFLAGS)

froud.o: froud.cpp froud.h
	g++ froud.cpp -c -o froud.o $(CFLAGS)


./tinyxml2/libtinyxml2.a: 
	git clone https://github.com/leethomason/tinyxml2
	make -C tinyxml2

.PHONY: clean
clean:
	rm -f *.o
	rm -f kicad-fbp

run: all
	./kicad-fbp schematic.xml