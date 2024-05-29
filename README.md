# Jogo_Tron
Um Jogo super divertido, para a disciplina de PIF da Faculdade Cesar School. A proposta do jogo é inspirada no jogo de Arcade Tron (sim, o mesmo que inspirou o filme) e ele é feito na linguagem C.


cli-lib
Command Line Interface library, for developing CLI applications and games in C. It has functions to access keyboard, screen and manage timing tasks.

Requirements
This library works with the follwing OS:
Linux based (Ubuntu, etc)
MacOS
It is necessary to have GCC installed.
Usage
The file main.c has an example of how to use the Keyboard, Screen nd Timer functions.

To build this example via command line, just switch to library root directory and type the following command:

gcc ./src/*.c -I./include -o run
To run

./run
To use this library to create your own apps, simply add source and header files to your project and compile it together with your own code, replacing the main.c file to your own.
