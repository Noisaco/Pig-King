#include<stdio.h> // Se copila con gcc ./tigr/tigr.c juego.c -o juego -I./tigr -lGLU -lGL -lX11 -lm
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
    struct coordenadas cursor, cortina, plantilla, titulo, sombra, boton_jugar, boton_niveles, boton_puntuacion;
    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0; // Establecimiento de la posicion anterior del raton.
    cortina.x=0, cortina.y=0, cortina.w=ancho_ventana, cortina.h=2*alto_ventana/8;

    titulo.y=0, titulo.w=251, titulo.h=40;
    titulo.x=(ancho_ventana-titulo.w)/2, titulo.i=tigrBitmap(titulo.w, titulo.h);

    sombra.y=0, sombra.w=titulo.w, sombra.h=titulo.h;
    sombra.x=titulo.x+4, sombra.i=tigrBitmap(sombra.w, sombra.h);

    plantilla.x=ancho_ventana/8, plantilla.y=alto_ventana, plantilla.w=6*ancho_ventana/8, plantilla.h=10*alto_ventana/16;

    boton_jugar.x=ancho_ventana, boton_jugar.y=alto_ventana, boton_jugar.w=0, boton_jugar.h=2*alto_ventana/16;
    boton_niveles.x=ancho_ventana, boton_niveles.y=alto_ventana, boton_niveles.w=0, boton_niveles.h=2*alto_ventana/16;
    boton_puntuacion.x=ancho_ventana, boton_puntuacion.y=alto_ventana, boton_puntuacion.w=0, boton_puntuacion.h=2*alto_ventana/16;

    int aparicion_botones=1, seleccion=0, click, last_click, botes=0, elevacion=0, i, j;

    float tiempo_inicial=0, tiempo_final=0, tiempo_total=0, tiempo_animacion=0, tiempo_parpadeo=0, tiempo_botones=0;// Los diferentes controladores de tiempo.
    float resguardo=0 , velocidad_menu=0, velocidad_plantilla=250, aceleracion=1000;

    bool plantilla_terminada=false, seleccionado=false, permitido=false, opening=true;

    TPixel color_morado=tigrRGB(160, 0, 100), color_azul=tigrRGB(69, 0, 98), efecto_sombra, brillo, color_amarillo1=tigrRGB(40, 131, 250), color_amarillo2=tigrRGB(252, 221, 78), color_sombra=tigrRGB(26, 6, 92), color_azul_claro=tigrRGB(116, 130, 252);
    Tigr *sombra_o_titulo;


    printf("Ventana del juego abierta.\n");
    brillo=color_amarillo1;
    while(!seleccionado && !tigrClosed(screen) && !tigrKeyHeld(screen, TK_ESCAPE)) {
        tiempo_final=tigrTime();
        tiempo_total=(tiempo_final-tiempo_inicial);
        tiempo_animacion+=tiempo_total;
        tiempo_inicial=tigrTime();
        if((resguardo==(2*alto_ventana/8)) && (velocidad_menu>0) && (botes!=4)) { // Numero de botes del titulo.
            velocidad_menu=-velocidad_menu*0.7;
            botes++;
        }
        if(botes!=4) { // Desplazamiento del titulo.
            velocidad_menu+=aceleracion*tiempo_total;
            resguardo+=velocidad_menu*tiempo_total;
            cortina.y=resguardo;
            titulo.y=resguardo-70;
            sombra.y=resguardo-70;
        }
        if(resguardo>(2*alto_ventana/8)) {
            resguardo=2*alto_ventana/8;
        }
        if((tiempo_animacion>2.5) && (plantilla_terminada==false)) { // Desplazamiento de la plantilla.
            plantilla.y-=velocidad_plantilla*tiempo_total;
            if(plantilla.y<=(5*alto_ventana/16)) {
                plantilla.y=5*alto_ventana/16;
                plantilla_terminada=true;
            }
        }
        if(tiempo_animacion>2) { // Parpadeo del menu.
            tiempo_parpadeo+=tiempo_total;
            if(tiempo_parpadeo>2) {
                tiempo_botones+=tiempo_total;
                if(tiempo_botones>0.25)
                brillo=color_amarillo2;
                tiempo_parpadeo=0;
                elevacion=5;
            }
            else if(tiempo_parpadeo>1) {
                brillo=color_amarillo1;
                elevacion=0;
            }
        }
        if(plantilla_terminada==true) { // Aparicion de los botones.
            tiempo_botones+=tiempo_total;
            switch(aparicion_botones) {
                case 1: boton_jugar.w=9*ancho_ventana/16;
                        boton_jugar.h=3*alto_ventana/16;
                        boton_jugar.x=3.5*ancho_ventana/16;
                        boton_jugar.y=5.5*alto_ventana/16;
                        if(tiempo_botones>0.25) {
                            boton_jugar.w=8*ancho_ventana/16;
                            boton_jugar.h=2*alto_ventana/16;
                            boton_jugar.x=4*ancho_ventana/16;
                            boton_jugar.y=6*alto_ventana/16;
                            tiempo_botones=0;
                            aparicion_botones=2;
                        }
                        
                        break;
                case 2: boton_niveles.w=9*ancho_ventana/16;
                        boton_niveles.h=3*alto_ventana/16;
                        boton_niveles.x=3.5*ancho_ventana/16;
                        boton_niveles.y=8.5*alto_ventana/16;

                            
                        if(tiempo_botones>0.25) {
                            boton_niveles.w=8*ancho_ventana/16;
                            boton_niveles.h=2*alto_ventana/16;
                            boton_niveles.x=4*ancho_ventana/16;
                            boton_niveles.y=9*alto_ventana/16;
                            tiempo_botones=0;
                            aparicion_botones=3;
                        }
                        break;
                case 3: boton_puntuacion.w=9*ancho_ventana/16;
                        boton_puntuacion.h=3*alto_ventana/16;
                        boton_puntuacion.x=3.5*ancho_ventana/16;
                        boton_puntuacion.y=11.5*alto_ventana/16;
                        if(tiempo_botones>0.25) {
                            boton_puntuacion.w=8*ancho_ventana/16;
                            boton_puntuacion.h=2*alto_ventana/16;
                            boton_puntuacion.x=4*ancho_ventana/16;
                            boton_puntuacion.y=12*alto_ventana/16;
                            aparicion_botones=4;
                        }
                        break;
            }
        }
        if(click==1 && !opening) {
            if(permitido) {
                seleccionado=true;
            }
        }
        tigrSetPostFX(screen, 1, 1, 1, 2.0f); // Inserta el efecto de television de tubos.
        tigrClear(screen, tigrRGB(0, 0, 0)); // Limpia la pantalla.
        tigrFill(screen, cortina.x, 0, cortina.w, cortina.y, color_azul); // Cortina del menu.
        tigrBlitAlpha(screen, sombra.i, sombra.x+(elevacion*0.5), sombra.y, 0, 0, sombra.w, sombra.h, 1);
        tigrBlitAlpha(screen, titulo.i, titulo.x, titulo.y-elevacion, 0, 0, titulo.w, titulo.h, 1);
        tigrFill(screen, cortina.x, cortina.y, cortina.w, 5, color_morado);
        for(i=1; i<=2; i++) {
            if(i==1) {
                sombra_o_titulo=sombra.i;
                efecto_sombra=color_sombra;
            }
            else {
                sombra_o_titulo=titulo.i;
                efecto_sombra=brillo;
            }
            for(j=0; j<10; j++) { // Dibuja el texto del titulo.
                // La letra P.
                tigrLine(sombra_o_titulo, 0+j, 40, 10+j, 0, efecto_sombra);
                tigrLine(sombra_o_titulo, 10, 0+j, 40-j, 0+j, efecto_sombra);
                tigrLine(sombra_o_titulo, 40-j, 0, 33-j, 30, efecto_sombra);
                tigrLine(sombra_o_titulo, 33, 30-j, 10, 30-j, efecto_sombra);
                // La letra I.
                tigrLine(sombra_o_titulo, 40+j, 40, 50+j, 0, efecto_sombra);
                // La letra G.
                tigrLine(sombra_o_titulo, 60+j, 40, 70+j, 0, efecto_sombra);
                tigrLine(sombra_o_titulo, 60+j, 40-j, 90, 40-j, efecto_sombra);
                tigrLine(sombra_o_titulo, 90-j, 40, 95-j, 20, efecto_sombra);
                tigrLine(sombra_o_titulo, 70, 0+j, 95, 0+j, efecto_sombra);
                tigrLine(sombra_o_titulo, 100-j, 0, 97-j, 10, efecto_sombra);
                // La letra K.
                tigrLine(sombra_o_titulo, 110+j, 40, 120+j, 0, efecto_sombra);
                tigrLine(sombra_o_titulo, 123+j, 20, 130+j, 40, efecto_sombra);
                tigrLine(sombra_o_titulo, 123+j, 20, 140+j, 0, efecto_sombra);
                // La letra I.
                tigrLine(sombra_o_titulo, 150+j, 40, 160+j, 0, efecto_sombra);
                // La letra N.
                tigrLine(sombra_o_titulo, 170+j, 40, 180+j, 0, efecto_sombra);
                tigrLine(sombra_o_titulo, 180+j, 0, 190+j, 40, efecto_sombra);
                tigrLine(sombra_o_titulo, 190+j, 40, 200+j, 0, efecto_sombra);
                // La letra G.
                tigrLine(sombra_o_titulo, 210+j, 40, 220+j, 0, efecto_sombra);
                tigrLine(sombra_o_titulo, 210+j, 40-j, 240, 40-j, efecto_sombra);
                tigrLine(sombra_o_titulo, 240-j, 40, 245-j, 20, efecto_sombra);
                tigrLine(sombra_o_titulo, 220, 0+j, 245, 0+j, efecto_sombra);
                tigrLine(sombra_o_titulo, 250-j, 0, 247-j, 10, efecto_sombra);
            }
        }
        tigrFill(screen, plantilla.x, plantilla.y, plantilla.w, plantilla.h, color_azul);
        // Macos plantilla.
        tigrFill(screen, plantilla.x, plantilla.y, plantilla.w, 3, color_morado);
        tigrFill(screen, plantilla.x, plantilla.y, 3, plantilla.h, color_morado);
        tigrFill(screen, plantilla.x+plantilla.w-3, plantilla.y, 3, plantilla.h, color_morado);
        tigrFill(screen, plantilla.x, plantilla.y+plantilla.h-3, plantilla.w, 3, color_morado);
        
        tigrFill(screen, boton_jugar.x, boton_jugar.y, boton_jugar.w, boton_jugar.h, color_azul_claro);
        tigrFill(screen, boton_niveles.x, boton_niveles.y, boton_niveles.w, boton_niveles.h, color_azul_claro);
        tigrFill(screen, boton_puntuacion.x, boton_puntuacion.y, boton_puntuacion.w, boton_puntuacion.h, color_azul_claro);
        // Marcos boton jugar.
        tigrFill(screen, boton_jugar.x, boton_jugar.y, boton_jugar.w, 2, brillo);
        tigrFill(screen, boton_jugar.x, boton_jugar.y, 2, boton_jugar.h, brillo);
        tigrFill(screen, boton_jugar.x+boton_jugar.w-2, boton_jugar.y, 2, boton_jugar.h, brillo);
        tigrFill(screen, boton_jugar.x, boton_jugar.y+boton_jugar.h-2, boton_jugar.w, 2, brillo);
        // Marcos boton niveles.
        tigrFill(screen, boton_niveles.x, boton_niveles.y, boton_niveles.w, 2, brillo);
        tigrFill(screen, boton_niveles.x, boton_niveles.y, 2, boton_niveles.h, brillo);
        tigrFill(screen, boton_niveles.x+boton_niveles.w-2, boton_niveles.y, 2, boton_niveles.h, brillo);
        tigrFill(screen, boton_niveles.x, boton_niveles.y+boton_niveles.h-2, boton_niveles.w, 2, brillo);
        // Marcos boton puntuacion.
        tigrFill(screen, boton_puntuacion.x, boton_puntuacion.y, boton_puntuacion.w, 2, brillo);
        tigrFill(screen, boton_puntuacion.x, boton_puntuacion.y, 2, boton_puntuacion.h, brillo);
        tigrFill(screen, boton_puntuacion.x+boton_puntuacion.w-2, boton_puntuacion.y, 2, boton_puntuacion.h, brillo);
        tigrFill(screen, boton_puntuacion.x, boton_puntuacion.y+boton_puntuacion.h-2, boton_puntuacion.w, 2, brillo);
        // Textos de los botones.
        if(aparicion_botones>=2) {
            tigrPrint(screen, tfont, 7.35*ancho_ventana/16 , 6.85*alto_ventana/16, brillo, "JUGAR", 0);
        }
        if(aparicion_botones>=3) {
            tigrPrint(screen, tfont, 7*ancho_ventana/16 , 9.85*alto_ventana/16, brillo, "NIVELES", 0);

        }
        if(aparicion_botones==4) {
            tigrPrint(screen, tfont, 6.6*ancho_ventana/16 , 12.85*alto_ventana/16, brillo, "PUNTUACIÓN", 0);

        }
        tigrPrint(screen, tfont, 5, alto_ventana-15, tigrRGB(225, 225, 225), "Hecho por: Alexander Veldemar, Danil Laurov y Andres Maciá.", 0);
        tigrMouse(screen, &cursor.x, &cursor.y, &click); // Obtencion de la posicion del cursor y comprobacion de si se ha clicado.
        tigrBlitAlpha(screen, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1); // Pintar cursor personalizado.
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