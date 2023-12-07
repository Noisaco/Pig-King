#include<stdio.h>
#include <math.h>
#include<stdbool.h>
#include"./tigr/tigr.h"
struct coordenadas { // Establecimiento del registro de imagen, coordenadas y dimensiones de los diferentes elementos del juego.
        Tigr *i;
        int x;
        int y;
        int w;
        int h;
    };
int menu(Tigr *screen, int ancho_ventana, int alto_ventana) { // Modulo que muestra el menu.
    struct coordenadas cursor, barra, plantilla;
    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0; // Establecimiento de la posicion anterior del raton.
    barra.x=0, barra.y=0, barra.w=ancho_ventana, barra.h=5;
    plantilla.x=ancho_ventana/4, plantilla.y=0, plantilla.w=2*ancho_ventana/4, plantilla.h=4*alto_ventana/8;
    int seleccion=0, click, last_click, botes=0, i;
    float resguardo=0 , velocidad=0, tiempo_inicial=0, tiempo_final=0, tiempo_total=0, tiempo_animacion=0, aceleracion=1000;
    bool seleccionado=false, permitido=false, opening=true;
    TPixel color_morado=tigrRGB(160, 0, 100), color_azul=tigrRGB(69, 0, 98), color_amarillo1=tigrRGB(252, 248, 167);
    
    printf("Ventana del juego abierta.\n");
    while(!seleccionado && !tigrClosed(screen) && !tigrKeyHeld(screen, TK_ESCAPE)) {
        tiempo_final=tigrTime();
        tiempo_total=(tiempo_final-tiempo_inicial);
        tiempo_animacion+=tiempo_total;
        tiempo_inicial=tigrTime();
        if(resguardo>=2*alto_ventana/8 && velocidad>0 && botes!=4) { // Numero de botes del titulo.
            velocidad=-velocidad*0.7;
            botes++;
        }
        if(botes!=4) { // Desplazamiento del titulo.
            velocidad+=aceleracion*tiempo_total;
            resguardo+=velocidad*tiempo_total;
            barra.y=resguardo;
        }
        if(tiempo_animacion>5 && plantilla.y>(3*alto_ventana/8)) { // Desplazamiento de la plantilla.
            velocidad=20;
            plantilla.y-=velocidad*tiempo_total;
        }
        if(click==1 && !opening) {
            if(permitido) {
                seleccionado=true;
            }
        }
        tigrSetPostFX(screen, 1, 1, 1, 2.0f); // Inserta el efecto de television de tubos.
        tigrClear(screen, tigrRGB(0, 0, 0)); // Limpia la pantalla.
        tigrFill(screen, barra.x, 0, barra.w, barra.y, color_azul); // Cortina del menu.
        tigrFill(screen, barra.x, barra.y, barra.w, barra.h, color_morado);
        for(i=0; i<10; i++) { // Dibuja el texto del titulo.
            // La letra P.
            tigrLine(screen, ancho_ventana/2-125+i, barra.y-30, ancho_ventana/2-115+i, barra.y-70, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-115, barra.y-70+i, ancho_ventana/2-85-i, barra.y-70+i, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-85-i, barra.y-70, ancho_ventana/2-93-i, barra.y-40, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-93, barra.y-40-i, ancho_ventana/2-115, barra.y-40-i, color_amarillo1);
            // La letra I.
            tigrLine(screen, ancho_ventana/2-85+i, barra.y-30, ancho_ventana/2-75+i, barra.y-70, color_amarillo1);
            // La letra G.
            tigrLine(screen, ancho_ventana/2-65+i, barra.y-30, ancho_ventana/2-55+i, barra.y-70, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-65+i, barra.y-30-i, ancho_ventana/2-35, barra.y-30-i, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-35-i, barra.y-30, ancho_ventana/2-30-i, barra.y-50, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-55, barra.y-70+i, ancho_ventana/2-30, barra.y-70+i, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-25-i, barra.y-70, ancho_ventana/2-28-i, barra.y-60, color_amarillo1);
            // La letra K.
            tigrLine(screen, ancho_ventana/2-15+i, barra.y-30, ancho_ventana/2-5+i, barra.y-70, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-2+i, barra.y-50, ancho_ventana/2+5+i, barra.y-30, color_amarillo1);
            tigrLine(screen, ancho_ventana/2-2+i, barra.y-50, ancho_ventana/2+15+i, barra.y-70, color_amarillo1);
            // La letra I.
            tigrLine(screen, ancho_ventana/2+25+i, barra.y-30, ancho_ventana/2+35+i, barra.y-70, color_amarillo1);
            // La letra N.
            tigrLine(screen, ancho_ventana/2+45+i, barra.y-30, ancho_ventana/2+55+i, barra.y-70, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+55+i, barra.y-70, ancho_ventana/2+65+i, barra.y-30, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+65+i, barra.y-30, ancho_ventana/2+75+i, barra.y-70, color_amarillo1);
            // La letra G.
            tigrLine(screen, ancho_ventana/2+85+i, barra.y-30, ancho_ventana/2+95+i, barra.y-70, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+85+i, barra.y-30-i, ancho_ventana/2+115, barra.y-30-i, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+115-i, barra.y-30, ancho_ventana/2+120-i, barra.y-50, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+95, barra.y-70+i, ancho_ventana/2+120, barra.y-70+i, color_amarillo1);
            tigrLine(screen, ancho_ventana/2+125-i, barra.y-70, ancho_ventana/2+122-i, barra.y-60, color_amarillo1);
        }
        tigrFill(screen, plantilla.x, plantilla.y, plantilla.w, plantilla.y, color_azul);
        //tigrPrint(screen, tfont, 120, 110, tigrRGB(224, 13, 40), "PIG KING", 600);
        tigrMouse(screen, &cursor.x, &cursor.y, &click); // Obtencion de la posicion del cursor y comprobacion de si se ha clicado.
        tigrBlitAlpha(screen, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1); //Pintar cursor personalizado.
        tigrUpdate(screen);
    }
    printf("%f", tiempo_animacion);
    return seleccion;
}
int juego() {

    return 0;
}
int selector_de_niveles() {

}
int main() {
    int caso;
    int ancho_ventana=400, alto_ventana=400; // Establecimiento de las dimensiones de la ventana del juego.
    Tigr *screen;

    printf("Se esta abriendo la ventana del juego...");
    screen=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", TIGR_NOCURSOR | TIGR_2X); // Abre la ventana del juego
    caso=menu(screen, ancho_ventana, alto_ventana);
        switch(caso) {
            case 1:
                    break;
            case 2: 
                    break;
        }
    tigrFree(screen); // Cierre de la ventana de juego.
    printf("Juego cerrado.\n");
    return 0;
}