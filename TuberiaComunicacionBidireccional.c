/***
	Programa para ilustrar el envío de mensajes entre un proceso
	emisor y otro receptor a través de dos tuberías sin nombre.
	Estas tuberías permitirán implementar una comunicación 
	bidiraccional. El proceso emisor pedirá un mensaje que le
	enviará al proceso receptor. Cuando el proceso receptor haya
	presentado el mensaje por pantalla, solicitará al proceso 
	emisor otro mensaje, indicándole así que está listo y que 
	puede pedirle otro mensaje al usuario.
***/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define MAX 256

int main(void){
	int tuberia_em_re[2];
	int tuberia_re_em[2];
	int pid;
	char mensaje [MAX];

	/*Creación de las tuberías de comunicación*/
	if(pipe(tuberia_em_re) == -1 || pipe(tuberia_re_em) == -1){
		perror("pipe");
		exit(-1);
	}

	/*Creación del proceso hijo.*/
	if((pid=fork()) == -1){
		perror("fork");
		exit(-1);
	}
	else if (pid == 0){
		/*Código del proceso hijo.*/
		/*El proceso hijo (receptor) se encarga de leer un 
		mensaje de la tubería y presentarlo en la pantalla. Al 
		recibir el mensaje "FIN\n", termina el proceso.*/
		while(read(tuberia_em_re[0], mensaje, MAX) > 0 &&
			strcmp (mensaje, "FIN\n") != 0){
			printf("proceso RECEPTOR. mensaje: %s\n", mensaje);
			/*Enviamos al proceso emisor un mensaje para indicar
			que estamos listos para recibir otro mensaje.*/
			strcpy(mensaje,"LISTO");
			write(tuberia_re_em[1], mensaje, strlen(mensaje)+1);
		}
		close(tuberia_em_re[0]);
		close(tuberia_em_re[1]);
		close(tuberia_re_em[0]);
		close(tuberia_re_em[1]);
		exit(0);
	}
	else{
		/*Código del proceso padre.*/
		/*El proceso padre (emisor) se encarga de leer un mensaje
		de la entrada estándar y acto seguido escribirlo en la
		tubería, para que lo reciba el proceso hijo. Al escribir
		el mensaje "FIN\n" acaban los dos procesos*/
		while(printf("PROCESO EMISOR. MENSAJE: ") != 0 &&
			fgets(mensaje, sizeof(mensaje), stdin) != NULL &&
			write(tuberia_em_re[1],mensaje, 
				strlen(mensaje)+1) > 0 && 
			strcmp(mensaje, "FIN\n") != 0){
			do{
				read(tuberia_re_em[0], mensaje, MAX);
			} while(strcmp(mensaje, "LISTO") != 0);
		}
		close(tuberia_em_re[0]);
		close(tuberia_em_re[1]);
		close(tuberia_re_em[0]);
		close(tuberia_re_em[1]);
		exit(0);
	}
}