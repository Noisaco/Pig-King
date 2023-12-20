# PIG KING
ATENCION! Este programa esta diseñado unicamente para que pueda funcionar en sistemas operativos derivados de Linux, como Ubunte y Lubuntu. No se garantiza su correcto funcionamiento en otros sistemas operativos.

Ademas, debido a configuraciones del sistema operativo, a veces se descargaran las carpetas del juego con el nombre comenzando por mayuscula o con todo en mayuscula. Si se intenta ejecutar el programa con las carpetas sin modificar se ejeutara y se cerrara de manera espontanea. Para solucionar esto es necesario renombrar las carpetas en minuscula. Importante que sea el mismo nombre pero en minuscula, si no el programa no detectara de manera correcta las carpetas.
Las carpetas que usualmente requieren renombrase son: la carpeta "imagenes" (normalmente pone la "i" en mayuscula, como "Imagenes") y la carpeta "tigr" (normalmente lo suele poner todo en mayuscula, como "TIGR"). Por ultimo, normalmente la carpeta "personajes" no se modifica, pero por si acaso, es recomendable comprobarlo igualmente.

Ya con las carpetas comprobadas, se debera compilar el programa en la terminar. Para eso deberas abrir la terminal del sitema operativo y acceder a la carpeta del juego. Estando ya en la carpeta, se debera introducir ( gcc ./tigr/tigr.c juego.c -o juego -I./tigr -lGLU -lGL -lX11 -lm ) sin los parentesis y ejecutar el programa con ( ./juego ).

Este juego funcionara de manera tosca y torpe si el ordenador es antiguo. Para mejorar su rendimiento, es recomendable cerrar todas las ventanas en segundo plano que esten consumiendo recursos del ordenador.

Ya en cuanto al contenido del programa, se trata de un juego al estilo Donkey Kong, un juego 2D de vista lateral en la que el objetivo es subir arriba del todo de la torre mientras el enemigo principal te lanza barriles desde arriba. El juego dispone de solo dos niveles, siendo el nivel 2 de estructura igual que el primero pero con agregados que lo hacen mas dicifil.
