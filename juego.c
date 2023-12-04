#include<stdio.h>
#include <math.h>
#include<stdbool.h>
#include"./tigr/tigr.h"
#define TIGR_NOCURSOR 32
struct coordenadas { // Establecimiento del registro de imagen, coordenadas y dimensiones de los diferentes elementos del juego.
        Tigr *i;
        int x;
        int y;
        int w;
        int h;
    };
int menu(Tigr *screen, int ancho_ventana, int alto_ventana) { // Modulo que muestra el menu.
    struct coordenadas cursor, barra;
    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0; // Establecimiento de la posicion anterior del raton.
    barra.x=0, barra.y, barra.w=ancho_ventana, barra.h=5;
    int seleccion=0, click, last_click, aceleracion=0;
    float tiempo;
    bool seleccionado=false, permitido=false, opening=true;
    TPixel color_morado = tigrRGB(160, 0, 100);
    while(!seleccionado && !tigrClosed(screen) && !tigrKeyHeld(screen, TK_ESCAPE)) {
//        tigrPrint(screen, tfont, 120, 110, tigrRGB(0, 342, 0), "PIG KING");
        if (barra.y==2*alto_ventana/8) {
            aceleracion=20;
            tiempo=0;
        }
        tiempo+=tigrTime(); // Gravedad.
        barra.y=barra.y+exp(2*tiempo)-aceleracion;
        if(barra.y>=2*alto_ventana/8) {
            barra.y=2*alto_ventana/8;
        }
        tigrClear(screen, tigrRGB(0, 0, 0)); // Limpia la pantalla.
        tigrFill(screen, barra.x, barra.y, barra.w, barra.h, color_morado);
        tigrMouse(screen, &cursor.x, &cursor.y, &click); // Comprobacion de si se ha pulsado un boton
        if(click==1 && !opening) {
            if(permitido) {
                seleccionado=true;
            }
        }
        tigrBlitAlpha(screen, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1);
        tigrUpdate(screen);
        printf("%f", tiempo);
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
    screen=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", 32); // Abre la ventana del juego
    caso=menu(screen, ancho_ventana, alto_ventana);
        switch(caso) {
            case 1:
                    break;
            case 2: 
                    break;
        }
    tigrFree(screen); // Cierre de la ventana de juego.
    return 0;
}