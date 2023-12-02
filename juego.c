#include<stdio.h>
#include<stdbool.h>
#include"./tigr/tigr.h"
struct coordenadas { // Establecimiento del registro de coordenadas
        int x;
        int y;
    } raton;
int menu(Tigr *screen) { // Modulo que muestra el menu.
    struct coordenadas raton;
    raton.x=0;
    raton.y=0;
    int seleccion=0, click, last_click;
    bool seleccionado=false;
    while(!seleccionado && !tigrClosed(screen) && !tigrKeyHeld(screen, TK_ESCAPE)) {
        tigrPrint(screen, tfont, 120, 110, tigrRGB(0xff, 0xff, 0xff), "PIG KING");
        tigrMouse(screen, &raton.x, &raton.y, &click);
        if(click==1) {
            seleccionado=true;
        }
        tigrUpdate(screen);
    }
    return seleccion;
}
int juego() {

    return 0;
}
int selector_de_niveles() {

}
int main() {
    int caso;
    int ancho_ventana=800, alto_ventana=800; // Establecimiento de las dimensiones de la ventana del juego.
    Tigr *screen;
    screen=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", 0); // Abre la ventana del juego
    tigrClear(screen, tigrRGB(0, 0, 0)); // Estable el color de fondo.
    caso=menu(screen);
        switch(caso) {
            case 1:
                    break;
            case 2: 
                    break;
        }
    tigrFree(screen); // Cierre de la ventana de juego.
    return 0;
}


