all: kicad-fbp

kicad-fbp: ./tinyxml2/libtinyxml2.a main.o parser.o dot.o fbp.o
	g++ main.o parser.o dot.o fbp.o ./tinyxml2/libtinyxml2.a -o kicad-fbp

main.o: main.cpp datastructure.h parser.h dot.h fbp.h
	g++ main.cpp -c -o main.o -std=c++17

parser.o: parser.cpp parser.h
	g++ parser.cpp -c -o parser.o

dot.o: dot.cpp dot.h
	g++ dot.cpp -c -o dot.o

fbp.o: fbp.cpp fbp.h
	g++ fbp.cpp -c -o fbp.o

./tinyxml2/libtinyxml2.a: 
	git clone https://github.com/leethomason/tinyxml2
	make -C tinyxml2

clean:
	rm -f *.o
	rm -f kicad-fbp

run: all
	./kicad-fbp schematic.xml