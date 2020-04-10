all: kicad-fbp

kicad-fbp: ./tinyxml2/libtinyxml2.a main.o
	g++ main.o ./tinyxml2/libtinyxml2.a -o kicad-fbp

main.o: main.cpp datastructure.h
	g++ main.cpp -c -o main.o

./tinyxml2/libtinyxml2.a: 
	git clone https://github.com/leethomason/tinyxml2
	make -C tinyxml2

clean:
	rm -f *.o
	rm -f kicad-fbp

run: all
	./kicad-fbp schematic.xml