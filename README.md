# Kicad-FBP

## What is it ?
A Kicad netlist plugin. We are "hacking" Kicad to generate programs instead of electronic circuits.

Kicad is an an Electronic Design Automation Software. Why would we want to use it for Flow-Based Programming, which is not related to electronics ? Because it allows us to draw schematics, with components and links between them, that model our program. Components can have input and output ports, like FBP processes. We can draw our own components like we want. And Kicad generates netlists, which are a textual description of a schematic : what components are there, and how they are linked together.

The nice thing is that it is quite easy to make a plugin to generate the netlist format you want. So Kicad-FBP is a plugin to generate FBP programs.

Currently supported output format are :
- FBP notation
- Dot format (Graphviz)
- Froud (an experimental Flow-Based Programming library for the Arduino framework (made by me!)

JSON format will be implemented in a next release. (It is quite easy to implement a new output format).

## How to install it ?

```
sudo apt install git build-essential
git clone https://github.com/max22-/kicad-fbp
make -C kicad-fbp
sudo make -C kicad-fbp install
```

## Creating a network in Kicad

## Integrating kicad-fbp as a Kicad plugin

## Generating the code

## TODO

- Other output formats : JSON, Java, etc.
- Subnets : is it even possible ? I don't know.
- Specify connection capacity
- Support for "dropOldest"
