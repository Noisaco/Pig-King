#include<stdio.h>
#include<stdbool.h>
#include"./tigr/tigr.h"
int menu() {
    int seleccion=0;


    tigrUpdate(screen);
    return seleccion;
}
int juego() {

    return 0;
}
int selector_de_niveles() {

}
int main() {
    int caso;
    int ancho_ventana=800, alto_ventana=800; // Establecer dimensiones de la ventana del juego.
    Tigr *screen;
    screen=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", 0);
    tigrClear(screen, tigrRGB(0, 0, 0)); // Creacion de la ventana junto con el color de fondo.
    caso=menu();
    switch(caso) { 
        case 1:
                break;
        case 2: 
                break;
    }
    tigrFree(screen); // Cierre de la ventana de juego.
    return 0;
}