all: kicad-fbp

kicad-fbp: ./tinyxml2/libtinyxml2.a main.o parser.o
	g++ main.o parser.o ./tinyxml2/libtinyxml2.a -o kicad-fbp

main.o: main.cpp datastructure.h parser.h
	g++ main.cpp -c -o main.o -std=c++17

parser.o: parser.cpp parser.h
	g++ parser.cpp -c -o parser.o

./tinyxml2/libtinyxml2.a: 
	git clone https://github.com/leethomason/tinyxml2
	make -C tinyxml2

clean:
	rm -f *.o
	rm -f kicad-fbp

run: all
	./kicad-fbp schematic.xml