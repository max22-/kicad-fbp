all: kicad-fbp

kicad-fbp: ./tinyxml2/libtinyxml2.a main.o connection.o process.o
	g++ main.o connection.o process.o ./tinyxml2/libtinyxml2.a -o kicad-fbp

main.o: main.cpp process.h
	g++ main.cpp -c -o main.o

connection.o: connection.cpp connection.h process.h
	g++ connection.cpp -c -o connection.o

process.o: process.cpp process.h
	g++ process.cpp -c -o process.o

./tinyxml2/libtinyxml2.a: 
	git clone https://github.com/leethomason/tinyxml2
	make -C tinyxml2

clean:
	rm -f *.o
	rm -f kicad-fbp

run: all
	./kicad-fbp schematic.xml