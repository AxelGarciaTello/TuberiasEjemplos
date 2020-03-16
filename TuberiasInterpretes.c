/***
Forma de uso: $ tuberia programa_1 programa_2
Esto es equivalente a: $ programa_1 | programa_2
***/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	int tuberia[2];
	int pid;
	if(argc < 3){
		fprintf(stderr,
			"Formas de uso: %s programa_1 programa_2\n",
			argv[0]);
		exit(-1);
	}
	/*Creación de la tubería*/
	if(pipe(tuberia) == -1){
		perror(argv[0]);
		exit(-1);
	}
	/*Creación de los procesos padre e hijo*/
	if((pid=fork()) == -1){
		perror(argv[0]);
		exit(-1);
	}
	else if(pid == 0){
		/*Código del proceso hijo.*/
		close(0);
		dup(tuberia[0]);
		/*Al duplicar el descriptor asociado a tuberia[0], lo
		vamos a hacer sobre la primera entrada que haya libre
		en la tabla de descripciones de ficheros. Esta entrada
		es la número 0, ya que antes hemos cerrado el fichero de
		entrada (descriptor 0). Así, para el proceso hijo, el
		fichero estándar de entrada será el fichero asociado a
		la lectura de la tubería, y todas las funciones que
		trabajen con el fichero estándar de entrada en realidad
		van a trabajar con el fichero de lectura de la tubería.*/
		close(tuberia[0]);
		close(tuberia[1]);
		/*Ejecución de programa_2*/
		execlp(argv[2], argv[2], NULL);
		/*En la llamada a exec se heredan los descriptores
		asignados, por lo que el programa referenciado en argv[2]
		se ejecutará teniendo al fichero de lectura de la tubería
		como fichero estándar de entrada*/
	}
	else{
		/*Código del proceso padre.*/
		close(1);
		dup(tuberia[1]);
		close(tuberia[0]);
		close(tuberia[1]);
		/*Ejecución de programa_1*/
		execlp(argv[1], argv[1], NULL);
	}
	exit(0);
}