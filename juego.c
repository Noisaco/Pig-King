#include<stdio.h> // Se copila con gcc ./tigr/tigr.c juego.c -o juego -I./tigr -lGLU -lGL -lX11
#include<stdbool.h>
#include"./tigr/tigr.h"
#define FICHERO_TIEMPOS "tiempos.texto"

typedef struct { // Establecimiento del registro de imagen, coordenadas y dimensiones de los diferentes elementos del juego.
    Tigr *i;
    int x;
    int y;
    int w;
    int h;
} TCoordenadas;
typedef TCoordenadas TLista_coordenadas[15];
typedef struct {
    TCoordenadas coordenadas;
    char direccion;
    float cantidad_movimiento;
    float resguardo;
    bool caida;
    float velocidad_caida;
    float resguardo_gravedad;
    int ultima_posicion_y;
    int ultima_posicion_x;
    bool esta_escalera;
} TBarriles;
typedef TBarriles TLista_barriles[15];
typedef struct {
    TLista_coordenadas escaleras;
    TLista_coordenadas plataformas;
    TLista_barriles barriles;
    TCoordenadas tarta;
    TCoordenadas oveja;
} TNiveles;
float Calcular_Tiempo() {
    float tiempo_inicial=0, tiempo_final=0, tiempo_total=0;
    tiempo_final=tigrTime();
    tiempo_total=(tiempo_final-tiempo_inicial);
    tiempo_inicial=tigrTime();
    return tiempo_total;
}
// Modulo que pinta el titulo y la sombra.
void pintar_titulo_y_sombra(Tigr *transparencia_titulo, TPixel cuerpo_o_sombra) {
    int i;
    for(i=0; i<10; i++) {
        // La letra P.
        tigrLine(transparencia_titulo, 0+i, 40, 10+i, 0, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 10, 0+i, 40-i, 0+i, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 40-i, 0, 33-i, 30, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 33, 30-i, 10, 30-i, cuerpo_o_sombra);
        // La letra I.
        tigrLine(transparencia_titulo, 40+i, 40, 50+i, 0, cuerpo_o_sombra);
        // La letra G.
        tigrLine(transparencia_titulo, 60+i, 40, 70+i, 0, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 60+i, 40-i, 90, 40-i, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 90-i, 40, 95-i, 20, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 70, 0+i, 95, 0+i, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 100-i, 0, 97-i, 10, cuerpo_o_sombra);
        // La letra K.
        tigrLine(transparencia_titulo, 110+i, 40, 120+i, 0, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 123+i, 20, 130+i, 40, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 123+i, 20, 140+i, 0, cuerpo_o_sombra);
        // La letra I.
        tigrLine(transparencia_titulo, 150+i, 40, 160+i, 0, cuerpo_o_sombra);
        // La letra N.
        tigrLine(transparencia_titulo, 170+i, 40, 180+i, 0, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 180+i, 0, 190+i, 40, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 190+i, 40, 200+i, 0, cuerpo_o_sombra);
        // La letra G.
        tigrLine(transparencia_titulo, 210+i, 40, 220+i, 0, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 210+i, 40-i, 240, 40-i, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 240-i, 40, 245-i, 20, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 220, 0+i, 245, 0+i, cuerpo_o_sombra);
        tigrLine(transparencia_titulo, 250-i, 0, 247-i, 10, cuerpo_o_sombra);
    }
}
// Modulo que muestra el menu.
int menu(Tigr *pantalla, int ancho_ventana, int alto_ventana, float velocidad_ending, int *nivel) {
    TCoordenadas cursor, cortina, plantilla, titulo, sombra, boton_jugar, boton_niveles, boton_creditos, pantalla_menu;
   
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
    boton_creditos.x=ancho_ventana, boton_creditos.y=alto_ventana, boton_creditos.w=0, boton_creditos.h=2*alto_ventana/16;

    int aparicion_botones=1, seleccion=-1, click=0, last_click=0, botes=0, elevacion=0, elevacion_cierre=0, i=0, j=0;
    // Creacion de los diferentes controladores de tiempo.
    float tiempo_total, tiempo_animacion, tiempo_parpadeo, tiempo_botones;
    float resguardo=0,velocidad_titulo=0, velocidad_plantilla=250, aceleracion=1000;
    bool plantilla_terminada, cierre, permitido, opening, ending;
    TPixel color_morado=tigrRGB(160, 0, 100), color_azul=tigrRGB(69, 0, 98), cuerpo_o_sombra, tema_actual, tema1=tigrRGB(40, 131, 250), tema2=tigrRGB(252, 221, 78), color_sombra=tigrRGB(26, 6, 92);
    // Creacion del color de los botones.
    TPixel presion_boton_jugar, presion_boton_niveles, presion_boton_creditos, si_presionado=tigrRGB(70,83,199),  no_presionado=tigrRGB(116, 130, 252), color_seleccionado=tigrRGB(9, 176, 70);
    // Creacion del tema de los botones.
    TPixel tema_boton_jugar, tema_boton_niveles, tema_boton_creditos;
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
                // Aparicion del boton creditos.
                case 3: boton_creditos.w=9*ancho_ventana/16;
                        boton_creditos.h=3*alto_ventana/16;
                        boton_creditos.x=3.5*ancho_ventana/16;
                        boton_creditos.y=11.5*alto_ventana/16;
                        if(tiempo_botones>0.2) {
                            boton_creditos.w=8*ancho_ventana/16;
                            boton_creditos.h=2*alto_ventana/16;
                            boton_creditos.x=4*ancho_ventana/16;
                            boton_creditos.y=12*alto_ventana/16;
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
            seleccion=4;
        }
        else if((!opening) && (!ending) && (cursor.x>=boton_creditos.x) && (cursor.y>=boton_creditos.y) && (cursor.x<=boton_creditos.x+boton_creditos.w) && (cursor.y<=boton_creditos.y+boton_creditos.h)) {
            seleccion=5;
        }
        else if((!ending)) {
            seleccion=-1;
        }
        presion_boton_jugar=no_presionado;
        presion_boton_niveles=no_presionado;
        presion_boton_creditos=no_presionado;
        tema_boton_jugar=tema_actual;
        tema_boton_niveles=tema_actual;
        tema_boton_creditos=tema_actual;
        switch(seleccion) {
            case 1: presion_boton_jugar=si_presionado;
                    tema_boton_jugar=color_seleccionado;
                    if((click==1) && (last_click==0)) {
                        *nivel=1;
                        ending=true;
                    }
                    break;
            case 4: presion_boton_niveles=si_presionado;
                    tema_boton_niveles=color_seleccionado;
                    if((click==1) && (last_click==0)) {
                        ending=true;
                    }
                    break;
            case 5:  presion_boton_creditos=si_presionado;
                    tema_boton_creditos=color_seleccionado;
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
                // Pintar titulo y sombra.
                pintar_titulo_y_sombra(transparencia_titulo, cuerpo_o_sombra);
            }
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, plantilla.w, plantilla.h, color_azul);
            // Marco plantilla.
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, plantilla.w, 3, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x+plantilla.w-3, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_menu.i, plantilla.x, plantilla.y+plantilla.h-3, plantilla.w, 3, color_morado);
            // Botones.
            tigrFill(pantalla_menu.i, boton_jugar.x, boton_jugar.y, boton_jugar.w, boton_jugar.h, presion_boton_jugar);
            tigrFill(pantalla_menu.i, boton_niveles.x, boton_niveles.y, boton_niveles.w, boton_niveles.h, presion_boton_niveles);
            tigrFill(pantalla_menu.i, boton_creditos.x, boton_creditos.y, boton_creditos.w, boton_creditos.h, presion_boton_creditos);
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
            // Marco boton creditos.
            tigrFill(pantalla_menu.i, boton_creditos.x, boton_creditos.y, boton_creditos.w, 2, tema_boton_creditos);
            tigrFill(pantalla_menu.i, boton_creditos.x, boton_creditos.y, 2, boton_creditos.h, tema_boton_creditos);
            tigrFill(pantalla_menu.i, boton_creditos.x+boton_creditos.w-2, boton_creditos.y, 2, boton_creditos.h, tema_boton_creditos);
            tigrFill(pantalla_menu.i, boton_creditos.x, boton_creditos.y+boton_creditos.h-2, boton_creditos.w, 2, tema_boton_creditos);
            // Textos de los botones.
            if(aparicion_botones>=2) {
                tigrPrint(pantalla_menu.i, tfont, 7.35*ancho_ventana/16 , 6.85*alto_ventana/16, tema_boton_jugar, "JUGAR", 0);
            }
            if(aparicion_botones>=3) {
                tigrPrint(pantalla_menu.i, tfont, 7*ancho_ventana/16 , 9.85*alto_ventana/16, tema_boton_niveles, "NIVELES", 0);
            }
            if(opening==false) {
                tigrPrint(pantalla_menu.i, tfont, 6.6*ancho_ventana/16 , 12.85*alto_ventana/16, tema_boton_creditos, "CREDITOS", 0);
            }
        
            // Pinta cursor personalizado.
            tigrBlitAlpha(pantalla_menu.i, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1);
        }
        // Solucion errores.
        if(tigrKeyHeld(pantalla, TK_ESCAPE)) {
            seleccion=-1;
            ending=true;
        }
        // Integracion de la pantalla del menu a la pantalla del programa y elevacion de cierre.
        tigrBlitAlpha(pantalla, pantalla_menu.i, pantalla_menu.x, pantalla_menu.y, 0, 0, pantalla_menu.w, pantalla_menu.h, 1);
        // Actualiza la imagen
        tigrUpdate(pantalla);
    }
    return seleccion;
}
int poner_nombre(Tigr *pantalla, int ancho_ventana, int alto_ventana, char nombre[11],float velocidad_ending) {
    TCoordenadas pantalla_nombre, boton_escape;
    TPixel color_escape=tigrRGB(160, 0, 100), color_texto=tigrRGB(255,255,255);
    // Asignacion de las coordenadas y las dimensiones de la pantalla de cambiar nombre.
    pantalla_nombre.x=0, pantalla_nombre.y=0, pantalla_nombre.w=ancho_ventana, pantalla_nombre.h=alto_ventana;
    //Asignacion de la coordenadas del boton escape.
    boton_escape.x=2*ancho_ventana/50, boton_escape.y=45*alto_ventana/50, boton_escape.w=ancho_ventana/8, boton_escape.h=alto_ventana/10;
    pantalla_nombre.i=tigrBitmap(pantalla_nombre.w, pantalla_nombre.h);
    // Creacion de la variable que guarda el nombre del jugador.
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
        // Vuelve al menu.
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
        //Rectangulo de ESCAPE marco ESC.
        tigrFill(pantalla_nombre.i, boton_escape.x, boton_escape.y, boton_escape.w, boton_escape.h, color_escape);
        tigrPrint(pantalla_nombre.i, tfont, 3*ancho_ventana/50+7, 46*alto_ventana/50, color_texto, "ESC");
        tigrPrint(pantalla_nombre.i, tfont, 2*ancho_ventana/50+4, 48*alto_ventana/50, color_texto, "VOLVER");
        tigrFill(pantalla_nombre.i, boton_escape.x, boton_escape.y, boton_escape.w, 2, color_texto);
        tigrFill(pantalla_nombre.i, boton_escape.x, boton_escape.y, 2, boton_escape.h, color_texto);
        tigrFill(pantalla_nombre.i, boton_escape.x+boton_escape.w-2, boton_escape.y, 2, boton_escape.h, color_texto);
        tigrFill(pantalla_nombre.i, boton_escape.x, boton_escape.y+boton_escape.h-2, boton_escape.w, 2, color_texto);
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}
void pintar_estructuras_nivel1(Tigr *pantalla_estructura, int ancho_ventana, int alto_ventana,TPixel color_solido, TPixel color_escalera, TPixel color_barra, TPixel color_solido2, TNiveles nivel1) {
    int i, j;
    //Pintar plataformas.
    for(i=0; i<6; i++) {
        //1ª plataforma.  
        tigrLine(pantalla_estructura, 4*ancho_ventana/50, 44*alto_ventana/50+i, 47*ancho_ventana/50, 44*alto_ventana/50+i, color_solido);
        //2ª plataforma
        tigrLine(pantalla_estructura, 4*ancho_ventana/50, 39*alto_ventana/50+i, 47*ancho_ventana/50, 38*alto_ventana/50+i, color_solido);
        //3ª plataforma
        tigrLine(pantalla_estructura, 36*ancho_ventana/50, 31*alto_ventana/50+i, 47*ancho_ventana/50, 32*alto_ventana/50+i, color_solido);
        //4ª plataforma
        tigrLine(pantalla_estructura, 20*ancho_ventana/50, 29*alto_ventana/50+i, 31*ancho_ventana/50, 30*alto_ventana/50+i, color_solido);
        //5ª plataforma
        tigrLine(pantalla_estructura, 4*ancho_ventana/50, 27*alto_ventana/50+i, 15*ancho_ventana/50, 28*alto_ventana/50+i, color_solido);
        //6ª plataforma
        tigrLine(pantalla_estructura, 13*ancho_ventana/50, 21*alto_ventana/50+i, 47*ancho_ventana/50, 20*alto_ventana/50+i, color_solido);
        //7ª plataforma
        tigrLine(pantalla_estructura, ancho_ventana/50, 15*alto_ventana/50+i, 47*ancho_ventana/50, 15*alto_ventana/50+i, color_solido);
        //8ª plataforma
        tigrLine(pantalla_estructura, 18*ancho_ventana/50, 10*alto_ventana/50+i, 31*ancho_ventana/50, 10*alto_ventana/50+i, color_solido);
        //Entre la pltaforma 3 y 4.
        tigrLine(pantalla_estructura, 36*ancho_ventana/50, 31*alto_ventana/50+i, 31*ancho_ventana/50-2, 30*alto_ventana/50+i, color_solido2);
        //Entre la 4 y 5.
        tigrLine(pantalla_estructura, 15*ancho_ventana/50, 28*alto_ventana/50+i, 20*ancho_ventana/50, 29*alto_ventana/50+i, color_solido2);
    }
    // Pintar lineas horizontales de las escaleras.
    for(i=1; i<50; i++) {
        if((i%5)==0) {
            for(j=0; j<9; j++) {
                //1ª escalera
                if(i<nivel1.escaleras[j].h) {
                    tigrLine(pantalla_estructura, nivel1.escaleras[j].x, nivel1.escaleras[j].y+nivel1.escaleras[j].h-i, nivel1.escaleras[j].x+nivel1.escaleras[j].w, nivel1.escaleras[j].y+nivel1.escaleras[j].h-i, color_barra);    
                }
            }
        }
    }
    // Pintar lineas verticales de las escaleras.
    for(i=0; i<9; i++) {
        for(j=0; j<3; j++) {
            tigrLine(pantalla_estructura, nivel1.escaleras[i].x-j, nivel1.escaleras[i].y+nivel1.escaleras[i].h, nivel1.escaleras[i].x-j, nivel1.escaleras[i].y, color_escalera);
            tigrLine(pantalla_estructura, nivel1.escaleras[i].x+nivel1.escaleras[i].w-j, nivel1.escaleras[i].y+nivel1.escaleras[i].h, nivel1.escaleras[i].x+nivel1.escaleras[i].w-j, nivel1.escaleras[i].y, color_escalera);
        }
       
    }
    // Pintar base del juego.
    for(i=0; i<1; i++) {
    tigrLine(pantalla_estructura, 0, alto_ventana-1-i, ancho_ventana, alto_ventana-1-i, color_solido);
    }
}
bool ver_escalera(TCoordenadas personaje, TLista_coordenadas escaleras) { // Modulo para saber si estas en una escalera.
    int i;
   
    for(i=0; i<9; i++) {
        if(((personaje.x+(personaje.w/2))>escaleras[i].x) && ((personaje.x+(personaje.w/2))<(escaleras[i].x+escaleras[i].w)) && ((personaje.y+(3*personaje.h/4))>escaleras[i].y) && ((personaje.y+(3*personaje.h/4))<(escaleras[i].y+escaleras[i].h))) {
            return true;
        }
    }
    return false;
}
// Modulo encargado de pintar los barriles.
void pintar_barriles(Tigr *pantalla_juego, int numero_barriles, TLista_barriles barriles) {
    int i;

    for(i=0; i<numero_barriles; i++) {
        tigrBlitAlpha(pantalla_juego, barriles[i].coordenadas.i, barriles[i].coordenadas.x, barriles[i].coordenadas.y, 0, 0, barriles[i].coordenadas.w, barriles[i].coordenadas.h, 1);
    }
}
void barra_juego(Tigr *pantalla_juego, int ancho_ventana, int alto_ventana, char nombre[11],float tiempo_juego, TPixel color_barrajuego, TPixel color_texto, TPixel color_barrajuego2, int nivel) {
    TCoordenadas marco;
    int i, minutos_juego=(int)(tiempo_juego/60), segundos_juego=(int)tiempo_juego%60;
   
    // Rectangulo de la barra de juego.
    marco.x=0, marco.y=0, marco.w=ancho_ventana, marco.h=36;
    tigrFill(pantalla_juego, marco.x, marco.y, marco.w, marco.h, color_barrajuego);
    
    if(nivel==2){
        tigrPrint(pantalla_juego, tfont, 15*ancho_ventana/50, 2*alto_ventana/53, color_texto, "NIVEL 2", 1);
    }
    else{
        tigrPrint(pantalla_juego, tfont, 15*ancho_ventana/50, 2*alto_ventana/53, color_texto, "NIVEL 1", 1);

    }

    tigrPrint(pantalla_juego, tfont, 2*ancho_ventana/50, 2*alto_ventana/53, color_texto, "%s", nombre);
    tigrPrint(pantalla_juego, tfont, 38*ancho_ventana/50, 2*alto_ventana/53, color_texto, "Tiempo:", 1);
    tigrPrint(pantalla_juego, tfont, 44*ancho_ventana/50, 2*alto_ventana/53, color_texto, "%02d:", minutos_juego);
    tigrPrint(pantalla_juego, tfont, 46*ancho_ventana/50+2, 2*alto_ventana/53, color_texto, "%02d", segundos_juego);
   
    // Pinta los bordes de la pantalla.
    for(i=0;i<2;i++){
        tigrLine(pantalla_juego, 0, 4*alto_ventana/50+i, ancho_ventana, 4*alto_ventana/50+i, color_barrajuego2);
        tigrLine(pantalla_juego, 0+i, 0, 0+i, 4*alto_ventana/50, color_barrajuego2);
        tigrLine(pantalla_juego, ancho_ventana-2+i, 0, ancho_ventana-2+i, 4*alto_ventana/50, color_barrajuego2);
        tigrLine(pantalla_juego, 0, 0+i, ancho_ventana, 0+i, color_barrajuego2);
    }
}
bool imprimir_game_over(Tigr *pantalla, int ancho_ventana, int alto_ventana, int *seleccion, int *caso) {
    TCoordenadas pantalla_gameover, boton_menu, boton_jugar;
    int i, j, tecla, ancho_sombra;
    TPixel presion_boton_menu, presion_boton_jugar, cuerpo_o_sombra, rojo, sombra, fondo, color_marco, color_botones, color_seleccionado;
    // Establecimiento de las coordenadas y las dimensiones de la pantalla Game Over.
    pantalla_gameover.x=ancho_ventana/8, pantalla_gameover.y=alto_ventana/10, pantalla_gameover.w=6*ancho_ventana/8, pantalla_gameover.h=8*alto_ventana/10; 
    pantalla_gameover.i=tigrBitmap(pantalla_gameover.w, pantalla_gameover.h);
    boton_jugar.x=pantalla_gameover.w/6, boton_jugar.y=3*pantalla_gameover.h/8, boton_jugar.w=4*pantalla_gameover.w/6, boton_jugar.h=pantalla_gameover.h/6;
    boton_menu.x=pantalla_gameover.w/6, boton_menu.y=5*pantalla_gameover.h/8, boton_menu.w=4*pantalla_gameover.w/6, boton_menu.h=pantalla_gameover.h/6;
    ancho_sombra=6;
    // Establecimiento del los colores.
    rojo=tigrRGB(222, 15, 15), sombra=tigrRGB(58, 0, 82),color_marco=tigrRGB(160, 0, 100), fondo=tigrRGB(69, 0, 98), color_botones=tigrRGB(101, 9, 117),color_seleccionado=tigrRGB(75, 20, 89);
    cuerpo_o_sombra=sombra;
    // Selector de opciones mediante teclado.
    tecla=tigrReadChar(pantalla);
    if((*seleccion==1) && (tigrKeyHeld(pantalla,'S'))) {
        *seleccion=0;
    }
    else if((*seleccion==0) && (tigrKeyHeld(pantalla,'W'))) {
        *seleccion=1;
    }
    switch(*seleccion) {
        // Volver al menu
        case 0: presion_boton_jugar=color_botones;
                presion_boton_menu=color_seleccionado;
                if(tecla==13) {
                    *caso=0;
                    return true;
                }
                break;
        // Volver a intentarlo.
        case 1: presion_boton_menu=color_botones;
                presion_boton_jugar=color_seleccionado;
                if(tecla==13) {
                    *caso=2;
                    return true;
                }
                break;
    }
    // Limpia la pantalla de game over.
    tigrClear(pantalla_gameover.i, fondo);
    for(i=0; i<2; i++) {
        for(j=0;j<10;j++){
            // Letra G.
            tigrLine(pantalla_gameover.i, 60+j+ancho_sombra, 50+(ancho_sombra/2), 70+j+ancho_sombra, 10+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 60+j+ancho_sombra, 50-j+(ancho_sombra/2), 90+ancho_sombra, 50-j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 90-j+ancho_sombra, 50+(ancho_sombra/2), 95-j+ancho_sombra, 30+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 70+ancho_sombra, 10+j+(ancho_sombra/2), 95+ancho_sombra, 10+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 100-j+ancho_sombra, 10+(ancho_sombra/2), 97-j+ancho_sombra, 20+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra A.
            tigrLine(pantalla_gameover.i, 100+j+ancho_sombra, 50+(ancho_sombra/2), 120+j+ancho_sombra, 10+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 120+j+ancho_sombra, 10+(ancho_sombra/2), 130+j+ancho_sombra, 50+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 120+ancho_sombra, 40+j+(ancho_sombra/2), 130+ancho_sombra, 40+j+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra M.
            tigrLine(pantalla_gameover.i, 150+j+ancho_sombra, 50+(ancho_sombra/2), 160+j+ancho_sombra, 10+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 160+j+ancho_sombra, 10+(ancho_sombra/2), 170+j+ancho_sombra, 30+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 170+j+ancho_sombra, 30+(ancho_sombra/2), 190+j+ancho_sombra, 10+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 190+j+ancho_sombra, 10+(ancho_sombra/2), 180+j+ancho_sombra, 50+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra E.
            tigrLine(pantalla_gameover.i, 200+j+ancho_sombra, 50+(ancho_sombra/2), 210+j+ancho_sombra, 10+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 210+ancho_sombra, 10+j+(ancho_sombra/2), 235+ancho_sombra, 10+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 240-j+ancho_sombra, 10+(ancho_sombra/2), 237-j+ancho_sombra, 20+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 211+ancho_sombra, 25+j+(ancho_sombra/2), 235+ancho_sombra, 25+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 236-j+ancho_sombra, 25+(ancho_sombra/2), 234-j+ancho_sombra, 35+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 200+j+ancho_sombra, 50-j+(ancho_sombra/2), 230+ancho_sombra, 50-j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 233-j+ancho_sombra, 41+(ancho_sombra/2), 230-j+ancho_sombra, 50+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra O.
            tigrLine(pantalla_gameover.i, 65+j+ancho_sombra, 100+(ancho_sombra/2), 75+j+ancho_sombra, 60+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 75+ancho_sombra, 60+j+(ancho_sombra/2), 105-j+ancho_sombra, 60+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 65+j+ancho_sombra, 100-j+(ancho_sombra/2), 95+ancho_sombra, 100-j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 85+j+ancho_sombra, 100+(ancho_sombra/2), 95+j+ancho_sombra, 60+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra V.
            tigrLine(pantalla_gameover.i, 115+j+ancho_sombra, 60+(ancho_sombra/2), 120+j+ancho_sombra, 100+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 120+j+ancho_sombra, 100+(ancho_sombra/2), 145+j+ancho_sombra, 60+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra E.
            tigrLine(pantalla_gameover.i, 155+j+ancho_sombra, 100+(ancho_sombra/2), 165+j+ancho_sombra, 60+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 165+ancho_sombra, 60+j+(ancho_sombra/2), 190+ancho_sombra, 60+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 195-j+ancho_sombra, 60+(ancho_sombra/2), 192-j+ancho_sombra, 70+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 166+ancho_sombra, 75+j+(ancho_sombra/2), 190+ancho_sombra, 75+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 191-j+ancho_sombra, 75+(ancho_sombra/2), 189-j+ancho_sombra, 85+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 155+j+ancho_sombra, 100-j+(ancho_sombra/2), 185+ancho_sombra, 100-j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 188-j+ancho_sombra, 91+(ancho_sombra/2), 185-j+ancho_sombra, 100+(ancho_sombra/2), cuerpo_o_sombra);
            // Letra R.
            tigrLine(pantalla_gameover.i, 200+j+ancho_sombra, 100+(ancho_sombra/2), 210+j+ancho_sombra, 60+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 210+ancho_sombra, 60+j+(ancho_sombra/2), 240+ancho_sombra, 60+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 235+j+ancho_sombra, 60+(ancho_sombra/2), 225+j+ancho_sombra, 80+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 230+ancho_sombra, 75+j+(ancho_sombra/2), 205+ancho_sombra, 75+j+(ancho_sombra/2), cuerpo_o_sombra);
            tigrLine(pantalla_gameover.i, 221+j+ancho_sombra, 75+(ancho_sombra/2), 231+j+ancho_sombra, 100+(ancho_sombra/2), cuerpo_o_sombra);
        }
        ancho_sombra-=6;
        cuerpo_o_sombra=rojo;
    }
    //Fondo y texto para boton volver a jugar.
    tigrFill(pantalla_gameover.i, boton_jugar.x, boton_jugar.y, boton_jugar.w, boton_jugar.h, presion_boton_jugar);
    tigrPrint(pantalla_gameover.i, tfont, 5*ancho_ventana/19, 18*alto_ventana/50-2, rojo, "VOLVER A JUGAR", 1);
    //Fondo y texto para boton volver al menu.
    tigrFill(pantalla_gameover.i, boton_menu.x, boton_menu.y, boton_menu.w, boton_menu.h, presion_boton_menu);
    tigrPrint(pantalla_gameover.i,  tfont, 5*ancho_ventana/19, 28*alto_ventana/50-1, rojo, "VOLVER AL MENÚ", 1);
    // Marco de ventana.
    tigrFill(pantalla_gameover.i, 0, 0, pantalla_gameover.w, 3, color_marco);
    tigrFill(pantalla_gameover.i, 0, 0, 3, pantalla_gameover.h, color_marco);
    tigrFill(pantalla_gameover.i, 0, pantalla_gameover.h-3, pantalla_gameover.w, 3, color_marco);
    tigrFill(pantalla_gameover.i, pantalla_gameover.w-3, 0, 3, pantalla_gameover.h, color_marco);
    // Marco del boton volver a jugar.
    tigrFill(pantalla_gameover.i, boton_jugar.x, boton_jugar.y, boton_jugar.w, 2, color_marco);
    tigrFill(pantalla_gameover.i, boton_jugar.x, boton_jugar.y, 2, boton_jugar.h, color_marco);
    tigrFill(pantalla_gameover.i, boton_jugar.x, boton_jugar.y+boton_jugar.h-2, boton_jugar.w, 2, color_marco);
    tigrFill(pantalla_gameover.i, boton_jugar.x+boton_jugar.w-2, boton_jugar.y, 2, boton_jugar.h, color_marco);
    // Marco para el boton volvel al menu.
    tigrFill(pantalla_gameover.i, boton_menu.x, boton_menu.y, boton_menu.w, 2, color_marco);
    tigrFill(pantalla_gameover.i, boton_menu.x, boton_menu.y, 2, boton_menu.h, color_marco);
    tigrFill(pantalla_gameover.i, boton_menu.x, boton_menu.y+boton_menu.h-2, boton_menu.w, 2, color_marco);
    tigrFill(pantalla_gameover.i, boton_menu.x+boton_menu.w-2, boton_menu.y, 2, boton_menu.h, color_marco);
    tigrBlitAlpha(pantalla, pantalla_gameover.i, pantalla_gameover.x, pantalla_gameover.y, 0, 0, pantalla_gameover.w, pantalla_gameover.h, 1);
    return false;
}
bool imprimir_victoria(Tigr *pantalla, int ancho_ventana, int alto_ventana, int *seleccion,int *caso, int nivel) {
    TCoordenadas pantalla_victoria, boton_menu, boton_nivel;
    int i,tecla;
    TPixel presion_boton_menu, presion_boton_nivel;
    TPixel color_verde,fondo,color_marco, color_botones, color_seleccionado;
    
    // Establecimiento de las coordenadas y las dimensiones de la pantalla Game Over.
    pantalla_victoria.x=ancho_ventana/8, pantalla_victoria.y=alto_ventana/10, pantalla_victoria.w=6*ancho_ventana/8, pantalla_victoria.h=8*alto_ventana/10; 
    pantalla_victoria.i=tigrBitmap(pantalla_victoria.w, pantalla_victoria.h);
    boton_nivel.x=pantalla_victoria.w/6, boton_nivel.y=3*pantalla_victoria.h/8, boton_nivel.w=4*pantalla_victoria.w/6, boton_nivel.h=pantalla_victoria.h/6;
    boton_menu.x=pantalla_victoria.w/6, boton_menu.y=5*pantalla_victoria.h/8, boton_menu.w=4*pantalla_victoria.w/6, boton_menu.h=pantalla_victoria.h/6;
    // Establecimiento del los colores.
    color_verde=tigrRGB(17,171,58), color_marco=tigrRGB(10, 87, 18), fondo=tigrRGB(151,240,160), color_botones=tigrRGB(25, 115, 86),color_seleccionado=tigrRGB(14, 61, 29);
    
    tecla=tigrReadChar(pantalla);
    
    if(((*seleccion==1) && (tigrKeyHeld(pantalla,'S'))) || (nivel==2)) {
        *seleccion=0;
    }
    else if((*seleccion==0) && (tigrKeyHeld(pantalla,'W'))) {
        *seleccion=1;
    }
    switch(*seleccion) {
        // Volver al menu.
        case 0: presion_boton_nivel=color_botones;
                presion_boton_menu=color_seleccionado;
                if(tecla==13) {
                    *caso=0;
                    return true;
                }
                break;
        // Siguiente nivel.
        case 1: presion_boton_menu=color_botones;
                presion_boton_nivel=color_seleccionado;
                if(tecla==13) {
                    *caso=3;
                    return true;
                }
                break;
    }
    tigrClear(pantalla_victoria.i, fondo);
    for(i=0;i<10;i++){
        // Letra V.
        tigrLine(pantalla_victoria.i, 15+i, 50+20, 20+i, 10+20, color_verde); 
        tigrLine(pantalla_victoria.i, 20+i, 50+20, 45+i, 10+20, color_verde); 
        // Letra I.
        tigrLine(pantalla_victoria.i, 55+i, 50+20, 65+i, 10+20, color_verde);
        // Letra C
        tigrLine(pantalla_victoria.i, 75+i, 50+20, 85+i, 10+20, color_verde); 
        tigrLine(pantalla_victoria.i, 85+i, 10+i+20, 105, 10+i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 110-i+1, 10+20, 107-i+1, 20+20, color_verde); 
        tigrLine(pantalla_victoria.i, 75+i, 50-i+20, 105, 50-i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 108-i, 41+20, 105-i, 50+20, color_verde); 
        // Letra T.
        tigrLine(pantalla_victoria.i, 125+i, 50+20, 135+i, 10+20, color_verde); 
        tigrLine(pantalla_victoria.i, 115+10-i, 10+i+20, 145-i+10, 10+i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 135, 10+i+20, 165-i, 10+i+20, color_verde);
        tigrLine(pantalla_victoria.i, 115-i+10, 10+20, 112-i+10, 20+20, color_verde);
        // Letra O.
        tigrLine(pantalla_victoria.i, 155+i, 50+20, 165+i, 10+20, color_verde); 
        tigrLine(pantalla_victoria.i, 165, 10+i+20, 195-i, 10+i+20, color_verde);
        tigrLine(pantalla_victoria.i, 155+i, 50-i+20, 185, 50-i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 175+i, 50+20, 185+i, 10+20, color_verde); 
        // Letra R.
        tigrLine(pantalla_victoria.i, 195+i, 50+20, 205+i, 10+20, color_verde);
        tigrLine(pantalla_victoria.i, 205, 10+i+20, 235, 10+i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 230+i, 10+20, 220+i, 30+20, color_verde); 
        tigrLine(pantalla_victoria.i, 225, 25+i+20, 200, 25+i+20, color_verde); 
        tigrLine(pantalla_victoria.i, 216+i, 25+20, 226+i, 50+20, color_verde); 
        // Letra I.
        tigrLine(pantalla_victoria.i, 240+i, 50+20, 250+i, 10+20, color_verde); 
        // Letra A.
        tigrLine(pantalla_victoria.i, 253+i, 50+20, 273+i, 10+20, color_verde); 
        tigrLine(pantalla_victoria.i, 273+i, 10+20, 283+i, 50+20, color_verde); 
        tigrLine(pantalla_victoria.i, 273, 40+i+20, 283, 40+i+20, color_verde); 
    }
    //Fondo y texto para boton siguente nivel.
    if(nivel!=2) {
        tigrFill(pantalla_victoria.i, boton_nivel.x, boton_nivel.y, boton_nivel.w, boton_nivel.h, presion_boton_nivel);
        tigrPrint(pantalla_victoria.i, tfont, 5*ancho_ventana/19, 18*alto_ventana/50-2, color_verde, "SIGUIENTE NIVEL", 1);
    }
    //Fondo y texto para boton volver al menu.
    tigrFill(pantalla_victoria.i, boton_menu.x, boton_menu.y, boton_menu.w, boton_menu.h, presion_boton_menu);
    tigrPrint(pantalla_victoria.i,  tfont, 5*ancho_ventana/19, 28*alto_ventana/50-1, color_verde, "VOLVER AL MENÚ", 1);
    // Pintar marcos.
    for(i=0; i<4; i++) {
        //Marco de ventana.
        tigrLine(pantalla_victoria.i, 0, i, pantalla_victoria.w, i, color_marco);
        tigrLine(pantalla_victoria.i, 0, pantalla_victoria.h-i-1, pantalla_victoria.w, pantalla_victoria.h-i-1, color_marco);
        tigrLine(pantalla_victoria.i, 0+i, 0, 0+i, pantalla_victoria.h, color_marco);
        tigrLine(pantalla_victoria.i, pantalla_victoria.w-i-1, 0, pantalla_victoria.w-i-1, pantalla_victoria.h, color_marco);
        //Marco del boton volver a jugar.
        if(nivel!=2) {
            tigrLine(pantalla_victoria.i, boton_nivel.x, boton_nivel.y+i, boton_nivel.x+boton_nivel.w, boton_nivel.y+i, color_marco);
            tigrLine(pantalla_victoria.i, boton_nivel.x+i, boton_nivel.y, boton_nivel.x+i, boton_nivel.y+boton_nivel.h, color_marco);
            tigrLine(pantalla_victoria.i, boton_nivel.x+boton_nivel.w-i, boton_nivel.y, boton_nivel.x+boton_nivel.w-i, boton_nivel.y+boton_nivel.h, color_marco);
            tigrLine(pantalla_victoria.i, boton_nivel.x, boton_nivel.y+boton_nivel.h-i, boton_nivel.x+boton_nivel.w, boton_nivel.y+boton_nivel.h-i, color_marco);
        }
        //Marco para el boton volvel al menu.
        tigrLine(pantalla_victoria.i, boton_menu.x, boton_menu.y+i, boton_menu.x+boton_menu.w, boton_menu.y+i, color_marco);
        tigrLine(pantalla_victoria.i, boton_menu.x+i, boton_menu.y, boton_menu.x+i, boton_menu.y+boton_menu.h, color_marco);
        tigrLine(pantalla_victoria.i, boton_menu.x+boton_menu.w-i, boton_menu.y, boton_menu.x+boton_menu.w-i, boton_menu.y+boton_menu.h, color_marco);
        tigrLine(pantalla_victoria.i, boton_menu.x, boton_menu.y+boton_menu.h-i, boton_menu.x+boton_menu.w, boton_menu.y+boton_menu.h-i, color_marco);
    }
    tigrBlitAlpha(pantalla, pantalla_victoria.i, pantalla_victoria.x, pantalla_victoria.y, 0, 0, pantalla_victoria.w, pantalla_victoria.h, 1);

    return false;
}
bool saber_solido(TPixel color_solido, TPixel color_posicion) {
    if((color_posicion.r==color_solido.r) && (color_posicion.g==color_solido.g) && (color_posicion.b==color_solido.b)) {
       return true; 
    }
    return false;
}
// Modulo que comprueba si el jugador choca con un barril, la tarta o con la oveja.
bool colision_objeto(TCoordenadas personaje, TCoordenadas objeto) {
    // Esquina inferior izquierda.
    if ((personaje.x>=objeto.x) && (personaje.x<=(objeto.x+objeto.w)) && ((personaje.y + personaje.h-3)>=objeto.y) && ((personaje.y+personaje.h-3)<=(objeto.y+objeto.h))) {
        return true;
    }
    // Esquina inferior derecha.
    if (((personaje.x+personaje.w)>=objeto.x) && ((personaje.x+personaje.w)<=(objeto.x+objeto.w)) && ((personaje.y+personaje.h-3)>=objeto.y) && ((personaje.y+personaje.h-3)<=(objeto.y+objeto.h))) {
        return true;
    }
    return false;
}
bool bajar_escaleras_barril(TBarriles barril, TLista_coordenadas escaleras, int caso, int *i) {
    if(barril.direccion=='D') {
        for(*i=barril.ultima_posicion_x; *i<=barril.coordenadas.x; *i++) {
            // Escalera rota.
            if((!barril.caida) && ((caso%2)==0) && (*i==(escaleras[5].x+(escaleras[5].w/4))) && (barril.coordenadas.y>=(escaleras[5].y-30)) && (barril.coordenadas.y<=(escaleras[5].y+10))) {
                return true;
            }
            else if((barril.caida) && ((caso%2)==0) && (*i==(escaleras[5].x+(escaleras[5].w/4)-3)) && (barril.coordenadas.y>=(escaleras[5].y-30)) && (barril.coordenadas.y<=(escaleras[5].y+10))) {
                return true;
            }
            // Penultima escalera.
            if((!barril.caida) && (*i==(escaleras[7].x+(escaleras[5].w/4))) && (barril.coordenadas.y>=(escaleras[7].y-30)) && (barril.coordenadas.y<=(escaleras[7].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[7].x+(escaleras[5].w/4)-3)) && (barril.coordenadas.y>=(escaleras[7].y-30)) && (barril.coordenadas.y<=(escaleras[7].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[2].x+(escaleras[2].w/4))) && (barril.coordenadas.y>=(escaleras[2].y-30)) && (barril.coordenadas.y<=(escaleras[2].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[2].x+(escaleras[2].w/4)-3)) && (barril.coordenadas.y>=(escaleras[2].y-30)) && (barril.coordenadas.y<=(escaleras[2].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[1].x+(escaleras[1].w/4))) && (barril.coordenadas.y>=(escaleras[1].y-30)) && (barril.coordenadas.y<=(escaleras[1].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[1].x+(escaleras[1].w/4)-3)) && (barril.coordenadas.y>=(escaleras[1].y-30)) && (barril.coordenadas.y<=(escaleras[1].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[0].x+(escaleras[0].w/4))) && (barril.coordenadas.y>=(escaleras[0].y-30)) && (barril.coordenadas.y<=(escaleras[0].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[0].x+(escaleras[0].w/4)-3)) && (barril.coordenadas.y>=(escaleras[0].y-30)) && (barril.coordenadas.y<=(escaleras[0].y+10))) {
                return true;
            }
        }
    }
    else if(barril.direccion=='I') {
        for(*i=barril.ultima_posicion_x; *i>=barril.coordenadas.x; *i--) {
            // Escalera rota.
            if((!barril.caida) && ((caso%2)==0) && (*i==(escaleras[5].x+(escaleras[5].w/4))) && (barril.coordenadas.y>=(escaleras[5].y-30)) && (barril.coordenadas.y<=(escaleras[5].y+10))) {
                return true;
            }
            else if((barril.caida) && ((caso%2)==0) && (*i==(escaleras[5].x+(escaleras[5].w/4)-3)) && (barril.coordenadas.y>=(escaleras[5].y-30)) && (barril.coordenadas.y<=(escaleras[5].y+10))) {
                return true;
            }
            // Penultima escalera.
            if((!barril.caida) && (*i==(escaleras[7].x+(escaleras[7].w/4))) && (barril.coordenadas.y>=(escaleras[7].y-30)) && (barril.coordenadas.y<=(escaleras[7].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[7].x+(escaleras[7].w/4)-3)) && (barril.coordenadas.y>=(escaleras[7].y-30)) && (barril.coordenadas.y<=(escaleras[7].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[2].x+(escaleras[2].w/4))) && (barril.coordenadas.y>=(escaleras[2].y-30)) && (barril.coordenadas.y<=(escaleras[2].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[2].x+(escaleras[2].w/4)-3)) && (barril.coordenadas.y>=(escaleras[2].y-30)) && (barril.coordenadas.y<=(escaleras[2].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[1].x+(escaleras[1].w/4))) && (barril.coordenadas.y>=(escaleras[1].y-30)) && (barril.coordenadas.y<=(escaleras[1].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[1].x+(escaleras[1].w/4)-3)) && (barril.coordenadas.y>=(escaleras[1].y-30)) && (barril.coordenadas.y<=(escaleras[1].y+10))) {
                return true;
            }
            if((!barril.caida) && (*i==(escaleras[0].x+(escaleras[0].w/4))) && (barril.coordenadas.y>=(escaleras[0].y-30)) && (barril.coordenadas.y<=(escaleras[0].y+10))) {
                return true;
            }
            else if((barril.caida) && (*i==(escaleras[0].x+(escaleras[0].w/4)-3)) && (barril.coordenadas.y>=(escaleras[0].y-30)) && (barril.coordenadas.y<=(escaleras[0].y+10))) {
                return true;
            }
        }
    }
    return false;
}
bool nivel2(bool victoria, bool game_over, TCoordenadas *ovni, bool *disparado, TCoordenadas *disparo, float *velocidad_disparo, float *resguardo_disparo, float *tiempo_ovni, TCoordenadas *agua, int ancho_ventana, int alto_ventana, Tigr *pantalla_juego, TCoordenadas personaje, float tiempo_total, float *resguardo_agua) {
    float cantidad_movimiento;
    // Calcula el tiempo de aparicion del ovni.
    *tiempo_ovni+=tiempo_total;
    // Creacion del color del agua.
    TPixel color_agua=tigrRGB(7, 100, 230);
    cantidad_movimiento=0;
    if(!game_over) {
        // Elevacion del agua.
        cantidad_movimiento=10*tiempo_total;
        cantidad_movimiento=(-cantidad_movimiento);
        // Calculo de la posicion vertical del agua.
        *resguardo_agua+=cantidad_movimiento;
        agua->y=*resguardo_agua;
    }
    // Que aparezca encima del jugador.
    if(*tiempo_ovni<=11) {
        ovni->x=personaje.x;
    }
    else {
        if(!*disparado) {
            disparo->x=ovni->x+(ovni->w/2);
            disparo->y=ovni->y+ovni->h;
            *resguardo_disparo=disparo->y;
            *disparado=true;
        }
    }
    if(*disparado) {
        *velocidad_disparo+=240*tiempo_total;
        *resguardo_disparo+=(*velocidad_disparo)*tiempo_total;
        disparo->y=*resguardo_disparo;
        tigrBlitAlpha(pantalla_juego, disparo->i, disparo->x, disparo->y, 0, 0, disparo->w, disparo->h, 1);
    }
    // Pinta el ovni.
    if (*tiempo_ovni>=8) {
        tigrBlitAlpha(pantalla_juego, ovni->i, ovni->x, ovni->y, 0, 0, ovni->w, ovni->h, 1);
        if(*tiempo_ovni>=13) {
            *disparado=false;
            *velocidad_disparo=0;
            *tiempo_ovni=0;
        }
    }
    // Pintar el agua.
    tigrClear(agua->i, color_agua);
    // Integracion del agua a la pantalla de juego.
    tigrBlitAlpha(pantalla_juego, agua->i, agua->x, agua->y, 0, 0, agua->w, agua->h, 0.4);
    if(!game_over) {
        if(colision_objeto(personaje, *agua) || colision_objeto(personaje, *disparo)) {
            return true;
        }
        return false;
    }
    return true;
}
int juego(Tigr *pantalla, int ancho_ventana, int alto_ventana, char nombre[11], float velocidad_ending, int *nivel) {
    TCoordenadas pantalla_juego, pantalla_estructura, personaje, agua, ovni, disparo, anterior_posicion_escalera;
    TNiveles nivel1;
    int barril_reiniciar, elevacion_cierre, seleccion, caso, i, j, ultima_posicion_y, ultima_posicion_x, numero_barriles, gravedad, gravedad_barriles, posicion_escalera;
    float tiempo_inicio, tiempo_ovni, tiempo_juego, tiempo_barriles, tiempo_total=0, resguardo_gravedad, velocidad_caida, velocidad_disparo, resguardo_disparo, resguardo_movimiento, resguardo_escalera, cantidad_movimiento, cantidad_movimiento_escalera, resguardo_agua;
    char ultima_direccion;
    bool disparado, opening, cierre, pausa, caida, ending, esta_escalera, colgando, game_over, primer_salto, victoria;
    
    // Asignacion de las coordenadas y dimensiones de las escaleas del nivel 1 y 2.
    for(i=0; i<9; i++) {
        nivel1.escaleras[i].w=3*ancho_ventana/50;
    }
    nivel1.escaleras[0].x=43*ancho_ventana/50, nivel1.escaleras[0].y=44*alto_ventana/50, nivel1.escaleras[0].h= 6*alto_ventana/50;
    nivel1.escaleras[1].x=5*ancho_ventana/50, nivel1.escaleras[1].y=39*alto_ventana/50, nivel1.escaleras[1].h= 5*alto_ventana/50-1;
    nivel1.escaleras[2].x=43*ancho_ventana/50, nivel1.escaleras[2].y=32*alto_ventana/50, nivel1.escaleras[2].h= 6*alto_ventana/50-1;
    nivel1.escaleras[3].x=25*ancho_ventana/50, nivel1.escaleras[3].y=33*alto_ventana/50, nivel1.escaleras[3].h= 5*alto_ventana/50+3;
    nivel1.escaleras[4].x=5*ancho_ventana/50, nivel1.escaleras[4].y=22*alto_ventana/50, nivel1.escaleras[4].h= 5*alto_ventana/50;
    nivel1.escaleras[5].x=16*ancho_ventana/50, nivel1.escaleras[5].y=15*alto_ventana/50, nivel1.escaleras[5].h= 1*alto_ventana/50;
    nivel1.escaleras[6].x=16*ancho_ventana/50, nivel1.escaleras[6].y=20*alto_ventana/50, nivel1.escaleras[6].h= 1*alto_ventana/50-2;
    nivel1.escaleras[7].x=43*ancho_ventana/50, nivel1.escaleras[7].y=15*alto_ventana/50, nivel1.escaleras[7].h= 5*alto_ventana/50-1;
    nivel1.escaleras[8].x=12*ancho_ventana/50, nivel1.escaleras[8].y=10*alto_ventana/50, nivel1.escaleras[8].h= 5*alto_ventana/50-1;
    // Asignacion de la coordenadas iniciales del jugador.
    personaje.x=5, personaje.w=18, personaje.h=28;
    personaje.y=alto_ventana-personaje.h-2;
    // Asignacion de las coordenadas y dimensiones de la tarta.
    nivel1.tarta.i=tigrLoadImage("./imagenes/tarta.png"), nivel1.tarta.x=22*ancho_ventana/50, nivel1.tarta.y=5*alto_ventana/50, nivel1.tarta.w=47, nivel1.tarta.h=45;
    // Asignacion de las coordenadas y dimensiones de la oveja.
    nivel1.oveja.i=tigrLoadImage("./imagenes/oveja.png"), nivel1.oveja.x=2*ancho_ventana/50, nivel1.oveja.y=11*alto_ventana/50-6, nivel1.oveja.w=57, nivel1.oveja.h=41;
    // Asignacion de las coordenadas y dimensiones del ovni.
    ovni.i=tigrLoadImage("./imagenes/ovni.png"), ovni.x=25*ancho_ventana/50, ovni.y=6*alto_ventana/50, ovni.w=32, ovni.h=22;
    // Asignacion de las coordenadas y dimensiones del disparo.
    disparo.i=tigrLoadImage("./imagenes/disparo.png"), disparo.x=ancho_ventana, disparo.y=alto_ventana, disparo.w=4, disparo.h=9;

    pantalla_estructura.x=0, pantalla_estructura.y=0, pantalla_estructura.w=ancho_ventana, pantalla_estructura.h=alto_ventana;
    pantalla_estructura.i=tigrBitmap(pantalla_estructura.w, pantalla_estructura.h);
    pantalla_juego.x=0, pantalla_juego.y=0, pantalla_juego.w=ancho_ventana, pantalla_juego.h=alto_ventana;
    pantalla_juego.i=tigrBitmap(pantalla_juego.w, pantalla_juego.h); 
    // Asignacion de las coordenadas y dimensiones del agua.
    agua.x=0, agua.y=6*alto_ventana/4, agua.w=ancho_ventana, agua.h=alto_ventana;
    agua.i=tigrBitmap(agua.w, agua.h);
    
    barril_reiniciar=0, elevacion_cierre=0 , seleccion=1, caso=-1, ultima_posicion_y=personaje.y, ultima_posicion_x=personaje.x, numero_barriles=0, gravedad=900, gravedad_barriles=500, posicion_escalera=0;
    
    tiempo_inicio=0, tiempo_ovni=0, tiempo_juego=0, tiempo_barriles=3, resguardo_movimiento=personaje.x, resguardo_escalera=personaje.y, cantidad_movimiento=0, cantidad_movimiento_escalera=0, resguardo_gravedad=personaje.y, velocidad_caida=0, velocidad_disparo=0, resguardo_agua=agua.y, resguardo_escalera=personaje.y;
    
    ultima_direccion='D';
    
    disparado=false, opening=true, cierre=false, pausa=false, caida=true, ending=false, esta_escalera=false, colgando=false, game_over=false, primer_salto=false, victoria=false;
    // Creacion del color de las estructuras.
    TPixel color_solido=tigrRGB(69, 33, 217), color_solido2=tigrRGB(4, 19, 64),color_escalera=tigrRGB(30,222,45), color_barra=tigrRGB(120,143,63);
    // Colores para la barra de juego.
    TPixel color_barrajuego=tigrRGB(4,39,195), color_texto=tigrRGB(255,255,255), color_barrajuego2=tigrRGB(144,144,255);
    // Creacion del color del texto inicio.
    TPixel color_rojo=tigrRGB(255, 18, 77);
    // Creacion de las variables para saber si estamos encima de un solido y si hay rampas.
    TPixel color_mas_abajo, color_debajo, color_medio, color_arriba;
    while((!tigrClosed(pantalla)) && (!cierre)) {
        tiempo_total=Calcular_Tiempo();
        // Limpia el fondo de pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        // Limpia la pantalla de juego.
        tigrClear(pantalla_juego.i, tigrRGB(0, 0, 0));
        // Elevacion del cierre.
        if(ending) {
            elevacion_cierre=velocidad_ending*tiempo_total;
            pantalla_juego.y-=elevacion_cierre;
        }
        // Comprobacion de si el juego se va ha cerrar.
        if(pantalla_juego.y<=-pantalla_juego.h) {
            cierre=true;
        }
        if((!game_over) && (!ending) && (!victoria)) {
            // Calcula el tiempo del inicio.
            tiempo_inicio+=tiempo_total;
            // Calcula el tiempo de juego.
            if(!opening) {
                tiempo_juego+=tiempo_total;
            }
            tiempo_barriles+=tiempo_total;
            // Detecta que direccion que se quiere mover el jugador.
            if((!opening) && (tigrKeyHeld(pantalla, 'A')) && (personaje.x>0)) {
                cantidad_movimiento=100*tiempo_total;
                cantidad_movimiento=-cantidad_movimiento;
                ultima_direccion='I';
            }
            else if((!opening) && (tigrKeyHeld(pantalla, 'D')) && ((personaje.x+personaje.w)<ancho_ventana)){
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
            if((tigrKeyHeld(pantalla, 'D')) && (!caida)) {
                for(i=ultima_posicion_x; i<=personaje.x; i++) {
                    // Saber si hay solido a la derecha.
                    color_mas_abajo=tigrGet(pantalla_estructura.i, i+personaje.w/2, personaje.y+personaje.h+2);
                    // Saber si estas atravesando solido.
                    color_debajo=tigrGet(pantalla_estructura.i, i+personaje.w/2, personaje.y+personaje.h+1);
                    // Colision plataforma.
                    if((color_mas_abajo.r==color_solido.r) && (color_mas_abajo.g==color_solido.g) && (color_mas_abajo.b==color_solido.b) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                        personaje.y+=1;
                        resguardo_gravedad=personaje.y;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Que el jugador pueda bajar rampas izquierda.
            if((tigrKeyHeld(pantalla, 'A')) && (!caida)) {
                for(i=ultima_posicion_x; i>=personaje.x; i--) {
                    // Saber si hay solido a la izquierda.
                    color_mas_abajo=tigrGet(pantalla_estructura.i, i+personaje.w/2, personaje.y+personaje.h+2);
                    // Saber si estas atravesando solido.
                    color_debajo=tigrGet(pantalla_estructura.i, i+personaje.w/2, personaje.y+personaje.h+1);
                    // Colision plataforma.
                    if((color_mas_abajo.r==color_solido.r) && (color_mas_abajo.g==color_solido.g) && (color_mas_abajo.b==color_solido.b) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                        personaje.y+=1;
                        resguardo_gravedad=personaje.y;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Saber si el jugador esta en el aire o no.
            color_debajo=tigrGet(pantalla_estructura.i, personaje.x+personaje.w/2, personaje.y+personaje.h+1);
            if((!colgando) && (color_debajo.r!=color_solido.r) && (color_debajo.g!=color_solido.g) && (color_debajo.b!=color_solido.b)) {
                caida=true;
            }
            // Que el jugador pueda subir rampas derecha.
            if((!caida) && (tigrKeyHeld(pantalla, 'D'))) {
                for(i=ultima_posicion_x; i<personaje.x; i++) {
                    // Saber si hay solido a la derecha.
                    color_medio=tigrGet(pantalla_estructura.i, i+personaje.w/2+1, personaje.y+personaje.h);
                    // Saber si estas atravesando solido.
                    color_arriba=tigrGet(pantalla_estructura.i, i+personaje.w/2+1, personaje.y+personaje.h-1);
                    // Colision plataforma.
                    if((color_medio.r==color_solido.r) && (color_medio.g==color_solido.g) && (color_medio.b==color_solido.b) && (color_arriba.r!=color_solido.r) && (color_arriba.g!=color_solido.g) && (color_arriba.b!=color_solido.b)) {
                        personaje.y-=1;
                        resguardo_gravedad=personaje.y;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Que el jugador pueda subir rampas izquierda.
            if((!caida) && (tigrKeyHeld(pantalla, 'A'))) {
                for(i=ultima_posicion_x; i>personaje.x; i--) {
                    // Saber si hay solido a la derecha.
                    color_medio=tigrGet(pantalla_estructura.i, i+personaje.w/2-1, personaje.y+personaje.h);
                    // Saber si estas atravesando solido.
                    color_arriba=tigrGet(pantalla_estructura.i, i+personaje.w/2-1, personaje.y+personaje.h-1);
                    // Colision plataforma.
                    if((color_medio.r==color_solido.r) && (color_medio.g==color_solido.g) && (color_medio.b==color_solido.b) && (color_arriba.r!=color_solido.r) && (color_arriba.g!=color_solido.g) && (color_arriba.b!=color_solido.b)) {
                        personaje.y-=1;
                        resguardo_gravedad=personaje.y;
                        ultima_posicion_y=personaje.y;
                        break;
                    }
                }
            }
            // Detecta si el jugador quiere saltar.
            if((!opening) && (tigrKeyDown(pantalla, TK_SPACE)) && (!caida)) {
                personaje.y--;
                velocidad_caida=-250;
                caida=true;
                colgando=false;
            }
            // Saber si el jugador esta en las escaleras.
            esta_escalera=ver_escalera(personaje, nivel1.escaleras);
            // Subir escaleras.
            anterior_posicion_escalera=personaje;
            if((esta_escalera) && (tigrKeyHeld(pantalla, 'W'))) {
                colgando=true;
                caida=false;
                velocidad_caida=0;
                cantidad_movimiento_escalera=60*tiempo_total;
                cantidad_movimiento_escalera=-cantidad_movimiento_escalera;
                resguardo_escalera+=cantidad_movimiento_escalera;
                personaje.y=resguardo_escalera;
                resguardo_gravedad=personaje.y;
                ultima_posicion_y=personaje.y;
            }
            else if(!esta_escalera) {
                colgando=false;
            }
            // Comprobar que en todo el recorrido de escalera no haya llegado al tope.
            for(anterior_posicion_escalera.y; anterior_posicion_escalera.y>=personaje.y && colgando && !ver_escalera(personaje, nivel1.escaleras); anterior_posicion_escalera.y--) {
                if(!ver_escalera(anterior_posicion_escalera, nivel1.escaleras)) {
                    resguardo_escalera-=anterior_posicion_escalera.y-personaje.y-5;
                    personaje.y=resguardo_escalera;
                    resguardo_gravedad=personaje.y;
                    ultima_posicion_y=personaje.y;
                    break;
                }
            }
            if(!colgando) {
                resguardo_escalera=personaje.y;
            }
            // Calculo de la gravedad del jugador.
            if((caida) && (!colgando)) {
                velocidad_caida+=gravedad*tiempo_total;
                resguardo_gravedad+=velocidad_caida*tiempo_total;
                ultima_posicion_y=personaje.y;
                personaje.y=resguardo_gravedad;
            }
            else {
                velocidad_caida=0;
            }
            // Comprobar que en todo el recorrido no se ha chocado con una estructura.
            if((velocidad_caida>=0) && (caida)) {
                for(i=ultima_posicion_y; i<=personaje.y; i++) {
                    // Saber si hay solido debajo.
                    color_debajo=tigrGet(pantalla_estructura.i, personaje.x+personaje.w/2, i+personaje.h+1);
                    // Saber si estas atravesando solido.
                    color_medio=tigrGet(pantalla_estructura.i, personaje.x+personaje.w/2, i+personaje.h);
                    // Colision plataforma.
                    if((saber_solido(color_solido, color_debajo)) && (color_medio.r!=color_solido.r) && (color_medio.g!=color_solido.g) && (color_medio.b!=color_solido.b)) {
                        caida=false;
                        velocidad_caida=0;
                        personaje.y=i;
                        resguardo_gravedad=personaje.y;
                        break;
                    }
                }
            }
            // Generar nuevo barril y reiniciar posicion barriles.
            if(tiempo_barriles>4) {
                tiempo_barriles=0;
                nivel1.barriles[barril_reiniciar].coordenadas.i=tigrLoadImage("./imagenes/barril.png"), nivel1.barriles[barril_reiniciar].coordenadas.x=9*ancho_ventana/50,  nivel1.barriles[barril_reiniciar].coordenadas.y=13*alto_ventana/50-3 ,  nivel1.barriles[barril_reiniciar].coordenadas.w=20, nivel1.barriles[barril_reiniciar].coordenadas.h=20;
                nivel1.barriles[barril_reiniciar].direccion='D';
                nivel1.barriles[barril_reiniciar].resguardo=nivel1.barriles[barril_reiniciar].coordenadas.x;
                nivel1.barriles[barril_reiniciar].caida=false;
                nivel1.barriles[barril_reiniciar].velocidad_caida=0;
                nivel1.barriles[barril_reiniciar].resguardo_gravedad=nivel1.barriles[barril_reiniciar].coordenadas.y;
                nivel1.barriles[barril_reiniciar].ultima_posicion_y=nivel1.barriles[barril_reiniciar].coordenadas.y;
                nivel1.barriles[barril_reiniciar].ultima_posicion_x=nivel1.barriles[barril_reiniciar].coordenadas.x;
                barril_reiniciar++;
                if(barril_reiniciar==10) {
                    barril_reiniciar=0;
                }
                if(numero_barriles!=10) {
                    numero_barriles++;
                }
            }
            // Desplazamiento de los barriles horizontalmente.
            for(i=0; i<numero_barriles; i++) {
                if(!nivel1.barriles[i].caida) {
                    nivel1.barriles[i].cantidad_movimiento=50*tiempo_total;
                    if(nivel1.barriles[i].direccion=='I') {
                        nivel1.barriles[i].cantidad_movimiento=(-nivel1.barriles[i].cantidad_movimiento);
                    }
                    nivel1.barriles[i].resguardo+=nivel1.barriles[i].cantidad_movimiento;
                    nivel1.barriles[i].ultima_posicion_x=nivel1.barriles[i].coordenadas.x;
                    nivel1.barriles[i].coordenadas.x=nivel1.barriles[i].resguardo;
                }
                // Que el barril pueda bajar rampas derecha.
                if((!nivel1.barriles[i].caida) && (nivel1.barriles[i].direccion=='D')) {
                    for(j=nivel1.barriles[i].ultima_posicion_x; j<=nivel1.barriles[i].coordenadas.x; j++) {
                        // Saber si hay solido a la derecha.
                        color_mas_abajo=tigrGet(pantalla_estructura.i, j+nivel1.barriles[i].coordenadas.w/2, nivel1.barriles[i].coordenadas.y+nivel1.barriles[i].coordenadas.h+2);
                        // Saber si estas atravesando solido.
                        color_debajo=tigrGet(pantalla_estructura.i, j+nivel1.barriles[i].coordenadas.w/2, nivel1.barriles[i].coordenadas.y+nivel1.barriles[i].coordenadas.h+1);
                        // Colision plataforma.
                        if((((saber_solido(color_solido, color_mas_abajo)) || (saber_solido(color_solido2, color_mas_abajo))) && (!saber_solido(color_solido, color_debajo)) && (!saber_solido(color_solido2, color_debajo)) && ((i%2)==0)) || ((saber_solido(color_solido, color_mas_abajo)) && (!saber_solido(color_solido, color_debajo)) && ((i%2)!=0))) {
                            nivel1.barriles[i].coordenadas.y+=1;
                            nivel1.barriles[i].resguardo_gravedad=nivel1.barriles[i].coordenadas.y;
                            nivel1.barriles[i].ultima_posicion_y=nivel1.barriles[i].coordenadas.y;
                            break;
                        }
                    }
                }
                // Que el barril pueda bajar rampas izquierda.
                if((!nivel1.barriles[i].caida) && (nivel1.barriles[i].direccion=='I')) {
                    for(j=nivel1.barriles[i].ultima_posicion_x; j>=nivel1.barriles[i].coordenadas.x; j--) {
                        // Saber si hay solido a la izquierda.
                        color_mas_abajo=tigrGet(pantalla_estructura.i, j+nivel1.barriles[i].coordenadas.w/2, nivel1.barriles[i].coordenadas.y+nivel1.barriles[i].coordenadas.h+2);
                        // Saber si estas atravesando solido.
                        color_debajo=tigrGet(pantalla_estructura.i, j+nivel1.barriles[i].coordenadas.w/2, nivel1.barriles[i].coordenadas.y+nivel1.barriles[i].coordenadas.h+1);
                        // Colision plataforma.
                        if((((saber_solido(color_solido, color_mas_abajo)) || (saber_solido(color_solido2, color_mas_abajo))) && (!saber_solido(color_solido, color_debajo)) && (!saber_solido(color_solido2, color_debajo)) && ((i%2)==0)) || ((saber_solido(color_solido, color_mas_abajo)) && (!saber_solido(color_solido, color_debajo)) && ((i%2)!=0))) {
                            nivel1.barriles[i].coordenadas.y+=1;
                            nivel1.barriles[i].resguardo_gravedad=nivel1.barriles[i].coordenadas.y; 
                            nivel1.barriles[i].ultima_posicion_y=nivel1.barriles[i].coordenadas.y; 
                            break;
                        }
                    }
                }
                // Saber si los barriles estan en el aire o no.
                color_debajo=tigrGet(pantalla_estructura.i, nivel1.barriles[i].coordenadas.x+nivel1.barriles[i].coordenadas.w/2, nivel1.barriles[i].coordenadas.y+nivel1.barriles[i].coordenadas.h+1);
                if(((!nivel1.barriles[i].caida) && (!saber_solido(color_solido, color_debajo)) && (!saber_solido(color_solido2, color_debajo)) && ((i%2)==0)) || ((!nivel1.barriles[i].caida) && (!saber_solido(color_solido, color_debajo)) && ((i%2)!=0))) {
                    nivel1.barriles[i].caida=true;
                    if(nivel1.barriles[i].direccion=='D') {
                        nivel1.barriles[i].direccion='I';
                    }
                    else {
                        nivel1.barriles[i].direccion='D';
                    }
                }
                // Que los barriles bajen escaleras.
                nivel1.barriles[i].esta_escalera=bajar_escaleras_barril(nivel1.barriles[i], nivel1.escaleras, i, &posicion_escalera);
                if((nivel1.barriles[i].esta_escalera) && (!nivel1.barriles[i].caida)) {
                    if(nivel1.barriles[i].direccion=='D') {
                        nivel1.barriles[i].direccion='I';
                    }
                    else {
                        nivel1.barriles[i].direccion='D';
                    }
                    nivel1.barriles[i].caida=true;
                    nivel1.barriles[i].coordenadas.x=posicion_escalera;
                }
                // Cambio de imagen de los barriles si estan en el aire.
                if(nivel1.barriles[i].caida && nivel1.barriles[i].coordenadas.w==20) {
                    nivel1.barriles[i].coordenadas.i=tigrLoadImage("./imagenes/barril_caida.png");
                    nivel1.barriles[i].coordenadas.w=26;
                    nivel1.barriles[i].coordenadas.x-=3;
                    nivel1.barriles[i].ultima_posicion_x=nivel1.barriles[i].coordenadas.x;
                }
                else if(!nivel1.barriles[i].caida && nivel1.barriles[i].coordenadas.w==26) {
                    nivel1.barriles[i].coordenadas.i=tigrLoadImage("./imagenes/barril.png");
                    nivel1.barriles[i].coordenadas.w=20;
                }
                // Calculo de la gravedad de los barriles.
                if(nivel1.barriles[i].caida) {
                    nivel1.barriles[i].velocidad_caida+=gravedad_barriles*tiempo_total;
                    nivel1.barriles[i].resguardo_gravedad+=nivel1.barriles[i].velocidad_caida*tiempo_total;
                    nivel1.barriles[i].ultima_posicion_y=nivel1.barriles[i].coordenadas.y;
                    nivel1.barriles[i].coordenadas.y=nivel1.barriles[i].resguardo_gravedad;
                }
                // Comprobar que en todo el recorrido el barril no se ha chocado con una estructura.
                if((nivel1.barriles[i].caida) && (!nivel1.barriles[i].esta_escalera)) {
                    for(j=nivel1.barriles[i].ultima_posicion_y; j<=nivel1.barriles[i].coordenadas.y; j++) {
                        // Saber si hay solido debajo.
                        color_debajo=tigrGet(pantalla_estructura.i, nivel1.barriles[i].coordenadas.x+nivel1.barriles[i].coordenadas.w/2, j+nivel1.barriles[i].coordenadas.h+1);
                        // Saber si estas atravesando solido.
                        color_medio=tigrGet(pantalla_estructura.i, nivel1.barriles[i].coordenadas.x+nivel1.barriles[i].coordenadas.w/2, j+nivel1.barriles[i].coordenadas.h);
                        // Colision plataforma.
                        if((((saber_solido(color_solido, color_debajo)) || (saber_solido(color_solido2, color_debajo))) && (!saber_solido(color_solido, color_medio)) && (!saber_solido(color_solido2, color_medio)) && ((i%2)==0)) || ((saber_solido(color_solido, color_debajo)) && (!saber_solido(color_solido, color_medio)) && ((i%2)!=0))) {
                            nivel1.barriles[i].caida=false;
                            nivel1.barriles[i].velocidad_caida=0;
                            nivel1.barriles[i].coordenadas.y=j;
                            nivel1.barriles[i].resguardo_gravedad=nivel1.barriles[i].coordenadas.y;
                            break;
                        }
                    }
                }
                // Comprobar si un barril se ha chocado con el jugador.
                if(colision_objeto(personaje, nivel1.barriles[i].coordenadas)) {
                    game_over=true;
                }
            }
            // Comprobar si el jugador ha colisionado con la oveja.
            if(!game_over) {
                game_over=colision_objeto(personaje, nivel1.oveja);
            }
            // Comprobar si el jugador ha llegado a la tarta.
            if(!game_over) {
                victoria=colision_objeto(personaje, nivel1.tarta);
            }
        }
        // Efecto de muerte.
        if((game_over) && (!primer_salto)) {
            personaje.y--;
            velocidad_caida=-250;
            primer_salto=true;
        }
        // Se llama el modulo para pintar la estructura del juego.
        pintar_estructuras_nivel1(pantalla_estructura.i, ancho_ventana, alto_ventana, color_solido, color_escalera, color_barra, color_solido2, nivel1);
        // Integracion de la estructura al juego.
        tigrBlitAlpha(pantalla_juego.i, pantalla_estructura.i, pantalla_estructura.x, pantalla_estructura.y, 0, 0, pantalla_estructura.w, pantalla_estructura.h, 1);
        // Pinta los barriles.
        pintar_barriles(pantalla_juego.i, numero_barriles, nivel1.barriles);
        // Asignacion de que PNG pintar segun el estado del jugador.
        if((!opening) && (tigrKeyHeld(pantalla, 'A')) && (!game_over)) {
            personaje.i=tigrLoadImage("./imagenes/personaje/caminar_izquierda.png");
        }
        else if((!opening) && (tigrKeyHeld(pantalla, 'D')) && (!game_over)) {
            personaje.i=tigrLoadImage("./imagenes/personaje/caminar_derecha.png");
        }
        else if((ultima_direccion=='D') && (!game_over)) {
            personaje.i=tigrLoadImage("./imagenes/personaje/estatico_derecha.png");
        }
        else if((!opening) && (ultima_direccion=='I') && (!game_over)) {
            personaje.i=tigrLoadImage("./imagenes/personaje/estatico_izquierda.png");
        }
        // Pinta la tarta.
        tigrBlitAlpha(pantalla_juego.i, nivel1.tarta.i, nivel1.tarta.x, nivel1.tarta.y, 0, 0, nivel1.tarta.w, nivel1.tarta.h, 1);
        // Pinta la oveja.
        tigrBlitAlpha(pantalla_juego.i, nivel1.oveja.i, nivel1.oveja.x, nivel1.oveja.y, 0, 0, nivel1.oveja.w, nivel1.tarta.h, 1);
        // Pinta el jugador por pantalla.
        tigrBlitAlpha(pantalla_juego.i, personaje.i, personaje.x, personaje.y, 0, 0, personaje.w, personaje.h, 1);
        // Exclusivo del nivel 2.
        if(*nivel==2) {
            game_over=nivel2(victoria , game_over, &ovni, &disparado, &disparo, &velocidad_disparo, &resguardo_disparo, &tiempo_ovni, &agua, ancho_ventana, alto_ventana, pantalla_juego.i, personaje, tiempo_total, &resguardo_agua);
        }
        // Pinta contador del comienzo.
        if(tiempo_inicio<=3) {
            tigrPrint(pantalla_juego.i, tfont, 19*ancho_ventana/50+7, alto_ventana/2, color_rojo, "COMIENZA EN...", 1);
        }
        else if(tiempo_inicio<=5) {
            tigrPrint(pantalla_juego.i, tfont, ancho_ventana/2, alto_ventana/2, color_rojo, "3", 1);
        }
        else if(tiempo_inicio<=6) {
            tigrPrint(pantalla_juego.i, tfont, ancho_ventana/2, alto_ventana/2, color_rojo, "2", 1);
        }
        else if(tiempo_inicio<=8) {
            tigrPrint(pantalla_juego.i, tfont, ancho_ventana/2, alto_ventana/2, color_rojo, "1", 1);
        }
        else if(tiempo_inicio<=10) {
            tigrPrint(pantalla_juego.i, tfont, ancho_ventana/2, alto_ventana/2, color_rojo, "YA", 1);
            opening=false;
        }
        // Integracion de la barra del juego.
        barra_juego(pantalla_juego.i, ancho_ventana, alto_ventana, nombre, tiempo_juego, color_barrajuego, color_texto, color_barrajuego2, *nivel);
        // Integracion de la ventana de juego a la pantalla de juego.
        tigrBlitAlpha(pantalla, pantalla_juego.i, pantalla_juego.x, pantalla_juego.y, 0, 0, pantalla_juego.w, pantalla_juego.h, 1);
        // Muestra la pantalla de game over.
        if((personaje.y>alto_ventana+alto_ventana/4) && (game_over) && (!ending)) {
            personaje.y=2*alto_ventana;
            ending=imprimir_game_over(pantalla, ancho_ventana, alto_ventana, &seleccion, &caso);
        }
        else if((game_over) && (!ending)) {
            velocidad_caida+=gravedad*tiempo_total;
            resguardo_gravedad+=velocidad_caida*tiempo_total;
            ultima_posicion_y=personaje.y;
            personaje.y=resguardo_gravedad;
        }
        // Muestra la pantalla victoria.
        if((victoria) && (!ending)) {
            ending=imprimir_victoria(pantalla, ancho_ventana, alto_ventana, &seleccion, &caso, *nivel);
        }
        // Actualiza la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}
int selector_de_niveles(Tigr *pantalla, int ancho_ventana, int alto_ventana, int *nivel, float velocidad_ending) {
    TCoordenadas cursor, pantalla_niveles, boton_nivel1, boton_nivel2, plantilla;
    int caso, click=0, last_click=0, seleccion, elevacion_cierre=0;
    float tiempo_total=0;
    bool cierre, ending;
    TPixel tema_nivel1,tema_nivel2;
    TPixel tema_niveles= tigrRGB(116, 130, 252), tema_nombre_niveles=tigrRGB(40, 131, 250), color_azul=tigrRGB(69, 0, 98),presionado=tigrRGB(70,83,199), color_morado=tigrRGB(160, 0, 100);
    
    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0;
    // Asignacion de las coordenadas y las dimensiones de la pantalla niveles.
    pantalla_niveles.x=0, pantalla_niveles.y=0, pantalla_niveles.w=ancho_ventana, pantalla_niveles.h=alto_ventana;
    pantalla_niveles.i=tigrBitmap(pantalla_niveles.w, pantalla_niveles.h);
    plantilla.x=ancho_ventana/8, plantilla.y=alto_ventana/10, plantilla.w=6*ancho_ventana/8, plantilla.h=8*alto_ventana/10;
    boton_nivel1.x=4*ancho_ventana/16, boton_nivel1.y=4*alto_ventana/16, boton_nivel1.w=8*ancho_ventana/16, boton_nivel1.h=2*alto_ventana/16;
    boton_nivel2.x=4*ancho_ventana/16, boton_nivel2.y=10*alto_ventana/16, boton_nivel2.w=8*ancho_ventana/16, boton_nivel2.h=2*alto_ventana/16;
    
    caso=-1;
    
    cierre=false, ending=false;
    
    while((!tigrClosed(pantalla)) && (!cierre)) {
        tiempo_total=Calcular_Tiempo();
        tigrMouse(pantalla, &cursor.x, &cursor.y, &click);
        // Vuelve al menu.
        if((tigrKeyHeld(pantalla, TK_ESCAPE)) && (!ending)) {
            ending=true;
            caso=0;
        }
        // Elevacion del cierre.
        if(ending) {
            elevacion_cierre=velocidad_ending*tiempo_total;
            pantalla_niveles.y-=elevacion_cierre;
        }
        // Comprobacion de si la ventana se va ha cerrar.
        if(pantalla_niveles.y<=-pantalla_niveles.h) {
            cierre=true;
        }
        // Detecta que boton esta seleccionado.
        if((!ending) && (cursor.x>=boton_nivel1.x) && (cursor.y>=boton_nivel1.y) && (cursor.x<=boton_nivel1.x+boton_nivel1.w) && (cursor.y<=boton_nivel1.y+boton_nivel1.h)) {
            seleccion=1; 
        }
        else if((!ending) && (cursor.x>=boton_nivel2.x) && (cursor.y>=boton_nivel2.y) && (cursor.x<=boton_nivel2.x+boton_nivel2.w) && (cursor.y<=boton_nivel2.y+boton_nivel2.h)) {
            seleccion=2;
        }
        else if(!ending){
            seleccion=-1;
        }
        switch(seleccion){
            case 1:
                tema_nivel1=presionado;
                tema_nivel2=tema_niveles;
                if((click==1) && (last_click==0)) {
                    *nivel=1;
                    caso=1;
                    ending=true;
                }
                break;
            case 2:
                tema_nivel1=tema_niveles;
                tema_nivel2=presionado;
                if((click==1) && (last_click==0)) {
                    *nivel=2;
                    caso=1;
                    ending=true;
                 }
            break;
            case -1:
                tema_nivel1=tema_niveles;
                tema_nivel2=tema_niveles;
                break;
        }
        last_click=click;
        // Limpia el fondo de la pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        if(!ending) {
            // Limpia la pantalla de niveles.
            tigrClear(pantalla_niveles.i, tigrRGB(0, 0, 0));
            // Obtiene la posición del cursor y si esta haciendo click.
            tigrPrint(pantalla_niveles.i, tfont, 15*ancho_ventana/50-1, 2*alto_ventana/50+3, tigrRGB(255,255,255), "Selecciona el nivel a jugar:", 0);
            // Pinta la plantilla.
            tigrFill(pantalla_niveles.i, plantilla.x, plantilla.y, plantilla.w, plantilla.h, color_azul);
            // Marco plantilla.
            tigrFill(pantalla_niveles.i, plantilla.x, plantilla.y, plantilla.w, 3, color_morado);
            tigrFill(pantalla_niveles.i, plantilla.x, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_niveles.i, plantilla.x+plantilla.w-3, plantilla.y, 3, plantilla.h, color_morado);
            tigrFill(pantalla_niveles.i, plantilla.x, plantilla.y+plantilla.h-3, plantilla.w, 3, color_morado);
            // Pinta los botones.
            tigrFill(pantalla_niveles.i, boton_nivel1.x, boton_nivel1.y, boton_nivel1.w, boton_nivel1.h, tema_nivel1);
            tigrFill(pantalla_niveles.i, boton_nivel2.x, boton_nivel2.y, boton_nivel2.w, boton_nivel2.h , tema_nivel2);
            // Pinta el texto de los botones.
            tigrPrint(pantalla_niveles.i, tfont, 9*ancho_ventana/20-4 , 6*alto_ventana/20+1, tema_nombre_niveles , "NIVEL 1", 0);
            tigrPrint(pantalla_niveles.i, tfont, 9*ancho_ventana/20-4 , 14*alto_ventana/20-9, tema_nombre_niveles , "NIVEL 2", 0);
            //Marco boton nivel 1
            tigrFill(pantalla_niveles.i, boton_nivel1.x, boton_nivel1.y, boton_nivel1.w, 2, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel1.x, boton_nivel1.y, 2, boton_nivel1.h, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel1.x+boton_nivel1.w-2, boton_nivel1.y, 2, boton_nivel1.h, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel1.x, boton_nivel1.y+boton_nivel1.h-2, boton_nivel1.w, 2, color_morado);
            //Marco boton nivel 2
            tigrFill(pantalla_niveles.i, boton_nivel2.x, boton_nivel2.y, boton_nivel2.w, 2, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel2.x, boton_nivel2.y, 2, boton_nivel2.h, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel2.x+boton_nivel2.w-2, boton_nivel2.y, 2, boton_nivel2.h, color_morado);
            tigrFill(pantalla_niveles.i, boton_nivel2.x, boton_nivel2.y+boton_nivel2.h-2, boton_nivel2.w, 2, color_morado);
            // Pinta cursor personalizado.
            tigrBlitAlpha(pantalla_niveles.i, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1);
        }
        // Integracion de la ventana niveles a la pantalla
        tigrBlitAlpha(pantalla, pantalla_niveles.i, pantalla_niveles.x, pantalla_niveles.y, 0, 0, pantalla_niveles.w, pantalla_niveles.h, 1);
        // Refresca la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}
int imprimir_creditos(Tigr *pantalla, int ancho_ventana, int alto_ventana, float velocidad_ending) {
    TCoordenadas cursor, pantalla_creditos, boton_nivel1, boton_nivel2, plantilla;
    int caso, click=0, last_click=0, seleccion, elevacion_cierre=0;
    float tiempo_total=0;
    bool cierre, ending;
    TPixel color_azul=tigrRGB(69, 0, 98), color_morado=tigrRGB(160, 0, 100);
    
    cursor.i=tigrLoadImage("./imagenes/cursor.png"), cursor.x=0, cursor.y=0;
    // Asignacion de las coordenadas y las dimensiones de la pantalla niveles.
    pantalla_creditos.x=0, pantalla_creditos.y=0, pantalla_creditos.w=ancho_ventana, pantalla_creditos.h=alto_ventana;
    pantalla_creditos.i=tigrBitmap(pantalla_creditos.w, pantalla_creditos.h);
    plantilla.x=ancho_ventana/8, plantilla.y=alto_ventana/10, plantilla.w=6*ancho_ventana/8, plantilla.h=8*alto_ventana/10;
    
    caso=-1;
    
    cierre=false, ending=false;
    
    while((!tigrClosed(pantalla)) && (!cierre)) {
        tiempo_total=Calcular_Tiempo();
        tigrMouse(pantalla, &cursor.x, &cursor.y, &click);
        // Vuelve al menu.
        if((tigrKeyHeld(pantalla, TK_ESCAPE)) && (!ending)) {
            ending=true;
            caso=0;
        }
        // Elevacion del cierre.
        if(ending) {
            elevacion_cierre=velocidad_ending*tiempo_total;
            pantalla_creditos.y-=elevacion_cierre;
        }
        // Comprobacion de si la ventana se va ha cerrar.
        if(pantalla_creditos.y<=-pantalla_creditos.h) {
            cierre=true;
        }
        // Limpia el fondo de la pantalla.
        tigrClear(pantalla, tigrRGB(0, 0, 0));
        if(!ending) {
            // Limpia la pantalla de niveles.
            tigrClear(pantalla_creditos.i, tigrRGB(0, 0, 0));
            // Obtiene la posición del cursor y si esta haciendo click.
            // Pinta la plantilla.
            tigrFill(pantalla_creditos.i, plantilla.x, plantilla.y, plantilla.w, plantilla.h, color_azul);
            //poner nombres
                tigrPrint(pantalla_creditos.i, tfont, 4.35*ancho_ventana/16 , 6.85*alto_ventana/16, tigrRGB(255,255,255), "Maciá Valero, Andrés", 0);
                tigrPrint(pantalla_creditos.i, tfont, 4.35*ancho_ventana/16 , 7.85*alto_ventana/16, tigrRGB(255,255,255), "Launov Draganov, Danil", 0);
                tigrPrint(pantalla_creditos.i, tfont, 4.35*ancho_ventana/16 , 8.85*alto_ventana/16, tigrRGB(255,255,255), "Veldemar Volkov,Alexander", 0);

            

            
            tigrBlitAlpha(pantalla_creditos.i, cursor.i, cursor.x, cursor.y, 0, 0, 32, 32, 1);
        }
        // Integracion de la ventana niveles a la pantalla
        tigrBlitAlpha(pantalla, pantalla_creditos.i, pantalla_creditos.x, pantalla_creditos.y, 0, 0, pantalla_creditos.w, pantalla_creditos.h, 1);
        // Refresca la imagen.
        tigrUpdate(pantalla);
    }
    return caso;
}

int main() {
    // Establecimiento de la primera pantalla en abrirse.
    int caso=0;
    // Establecimiento del nivel al que ir.
    int nivel=1;
    // Establecimiento de las dimensiones de la ventana del juego.
    int ancho_ventana=400, alto_ventana=450;
    // Establecimieto de la velocidad de cierre de las diferentes pantallas del juego.
    float velocidad_cierre=800;
    Tigr *pantalla;
    char nombre[11];


    printf("Se esta abriendo la ventana del juego...");
    // Abre la ventana del juego.
    pantalla=tigrWindow(ancho_ventana, alto_ventana, "Pig King", TIGR_NOCURSOR | TIGR_2X);
    printf("Ventana del juego abierta.\n");
    // Inserta el efecto de television de tubos.
    tigrSetPostFX(pantalla, 1, 1, 1, 2.0f);
    // Abre el menu.
    do {
        switch(caso) {
            case 0: caso=menu(pantalla, ancho_ventana, alto_ventana, velocidad_cierre, &nivel);
                    break;
            case 1: caso=poner_nombre(pantalla, ancho_ventana, alto_ventana, nombre, velocidad_cierre);
                    break;
            case 2: caso=juego(pantalla, ancho_ventana, alto_ventana, nombre,velocidad_cierre, &nivel);
                    break;
            case 3: nivel=2;
                    caso=juego(pantalla, ancho_ventana, alto_ventana, nombre,velocidad_cierre, &nivel);
                    break;
            case 4: caso=selector_de_niveles(pantalla, ancho_ventana, alto_ventana, &nivel, velocidad_cierre);
                    break;
            case 5: caso=imprimir_creditos(pantalla, ancho_ventana, alto_ventana, velocidad_cierre);
        }

       
    } while(caso!=-1);
    // Cierre de la ventana del juego.
    tigrFree(pantalla);
    printf("Juego cerrado.\n"); 
    return 0;
}