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
    barra.x=0, barra.y=0, barra.w=ancho_ventana, barra.h=5;
    int seleccion=0, click, last_click, botes=0;
    float tiempo=0, aceleracion=0;
    bool seleccionado=false, permitido=false, opening=true;
    TPixel color_morado=tigrRGB(160, 0, 100);
    TPixel color_azul=tigrRGB(69, 0, 98);
    while(!seleccionado && !tigrClosed(screen) && !tigrKeyHeld(screen, TK_ESCAPE)) {
        if (barra.y==2*alto_ventana/8 && botes!=3) { // Control del numero de botes y absorcion de golpe del menu.
            aceleracion=2.68-botes;
            botes++;
            tiempo=0;
            
        }
        if(barra.y<=(barra.y+exp(2.7*tiempo)-aceleracion) && aceleracion!=0) {
            tiempo=0;
            aceleracion=0;
        }
        if(barra.y>2*alto_ventana/8) {
            barra.y=2*alto_ventana/8;
        }
        else if(botes!=3) { // Desplazamiento del menu.
            barra.y+=exp(2.7*tiempo)-aceleracion;
        }
        if(click==1 && !opening) {
            if(permitido) {
                seleccionado=true;
            }
        }
        tigrSetPostFX(screen, 1, 1, 1, 2.0f);
        tigrClear(screen, tigrRGB(0, 0, 0)); // Limpia la pantalla.
        tigrFill(screen, 0, 0, barra.w, barra.y, color_azul); // Pintar menu.
        tigrFill(screen, barra.x, barra.y, barra.w, barra.h, color_morado);
        tigrPrint(screen, tfont, 120, 110, tigrRGB(224, 13, 40), "PIG KING", 600);
        tigrMouse(screen, &cursor.x, &cursor.y, &click); // Obtencion de la posicion del cursor y comprobacion de si se ha clicado.
        tigrBlitAlpha(screen, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1); //Pintar cursor personalizado.
        tigrUpdate(screen);
        tiempo+=tigrTime(); // El tiempo.
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
