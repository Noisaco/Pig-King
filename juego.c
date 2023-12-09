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
// Modulo que muestra el menu.
int menu(Tigr *pantalla, int ancho_ventana, int alto_ventana) {
    struct coordenadas cursor, cortina, plantilla, titulo, sombra, boton_jugar, boton_niveles, boton_puntuacion, pantalla_menu;
    
    pantalla_menu.x=0, pantalla_menu.y=0, pantalla_menu.w=ancho_ventana, pantalla_menu.h=alto_ventana;
    pantalla_menu.i=tigrBitmap(pantalla_menu.w, pantalla_menu.h);

    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0; // Establecimiento de la posicion anterior del raton.
    cortina.x=0, cortina.y=0, cortina.w=pantalla_menu.w, cortina.h=2*pantalla_menu.h/8;

    titulo.y=0, titulo.w=251, titulo.h=40;
    titulo.x=(ancho_ventana-titulo.w)/2, titulo.i=tigrBitmap(titulo.w, titulo.h);

    sombra.y=0, sombra.w=titulo.w, sombra.h=titulo.h;
    sombra.x=titulo.x+4, sombra.i=tigrBitmap(sombra.w, sombra.h);

    plantilla.x=ancho_ventana/8, plantilla.y=alto_ventana, plantilla.w=6*ancho_ventana/8, plantilla.h=10*alto_ventana/16;
    // Creacion de las coordenadas y las dimensiones de los botones.
    boton_jugar.x=ancho_ventana, boton_jugar.y=alto_ventana, boton_jugar.w=0, boton_jugar.h=2*alto_ventana/16;
    boton_niveles.x=ancho_ventana, boton_niveles.y=alto_ventana, boton_niveles.w=0, boton_niveles.h=2*alto_ventana/16;
    boton_puntuacion.x=ancho_ventana, boton_puntuacion.y=alto_ventana, boton_puntuacion.w=0, boton_puntuacion.h=2*alto_ventana/16;

    int aparicion_botones=1, seleccion=0, click=0, last_click=0, botes=0, elevacion=0, elevacion_cierre=0, i=0, j=0;
    // Creacion de los diferentes controladores de tiempo.
    float tiempo_inicial=0, tiempo_final=0, tiempo_total=0, tiempo_animacion=0, tiempo_parpadeo=0, tiempo_botones=0;
    float resguardo=0, velocidad_ending=1000,velocidad_titulo=0, velocidad_plantilla=250, aceleracion=1000;
    bool plantilla_terminada=false, seleccionado=false, permitido=false, opening=true, ending=false;

    TPixel color_morado=tigrRGB(160, 0, 100), color_azul=tigrRGB(69, 0, 98), cuerpo_o_sombra, tema_actual, tema1=tigrRGB(40, 131, 250), tema2=tigrRGB(252, 221, 78), color_sombra=tigrRGB(26, 6, 92);
    // Creacion del color de los botones.
    TPixel presion_boton_jugar, presion_boton_niveles, presion_boton_puntuacion, si_presionado=tigrRGB(70,83,199),  no_presionado=tigrRGB(116, 130, 252), color_seleccionado=tigrRGB(9, 176, 70);
    // Creacion del tema de los botones.
    TPixel tema_boton_jugar, tema_boton_niveles, tema_boton_puntuacion;
    Tigr *transparencia_titulo;

    printf("Ventana del juego abierta.\n");
    // Tema inicial del menu.
    tema_actual=tema1;
    while(!seleccionado && !tigrClosed(pantalla) && !tigrKeyHeld(pantalla, TK_ESCAPE)) {
        // Calculo del tiempo que tarda el ordenador en ejecutar un ciclo del programa.
        tiempo_final=tigrTime();
        tiempo_total=(tiempo_final-tiempo_inicial);
        tiempo_animacion+=tiempo_total;
        tiempo_inicial=tigrTime();
        // Efecto de bote de la cortina del titulo.
        if((resguardo==(2*alto_ventana/8)) && (velocidad_titulo>0) && (botes!=4)) { 
            velocidad_titulo=-velocidad_titulo*0.7;
            botes++;
        }
        // Desplazamiento de la cortina del titulo.
        if(botes!=4) {
            velocidad_titulo+=aceleracion*tiempo_total;
            resguardo+=velocidad_titulo*tiempo_total;
            cortina.y=resguardo;
            titulo.y=resguardo-70;
            sombra.y=resguardo-70;
        }
        if(resguardo>(2*alto_ventana/8)) {
            resguardo=2*alto_ventana/8;
        }
        // Desplazamiento de la plantilla.
        if((tiempo_animacion>2.5) && (plantilla_terminada==false)) {
            plantilla.y-=velocidad_plantilla*tiempo_total;
            if(plantilla.y<=(5*alto_ventana/16)) {
                plantilla.y=5*alto_ventana/16;
                plantilla_terminada=true;
            }
        }
        // Parpadeo del menu.
        if(tiempo_animacion>2.6) {
            // Controlador del tiempo de duracion de los cambios de tema.
            tiempo_parpadeo+=tiempo_total;
            // Seleccion del tema 1.
            if(tiempo_parpadeo>2) {
                tema_actual=tema2;
                tiempo_parpadeo=0;
                elevacion=5;
            }
            // Seleccion del tema 2.
            else if(tiempo_parpadeo>1) {
                tema_actual=tema1;
                elevacion=0;
            }
        }
        // Aparcion de los botones.
        if((plantilla_terminada==true) && (opening==true)) {
            tiempo_botones+=tiempo_total;
            switch(aparicion_botones) {
                // Aparicion del boton Jugar.
                case 1: boton_jugar.w=9*ancho_ventana/16;
                        boton_jugar.h=3*alto_ventana/16;
                        boton_jugar.x=3.5*ancho_ventana/16;
                        boton_jugar.y=5.5*alto_ventana/16;
                        if(tiempo_botones>0.2) {
                            boton_jugar.w=8*ancho_ventana/16;
                            boton_jugar.h=2*alto_ventana/16;
                            boton_jugar.x=4*ancho_ventana/16;
                            boton_jugar.y=6*alto_ventana/16;
                            tiempo_botones=0;
                            aparicion_botones=2;
                        }
                        
                        break;
                // Aparicion del boton Niveles.
                case 2: boton_niveles.w=9*ancho_ventana/16;
                        boton_niveles.h=3*alto_ventana/16;
                        boton_niveles.x=3.5*ancho_ventana/16;
                        boton_niveles.y=8.5*alto_ventana/16;  
                        if(tiempo_botones>0.2) {
                            boton_niveles.w=8*ancho_ventana/16;
                            boton_niveles.h=2*alto_ventana/16;
                            boton_niveles.x=4*ancho_ventana/16;
                            boton_niveles.y=9*alto_ventana/16;
                            tiempo_botones=0;
                            aparicion_botones=3;
                        }
                        break;
                // Aparicion del boton Puntuacion.
                case 3: boton_puntuacion.w=9*ancho_ventana/16;
                        boton_puntuacion.h=3*alto_ventana/16;
                        boton_puntuacion.x=3.5*ancho_ventana/16;
                        boton_puntuacion.y=11.5*alto_ventana/16;
                        if(tiempo_botones>0.2) {
                            boton_puntuacion.w=8*ancho_ventana/16;
                            boton_puntuacion.h=2*alto_ventana/16;
                            boton_puntuacion.x=4*ancho_ventana/16;
                            boton_puntuacion.y=12*alto_ventana/16;
                            opening=false;
                        }
                        break;
            }
        }
        // Obtencion de la posicion del cursor y comprobacion de si se ha clicado.
        tigrMouse(pantalla, &cursor.x, &cursor.y, &click);
        // Comprobacion de si el cursor esta encima de algun boton y si ha clicado en el.
        if((!opening) && (!ending) && (cursor.x>=boton_jugar.x) && (cursor.y>=boton_jugar.y)&& (cursor.x<=boton_jugar.x+boton_jugar.w) && (cursor.y<=boton_jugar.y+boton_jugar.h)) {
            seleccion=1;
        }
        else if((!opening) && (!ending) && (cursor.x>=boton_niveles.x) && (cursor.y>=boton_niveles.y) && (cursor.x<=boton_niveles.x+boton_niveles.w) && (cursor.y<=boton_niveles.y+boton_niveles.h)) {
            seleccion=2;
        }
        else if((!opening) && (!ending) && (cursor.x>=boton_puntuacion.x) && (cursor.y>=boton_puntuacion.y) && (cursor.x<=boton_puntuacion.x+boton_puntuacion.w) && (cursor.y<=boton_puntuacion.y+boton_puntuacion.h)) {
            seleccion=3;
        }
        else if((!ending)) {
            seleccion=0;
        }
        presion_boton_jugar=no_presionado;
        presion_boton_niveles=no_presionado;
        presion_boton_puntuacion=no_presionado;
        tema_boton_jugar=tema_actual;
        tema_boton_niveles=tema_actual;
        tema_boton_puntuacion=tema_actual;
        switch(seleccion) {
            // Si el cursor no esta en ningun boton.
            case 0: presion_boton_jugar=no_presionado;
                    presion_boton_niveles=no_presionado;
                    presion_boton_puntuacion=no_presionado;
                    tema_boton_jugar=tema_actual;
                    tema_boton_niveles=tema_actual;
                    tema_boton_puntuacion=tema_actual;
                    break;
            case 1: presion_boton_jugar=si_presionado;
                    tema_boton_jugar=color_seleccionado;
                    if((click==1) && (last_click==0)) {
                        ending=true;
                    }
                    break;
            case 2: presion_boton_niveles=si_presionado;
                    tema_boton_niveles=color_seleccionado;
                    if((click==1) && (last_click==0)) {
                        ending=true;
                    }
                    break;
            case 3: presion_boton_puntuacion=si_presionado;
                    tema_boton_puntuacion=color_seleccionado;
                    if((click==1) && (last_click==0)) {
                        ending=true;
                    }
                    break;
        }
        last_click=click;
        // Inserta el efecto de television de tubos.
        tigrSetPostFX(pantalla, 1, 1, 1, 2.0f);
        // Comprobacion de si el menu se va ha cerrar.
        if(pantalla_menu.y<=-pantalla_menu.h) {
            seleccionado=true;
        }
        // Elevacion de la pantalla del menu.
        if(ending) {
            elevacion_cierre=velocidad_ending*tiempo_total;
            pantalla_menu.y-=elevacion_cierre;
        }
        // Limpia el fondo de pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        // Integracion de la pantalla del menu a la pantalla del programa y elevacion de cierre.
        tigrBlitAlpha(pantalla, pantalla_menu.i, pantalla_menu.x, pantalla_menu.y, 0, 0, pantalla_menu.w, pantalla_menu.h, 1);
        if(!ending) {
            // Limpia la pantalla.
            tigrClear(pantalla_menu.i, tigrRGB(0, 0, 0));
            tigrFill(pantalla_menu.i, cortina.x, 0, cortina.w, cortina.y, color_azul); // Cortina del menu.
            tigrBlitAlpha(pantalla_menu.i, sombra.i, sombra.x+(elevacion*0.5), sombra.y, 0, 0, sombra.w, sombra.h, 1);
            tigrBlitAlpha(pantalla_menu.i, titulo.i, titulo.x, titulo.y-elevacion, 0, 0, titulo.w, titulo.h, 1);
            tigrFill(pantalla_menu.i, cortina.x, cortina.y, cortina.w, 5, color_morado);
            // Dibujo de la transparencia del titulo.
            for(i=1; i<=2; i++) {
                // Dibuja la sombra del titulo.
                if(i==1) {
                    transparencia_titulo=sombra.i;
                    cuerpo_o_sombra=color_sombra;
                }
                // Dibuja el texto del titulo
                else {
                    transparencia_titulo=titulo.i;
                    cuerpo_o_sombra=tema_actual;
                }
                for(j=0; j<10; j++) {
                    // La letra P.
                    tigrLine(transparencia_titulo, 0+j, 40, 10+j, 0, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 10, 0+j, 40-j, 0+j, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 40-j, 0, 33-j, 30, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 33, 30-j, 10, 30-j, cuerpo_o_sombra);
                    // La letra I.
                    tigrLine(transparencia_titulo, 40+j, 40, 50+j, 0, cuerpo_o_sombra);
                    // La letra G.
                    tigrLine(transparencia_titulo, 60+j, 40, 70+j, 0, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 60+j, 40-j, 90, 40-j, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 90-j, 40, 95-j, 20, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 70, 0+j, 95, 0+j, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 100-j, 0, 97-j, 10, cuerpo_o_sombra);
                    // La letra K.
                    tigrLine(transparencia_titulo, 110+j, 40, 120+j, 0, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 123+j, 20, 130+j, 40, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 123+j, 20, 140+j, 0, cuerpo_o_sombra);
                    // La letra I.
                    tigrLine(transparencia_titulo, 150+j, 40, 160+j, 0, cuerpo_o_sombra);
                    // La letra N.
                    tigrLine(transparencia_titulo, 170+j, 40, 180+j, 0, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 180+j, 0, 190+j, 40, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 190+j, 40, 200+j, 0, cuerpo_o_sombra);
                    // La letra G.
                    tigrLine(transparencia_titulo, 210+j, 40, 220+j, 0, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 210+j, 40-j, 240, 40-j, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 240-j, 40, 245-j, 20, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 220, 0+j, 245, 0+j, cuerpo_o_sombra);
                    tigrLine(transparencia_titulo, 250-j, 0, 247-j, 10, cuerpo_o_sombra);
                }
            }
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, plantilla.w, plantilla.h, color_azul);
            // Maco plantilla.
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, plantilla.w, 3, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x+plantilla.w-3, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y+plantilla.h-3, plantilla.w, 3, color_morado);
            // Botones.
            tigrFill(pantalla_menu.i, boton_jugar.x, boton_jugar.y, boton_jugar.w, boton_jugar.h, presion_boton_jugar);
            tigrFill(pantalla_menu.i, boton_niveles.x, boton_niveles.y, boton_niveles.w, boton_niveles.h, presion_boton_niveles);
            tigrFill(pantalla_menu.i, boton_puntuacion.x, boton_puntuacion.y, boton_puntuacion.w, boton_puntuacion.h, presion_boton_puntuacion);
            // Marco boton Jugar.
            tigrFill(pantalla_menu.i, boton_jugar.x, boton_jugar.y, boton_jugar.w, 2, tema_boton_jugar);
            tigrFill(pantalla_menu.i, boton_jugar.x, boton_jugar.y, 2, boton_jugar.h, tema_boton_jugar);
            tigrFill(pantalla_menu.i, boton_jugar.x+boton_jugar.w-2, boton_jugar.y, 2, boton_jugar.h, tema_boton_jugar);
            tigrFill(pantalla_menu.i, boton_jugar.x, boton_jugar.y+boton_jugar.h-2, boton_jugar.w, 2, tema_boton_jugar);
            // Marco boton Niveles.
            tigrFill(pantalla_menu.i, boton_niveles.x, boton_niveles.y, boton_niveles.w, 2, tema_boton_niveles);
            tigrFill(pantalla_menu.i, boton_niveles.x, boton_niveles.y, 2, boton_niveles.h, tema_boton_niveles);
            tigrFill(pantalla_menu.i, boton_niveles.x+boton_niveles.w-2, boton_niveles.y, 2, boton_niveles.h, tema_boton_niveles);
            tigrFill(pantalla_menu.i, boton_niveles.x, boton_niveles.y+boton_niveles.h-2, boton_niveles.w, 2, tema_boton_niveles);
            // Marco boton Puntuacion.
            tigrFill(pantalla_menu.i, boton_puntuacion.x, boton_puntuacion.y, boton_puntuacion.w, 2, tema_boton_puntuacion);
            tigrFill(pantalla_menu.i, boton_puntuacion.x, boton_puntuacion.y, 2, boton_puntuacion.h, tema_boton_puntuacion);
            tigrFill(pantalla_menu.i, boton_puntuacion.x+boton_puntuacion.w-2, boton_puntuacion.y, 2, boton_puntuacion.h, tema_boton_puntuacion);
            tigrFill(pantalla_menu.i, boton_puntuacion.x, boton_puntuacion.y+boton_puntuacion.h-2, boton_puntuacion.w, 2, tema_boton_puntuacion);
            // Textos de los botones.
            if(aparicion_botones>=2) {
                tigrPrint(pantalla_menu.i, tfont, 7.35*ancho_ventana/16 , 6.85*alto_ventana/16, tema_boton_jugar, "JUGAR", 0);
            }
            if(aparicion_botones>=3) {
                tigrPrint(pantalla_menu.i, tfont, 7*ancho_ventana/16 , 9.85*alto_ventana/16, tema_boton_niveles, "NIVELES", 0);
            }
            if(opening==false) {
                tigrPrint(pantalla_menu.i, tfont, 6.6*ancho_ventana/16 , 12.85*alto_ventana/16, tema_boton_puntuacion, "PUNTUACIÓN", 0);
            }
            // Texto de los creditos.
            tigrPrint(pantalla_menu.i, tfont, 5, alto_ventana-15, tigrRGB(225, 225, 225), "Hecho por: Alexander Veldemar, Danil Launov y Andres Maciá.", 0);
            // Pinta cursor personalizado.
            tigrBlitAlpha(pantalla_menu.i, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1);
            }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return seleccion;
}
int poner_nombre(Tigr *pantalla, int ancho_ventana, int alto_ventana) {
    struct coordenadas pantalla_nombre;
    pantalla_nombre.x=0, pantalla_nombre.y=0, pantalla_nombre.w=ancho_ventana, pantalla_nombre.h=alto_ventana;
    pantalla_nombre.i=tigrBitmap(pantalla_nombre.w, pantalla_nombre.h);
    int elevacion_cierre=0;
    // Establecimiento de las coordenadas y las dimensiones de la pantala de cambiar nombre.
    int tecla=0, i=0, posicion_escritura=0;
    char nombre[10]="          ";
    // Creacion de los diferentes controladores de tiempo;
    float tiempo_inicial=0, tiempo_final=0, tiempo_total=0;
    // Creacion de los controladores de velociadad
    float velocidad_ending=1000;
    bool ending=false, cierre=false;


    while(!tigrClosed(pantalla) && (!tigrKeyHeld(pantalla, TK_ESCAPE)) && (!cierre)) {
        // Calculo del tiempo que tarda el ordenador en ejecutar un ciclo del programa.
        tiempo_final=tigrTime();
        tiempo_total=(tiempo_final-tiempo_inicial);
        tiempo_inicial=tigrTime();
        // Lee la tecla que se esta pulsando.
        tecla=tigrReadChar(pantalla);
        // Borra la anterior letra;
        if(tecla==13) {
            ending=true;
        }
        else if((tecla==8) && (posicion_escritura>0) && (!ending)) {
            posicion_escritura--;
            nombre[posicion_escritura]=' ';
        }
        // Lee la tecla que se quiere introducir.
        else if(tecla!=0 && posicion_escritura<10 && (tecla!=8) && (tecla!=13) && (!ending)) {
            nombre[posicion_escritura]=(char)tecla;
            posicion_escritura++;
        }
        if(ending) {
            elevacion_cierre=velocidad_ending*tiempo_total;
            pantalla_nombre.y-=elevacion_cierre;
        }
        // Comprobacion de si el menu se va ha cerrar.
        if(pantalla_nombre.y<=-pantalla_nombre.h) {
            cierre=true;
        }
        // Limpia el fondo de la pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        // Integracion de la pantalla de cambiar nombre a la pantalla del programa y elevacion de cierre.
        tigrBlitAlpha(pantalla, pantalla_nombre.i, pantalla_nombre.x, pantalla_nombre.y, 0, 0, pantalla_nombre.w, pantalla_nombre.h, 1);
        if(!ending) {
            // Imprime el texto por pantalla.
            tigrPrint(pantalla_nombre.i, tfont, ancho_ventana/6, alto_ventana/6, tigrRGB(252, 252, 252), "Introduce tu nombre:", 0);
            tigrPrint(pantalla_nombre.i, tfont, ancho_ventana/6+140, alto_ventana/6, tigrRGB(117, 227, 20),"%s", nombre);
        }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return 0;
}
int juego() {

    return 0;
}
int selector_de_niveles() {
 
}
int main() {
    int caso;
    int ancho_ventana=400, alto_ventana=400; // Establecimiento de las dimensiones de la ventana del juego.
    Tigr *pantalla;


    printf("Se esta abriendo la ventana del juego...");
    // Abre la ventana del juego.
    pantalla=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", TIGR_NOCURSOR | TIGR_2X);
    // Abre el menu.
    caso=menu(pantalla, ancho_ventana, alto_ventana);
        switch(caso) {
            case 0: caso=menu(pantalla, ancho_ventana, alto_ventana);
                    break;
            case 1: caso=poner_nombre(pantalla, ancho_ventana, alto_ventana);
                    break;
            case 2: 
                    break;
            case 3: 
                    break;
        }
    // Cierre de la ventana del juego.
    tigrFree(pantalla);
    printf("Juego cerrado.\n");
    return 0;
}
