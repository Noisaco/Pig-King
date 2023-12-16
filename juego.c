#include<stdio.h> // Se copila con gcc ./tigr/tigr.c juego.c -o juego -I./tigr -lGLU -lGL -lX11 -lm
#include <math.h>
#include<stdbool.h>
#include"./tigr/tigr.h"

typedef struct { // Establecimiento del registro de imagen, coordenadas y dimensiones de los diferentes elementos del juego.
    Tigr *i;
    int x;
    int y;
    int w;
    int h;
} TCoordenadas;
typedef TCoordenadas TLista_coordenadas[15];
typedef struct {
    TLista_coordenadas escaleras;
    TLista_coordenadas plataformas;
} TNiveles;
float Calcular_Tiempo() {
    float tiempo_inicial=0, tiempo_final=0, tiempo_total=0;
    tiempo_final=tigrTime();
    tiempo_total=(tiempo_final-tiempo_inicial);
    tiempo_inicial=tigrTime();
    return tiempo_total;
}
// Modulo que muestra el menu.
int menu(Tigr *pantalla, int ancho_ventana, int alto_ventana, float velocidad_ending) {
    TCoordenadas cursor, cortina, plantilla, titulo, sombra, boton_jugar, boton_niveles, boton_puntuacion, pantalla_menu;
    
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

    int aparicion_botones=1, seleccion=-1, click=0, last_click=0, botes=0, elevacion=0, elevacion_cierre=0, i=0, j=0;
    // Creacion de los diferentes controladores de tiempo.
    float tiempo_total, tiempo_animacion, tiempo_parpadeo, tiempo_botones;
    float resguardo=0,velocidad_titulo=0, velocidad_plantilla=250, aceleracion=1000;
    bool plantilla_terminada, cierre, permitido, opening, ending;
    TPixel color_morado=tigrRGB(160, 0, 100), color_azul=tigrRGB(69, 0, 98), cuerpo_o_sombra, tema_actual, tema1=tigrRGB(40, 131, 250), tema2=tigrRGB(252, 221, 78), color_sombra=tigrRGB(26, 6, 92);
    // Creacion del color de los botones.
    TPixel presion_boton_jugar, presion_boton_niveles, presion_boton_puntuacion, si_presionado=tigrRGB(70,83,199),  no_presionado=tigrRGB(116, 130, 252), color_seleccionado=tigrRGB(9, 176, 70);
    // Creacion del tema de los botones.
    TPixel tema_boton_jugar, tema_boton_niveles, tema_boton_puntuacion;
    Tigr *transparencia_titulo;
    // Asignacion del tema principal inicial.
    tema_actual=tema1;
    // Asignacion de los tiempos.
    tiempo_total=0, tiempo_animacion=0, tiempo_parpadeo=0, tiempo_botones=0;
    // Asignacion de los booleanos.
    plantilla_terminada=false, cierre=false, permitido=false, opening=true, ending=false;
    
    
    while((!cierre) && !tigrClosed(pantalla)) {
        // Calculo del tiempo que tarda el ordenador en ejecutar un ciclo del programa.
        tiempo_total=Calcular_Tiempo();
        tiempo_animacion+=tiempo_total;
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
        if((!opening) && (!ending) && (cursor.x>=boton_jugar.x) && (cursor.y>=boton_jugar.y) && (cursor.x<=boton_jugar.x+boton_jugar.w) && (cursor.y<=boton_jugar.y+boton_jugar.h)) {
            seleccion=1;
        }
        else if((!opening) && (!ending) && (cursor.x>=boton_niveles.x) && (cursor.y>=boton_niveles.y) && (cursor.x<=boton_niveles.x+boton_niveles.w) && (cursor.y<=boton_niveles.y+boton_niveles.h)) {
            seleccion=2;
        }
        else if((!opening) && (!ending) && (cursor.x>=boton_puntuacion.x) && (cursor.y>=boton_puntuacion.y) && (cursor.x<=boton_puntuacion.x+boton_puntuacion.w) && (cursor.y<=boton_puntuacion.y+boton_puntuacion.h)) {
            seleccion=3;
        }
        else if((!ending)) {
            seleccion=-1;
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
            case -1: break;
        }
        last_click=click;
        // Comprobacion de si el menu se va ha cerrar.
        if(pantalla_menu.y<=-pantalla_menu.h) {
            cierre=true;
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
        // Solucion errores.
        if(tigrKeyHeld(pantalla, TK_ESCAPE)) {
            seleccion=-1;
            ending=true;
        }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return seleccion;
}
int poner_nombre(Tigr *pantalla, int ancho_ventana, int alto_ventana, float velocidad_ending) {
    TCoordenadas pantalla_nombre;
    // Asignacion de las coordenadas y las dimensiones de la pantalla de cambiar nombre.
    pantalla_nombre.x=0, pantalla_nombre.y=0, pantalla_nombre.w=ancho_ventana, pantalla_nombre.h=alto_ventana;
    pantalla_nombre.i=tigrBitmap(pantalla_nombre.w, pantalla_nombre.h);
    // Creacion de la variable que guarda el nombre del jugador.
    char nombre[11];
    // Creacion de las diversas variables de tipo entero.
    int elevacion_cierre, tecla, i, posicion_escritura, caso;
    // Creacion de los diferentes controladores de tiempo.
    float tiempo_total;
    // Asignacion de las diferentes variables de tipo entero.
    elevacion_cierre=0, tecla=0, posicion_escritura=0, caso=-1;
    // Asignacion del nombre inicial del jugador.
    for(i=0; i<10; i++) {
        nombre[i]=' ';
    }
    // Creacion de los diferentes booleanos.
    bool ending, cierre;
    // Asignacion de los diferentes controladores de tiempo.
    tiempo_total=0;
    // Asignacion de los booleanos.
    ending=false, cierre=false;


    while((!tigrClosed(pantalla)) && (!cierre)) {
        // Calculo del tiempo que tarda el ordenador en ejecutar un ciclo del programa.
        tiempo_total=Calcular_Tiempo();
        // Lee la tecla que se esta pulsando.
        tecla=tigrReadChar(pantalla);
        // Confirmar nombre.
        if((tecla==13) && (!ending)) {
            ending=true;
            caso=2;
        }
        // Borra la anterior letra.
        else if((tecla==8) && (posicion_escritura>0) && (!ending)) {
            posicion_escritura--;
            nombre[posicion_escritura]=' ';
        }
        // Lee la tecla que se quiere introducir.
        else if((tecla!=0) && (posicion_escritura<10) && (tecla!=8) && (tecla!=13) && (tecla!=27) && (!ending)) {
            nombre[posicion_escritura]=(char)tecla;
            posicion_escritura++;
        }
        else if((tigrKeyHeld(pantalla, TK_ESCAPE)) && (!ending)) {
            ending=true;
            caso=0;
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
            // Limpia la pantalla de cambiar nombre.
            tigrClear(pantalla_nombre.i, tigrRGB(0, 0, 0));
            // Imprime el texto por pantalla.
            tigrPrint(pantalla_nombre.i, tfont, ancho_ventana/6, alto_ventana/6, tigrRGB(252, 252, 252), "Introduce tu nombre:", 0);
            tigrPrint(pantalla_nombre.i, tfont, ancho_ventana/6+140, alto_ventana/6, tigrRGB(117, 227, 20),"%s", nombre);
        }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}
void pintar_estructuras_nivel1(Tigr *pantalla_juego, int ancho_ventana, int alto_ventana,TPixel color_solido, TPixel color_escalera, TNiveles nivel1) {
    int i;
    //Pintar plataformas.
    for(i=0; i<6; i++) {    
        tigrLine(pantalla_juego, nivel1.plataformas[0].x, 45*alto_ventana/50+i, 47*ancho_ventana/50, 45*alto_ventana/50+i, color_solido);
        
    }
    tigrFill(pantalla_juego, 44*ancho_ventana/50, 45*alto_ventana/50, 3*ancho_ventana/50, 5*alto_ventana/50, tigrRGB(172, 7, 1));
    // Pintar lineas verticales de las escaleras.
    for(i=0; i<3; i++){
        tigrLine(pantalla_juego, nivel1.escaleras[0].x-i, nivel1.escaleras[0].y+nivel1.escaleras[0].h, nivel1.escaleras[0].x-i, nivel1.escaleras[0].y, color_escalera);
        tigrLine(pantalla_juego, nivel1.escaleras[0].x+nivel1.escaleras[0].w-i, nivel1.escaleras[0].y+nivel1.escaleras[0].h, nivel1.escaleras[0].x+nivel1.escaleras[0].w-i, 45*alto_ventana/50, color_escalera);
    }
    //Pintar lineas horizontales de las escaleras.
    for(i=0; i<5; i++){
        tigrLine(pantalla_juego, nivel1.escaleras[0].x, (50-i)*alto_ventana/50-i, 47*ancho_ventana/50, (50-i)*alto_ventana/50-i, color_escalera);    
    }
}
int juego(Tigr *pantalla, int ancho_ventana, int alto_ventana, float velocidad_ending) {
    TCoordenadas pantalla_juego, personaje;
    TNiveles nivel1;
    // Coordenadas y dimensiones de los elemetos del nivel 1.
    nivel1.escaleras[0].x=44*ancho_ventana/50, nivel1.escaleras[0].y=45*alto_ventana/50,nivel1.escaleras[0].w=3*ancho_ventana/50, nivel1.escaleras[0].h= 5*alto_ventana/50; 
    nivel1.plataformas[0].x=4*ancho_ventana/50;
    

    
    personaje.x=ancho_ventana/2, personaje.y=alto_ventana/2, personaje.w=18, personaje.h=32;
    personaje.i = tigrLoadImage("./imagenes/personaje.png");
    int caso, i, ultima_posicion_y, ultima_posicion_x;
    float tiempo_total=0, resguardo_gravedad, gravedad, velocidad_caida, resguardo_movimiento, cantidad_movimiento;
    char ultima_direccion;
    bool cierre, pausa, caida, ending;
    pantalla_juego.x=0, pantalla_juego.y=0, pantalla_juego.w=ancho_ventana, pantalla_juego.h=alto_ventana;
    pantalla_juego.i=tigrBitmap(pantalla_juego.w, pantalla_juego.h);
    caso=-1;
    resguardo_movimiento=personaje.x, cantidad_movimiento=0, resguardo_gravedad=0, gravedad=900, velocidad_caida=0;
    ultima_direccion='D';
    // Asignacion de los booleanos.
    cierre=false, pausa=false, caida=true, ending=false;
    // Creacion del color de las estructuras.
    TPixel color_solido=tigrRGB(69, 33, 217), color_escalera=tigrRGB(30,222,45);
    // Creacion de las variables para saber si estamos encima de un solido y si hay rampas.
    TPixel color_mas_abajo, color_debajo, color_medio, color_arriba;

    while((!tigrClosed(pantalla)) && (!cierre)) {
        tiempo_total=Calcular_Tiempo();
        if(!pausa) {
            // Detecta que direccion que se quiere mover el jugador.
            if((tigrKeyHeld(pantalla, 'A')) && (personaje.x>0)) {
                cantidad_movimiento=100*tiempo_total;
                cantidad_movimiento=(-cantidad_movimiento);
                ultima_direccion='I';
            }
            else if((tigrKeyHeld(pantalla, 'D')) && ((personaje.x+personaje.w)<ancho_ventana)){
                cantidad_movimiento=100*tiempo_total;
                ultima_direccion='D';
            }
            else {
                cantidad_movimiento=0;
            }
            // Calculo de la posicion horizontal del jugador.
            resguardo_movimiento+=cantidad_movimiento;
            // Que el jugador no atraviese la ventana por los lados.
            if(personaje.x<0) {
                resguardo_movimiento=0;
            }
            else if((personaje.x+personaje.w)>ancho_ventana) {
                resguardo_movimiento=ancho_ventana-personaje.w;
            }
            ultima_posicion_x=personaje.x;
            personaje.x=resguardo_movimiento;
            // Que el jugador pueda bajar rampas derecha.
            if(tigrKeyHeld(pantalla, 'D') && (!caida)) {
                for(i=ultima_posicion_x; i<=personaje.x; i++) {
                    // Saber si hay solido a la derecha.
                    color_mas_abajo=tigrGet(pantalla, i+personaje.w/2, personaje.y+personaje.h+2);
                    // Saber si estas atravesando solido.
                    color_debajo=tigrGet(pantalla, i+personaje.w/2, personaje.y+personaje.h+1);
                    // Colision plataforma.
                    if((color_mas_abajo.r==color_solido.r) && (color_mas_abajo.g==color_solido.g) && (color_mas_abajo.b==color_solido.b) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                        personaje.y+=1;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Que el jugador pueda bajar rampas izquierda.
            if(tigrKeyHeld(pantalla, 'A') && (!caida)) {
                for(i=ultima_posicion_x; i>=personaje.x; i--) {
                    // Saber si hay solido a la derecha.
                    color_mas_abajo=tigrGet(pantalla, i+personaje.w/2, personaje.y+personaje.h+2);
                    // Saber si estas atravesando solido.
                    color_debajo=tigrGet(pantalla, i+personaje.w/2, personaje.y+personaje.h+1);
                    // Colision plataforma.
                    if((color_mas_abajo.r==color_solido.r) && (color_mas_abajo.g==color_solido.g) && (color_mas_abajo.b==color_solido.b) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                        personaje.y+=1;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Saber si estas en el aire o no.
            color_debajo=tigrGet(pantalla, personaje.x+personaje.w/2, personaje.y+personaje.h+1);
            if((!caida) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                caida=true;
            }
            // Que el jugador no atraviese la ventana por abajo.
            if((personaje.y+personaje.h)>=alto_ventana) {
                personaje.y=alto_ventana-personaje.h;
                caida=false;
                velocidad_caida=0;
            }
            // Que el jugador pueda subir rampas derecha.
            if((!caida) && tigrKeyHeld(pantalla, 'D')) {
                for(i=ultima_posicion_x; i<personaje.x; i++) {
                    // Saber si hay solido a la derecha.
                    color_medio=tigrGet(pantalla, i+personaje.w/2+1, personaje.y+personaje.h);
                    // Saber si estas atravesando solido.
                    color_arriba=tigrGet(pantalla, i+personaje.w/2+1, personaje.y+personaje.h-1);
                    // Colision plataforma.
                    if((color_medio.r==color_solido.r) && (color_medio.g==color_solido.g) && (color_medio.b==color_solido.b) && (color_arriba.r!=color_solido.r) && (color_arriba.g!=color_solido.g) && (color_arriba.b!=color_solido.b)) {
                        personaje.y-=1;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Que el jugador pueda subir rampas izquierda.
            if((!caida) && tigrKeyHeld(pantalla, 'A')) {
                for(i=ultima_posicion_x; i>personaje.x; i--) {
                    // Saber si hay solido a la derecha.
                    color_medio=tigrGet(pantalla, i+personaje.w/2-1, personaje.y+personaje.h);
                    // Saber si estas atravesando solido.
                    color_arriba=tigrGet(pantalla, i+personaje.w/2-1, personaje.y+personaje.h-1);
                    // Colision plataforma.
                    if((color_medio.r==color_solido.r) && (color_medio.g==color_solido.g) && (color_medio.b==color_solido.b) && (color_arriba.r!=color_solido.r) && (color_arriba.g!=color_solido.g) && (color_arriba.b!=color_solido.b)) {
                        personaje.y-=1;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Detecta si el jugador quiere saltar.
            if((tigrKeyDown(pantalla, TK_SPACE)) && (!caida)) {
                personaje.y--;
                velocidad_caida=-280;
                caida=true;
            }
            // Calculo de la gravedad.
            if(caida) {
                velocidad_caida+=gravedad*tiempo_total;
                resguardo_gravedad+=velocidad_caida*tiempo_total;
                ultima_posicion_y=personaje.y;
                personaje.y=resguardo_gravedad;
            }
            // Comprobar que en todo el recorrido no se ha chocado con una estructura.
            if((velocidad_caida>=0) && (caida)) {
                for(i=ultima_posicion_y; i<=personaje.y; i++) {
                    // Saber si hay solido debajo.
                    color_debajo=tigrGet(pantalla, personaje.x+personaje.w/2, i+personaje.h+1);
                    // Saber si estas atravesando solido.
                    color_medio=tigrGet(pantalla, personaje.x+personaje.w/2, i+personaje.h);
                    // Colision plataforma.
                    if((color_debajo.r==color_solido.r) && (color_debajo.g==color_solido.g) && (color_debajo.b==color_solido.b) && (color_medio.r!=color_solido.r) && (color_medio.g!=color_solido.g) && (color_medio.b!=color_solido.b)) {
                        caida=false;
                        velocidad_caida=0;
                        personaje.y=i;
                        break;
                    }
                }
            }
            // Saber si esta en las escaleras.
            if(((personaje.x+(personaje.w/2))>nivel1.escaleras[0].x) && ((personaje.x+(personaje.w/2))<(nivel1.escaleras[0].x+nivel1.escaleras[0].w))) {

            }
            // Subir escaleras.
            

            pintar_estructuras_nivel1(pantalla_juego.i, ancho_ventana, alto_ventana, color_solido, color_escalera, nivel1);
            // Asignacion de que PNG pintar segun el estado del jugador.
            if(tigrKeyHeld(pantalla, 'A')) {
                personaje.i=tigrLoadImage("./imagenes/caminar_izquierda.png");
            }
            else if(tigrKeyHeld(pantalla, 'D')) {
                personaje.i=tigrLoadImage("./imagenes/caminar_derecha.png");
            }
            else if(ultima_direccion=='D') {
                personaje.i=tigrLoadImage("./imagenes/estatico_derecha.png");
            }
            else if(ultima_direccion=='I') {
                personaje.i=tigrLoadImage("./imagenes/estatico_izquierda.png");
            }
            // Pinta el jugador por pantalla.
            tigrBlitAlpha(pantalla_juego.i, personaje.i, personaje.x, personaje.y, 0, 0, personaje.w, personaje.h, 1);
        }
        // Limpia el fondo de pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        // Integracion de la pantalla de cambiar nombre a la pantalla del programa y elevacion de cierre.
        tigrBlitAlpha(pantalla, pantalla_juego.i, pantalla_juego.x, pantalla_juego.y, 0, 0, pantalla_juego.w, pantalla_juego.h, 1);
        if(!ending) {

            // Limpia la pantalla de juego.
            tigrClear(pantalla_juego.i, tigrRGB(0, 0, 0));
        }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}
int selector_de_niveles() {

}
int main() {
    // Establecimiento de la primera pantalla en abrirse.
    int caso=2;
    // Establecimiento de las dimensiones de la ventana del juego.
    int ancho_ventana=400, alto_ventana=450;
    // Establecimieto de la velocidad de cierre de las diferentes pantallas del juego.
    float velocidad_cierre=800;
    Tigr *pantalla;


    printf("Se esta abriendo la ventana del juego...");
    // Abre la ventana del juego.
    pantalla=tigrWindow(ancho_ventana, alto_ventana, "Pig Kong", TIGR_NOCURSOR | TIGR_2X);
    printf("Ventana del juego abierta.\n");
    // Inserta el efecto de television de tubos.
    tigrSetPostFX(pantalla, 1, 1, 1, 2.0f);
    // Abre el menu.
    do {
        switch(caso) {
            case 0: caso=menu(pantalla, ancho_ventana, alto_ventana, velocidad_cierre);
                    break;
            case 1: caso=poner_nombre(pantalla, ancho_ventana, alto_ventana, velocidad_cierre);
                    break;
            case 2: caso=juego(pantalla, ancho_ventana, alto_ventana, velocidad_cierre);
                    break;
            case 3: 
                    break;
        }
    } while(caso!=-1);
    // Cierre de la ventana del juego.
    tigrFree(pantalla);
    printf("Juego cerrado.\n");
    return 0;
}