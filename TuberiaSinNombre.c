#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX 256
int main(void){
	int tuberia[2];
	int pid;
	char mensaje [MAX];

	/*Creación de la tubería sin nombre*/
	if(pipe(tuberia) == -1){
		perror("pipe");
		exit(-1);
	}

	/*Creación del proceso hijo*/
	if ((pid=fork()) == -1){
		perror("fork");
		exit(-1);
	}
	else if(pid == 0){
		/*Código del proceso hijo*/
		/*El proceso hijo (receptor) se encarga de leer mensajes
		de la tubería y presentarlos en pantalla. El ciclo de
		lectura y presentación termina al leer el mensaje "FIN\n"
		*/
		while (read(tuberia[0], mensaje, MAX) > 0 &&
			strcmp(mensaje, "FIN\n") != 0){
			printf("PROCESO RECEPTOR. MENSAJE: %s\n", mensaje);
		}
		close(tuberia[0]);
		close(tuberia[1]);
		exit(0);
	}
	else{
		/*Código del proceso padre.*/
		/*El proceso padre (emisor) se encarga de leer mensajes
		de la entrada estándar y, acto seguido, escribirlos en la
		tubería para que los reciba el proceso hijo. El ciclo de
		lectura de la entrada estándar y escritura en la tubería
		terminará cuando se introduzca el mensaje "FIN\n".*/
		while(printf("PROCESO EMISOR. MENSAJE: ") != 0 &&
			fgets(mensaje,sizeof(mensaje), stdin) != NULL &&
			write(tuberia[1], mensaje, strlen(mensaje)+1) > 0 &&
			strcmp(mensaje, "FIN\n") !=0);
		close(tuberia[0]);
		close(tuberia[1]);
		exit(0);
	}
}