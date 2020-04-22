all: example1.cpp

example1.cpp: example1.xml
	../kicad-fbp -f froud example1.xml example1.cpp

.PHONY: clean
clean:
	rm -f *.fbp *.dot *.cpp