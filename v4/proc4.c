#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 2
#define N 16

void proceso_hijo( int np, int *datos, int pipefd[] );
void proceso_padre( int pipefd[] );
int * reservar_memoria( void );
void llenar_arreglo(int *datos);
void imprimir_arreglo( int *datos );
int buscar_mayor( int *datos );
int buscar_menor( int *datos );

int main() {
	int *datos;
	register int np;
	pid_t pid;
	int pipefd[2];

	datos = reservar_memoria();
	llenar_arreglo( datos );
	imprimir_arreglo( datos );

	pipe( pipefd );

	for ( np = 0; np < NUM_PROC; np++) {
		pid = fork();

		if ( pid == -1) {
			perror("Error al generar el proceso");
			exit (EXIT_FAILURE);
		} else if ( !pid ) {
			proceso_hijo( np, datos, pipefd );
		}
	}

	proceso_padre( pipefd  );
	free ( datos );

	return 0;
}

int * reservar_memoria( void ) {
	int *dato;
	dato = (int *)malloc(N * sizeof(int) );
	llenar_arreglo(dato);

	if ( !dato ) {
		perror("Error en la asignacion de memoria");
		exit(EXIT_FAILURE);
	}

	return dato;
}


void llenar_arreglo( int *datos ) {
	register int i;

	for (i = 0; i < N; i++) {
		datos[i] = rand() % 256;
	}
}

void imprimir_arreglo( int *datos ) {
	register int i;

	for (i = 0; i < N; i++) {
		printf("%d ", datos[i]);
		if ( ! ((i+1) % 8)  ) {
			printf("\n");
		}
	}
}

int buscar_mayor( int *datos ) {
	register int i;
	int mayor = datos[0];

	for (i = 1; i < N; i++) {
		if ( datos[i] > mayor ) mayor = datos[i];
	}

	return mayor;
}

int buscar_menor( int *datos ) {
	register int i;
	int menor = datos[0];

	for (i = 1; i < N; i++) {
		if ( datos[i] < menor ) menor = datos[i];
	}

	return menor;
}


void proceso_hijo( int np, int *datos, int pipefd[] ) {
	int mayor, menor;

	printf("Proceso %d con pid %d ejecutado\n", np, getpid() );
	close( pipefd[0] );

	if ( np == 0 ) {
		mayor = buscar_mayor( datos );
		write( pipefd[1], &mayor, sizeof(int) );
		exit( np );
	}
	else if ( np == 1 ) {
		menor = buscar_menor( datos );
		write( pipefd[1], &menor, sizeof(int) );
		exit( np );
	}
}

void proceso_padre( int pipefd[] ) {
	register int i;
	int numero, np;
	pid_t pid;

	close( pipefd[1] );

	for (i = 0; i < NUM_PROC; i++) {
		read( pipefd[0], &numero, sizeof(int) );
		pid = wait( &np );
		printf("Proceso con pid %d \n", pid);

		if ( np == 0 ) printf("El numero mayor es: %d\n", numero);
		else printf("El numero menor es: %d\n", numero);

	}
}

//Ordenar elementos del arreglo y obtener el promedio de los datos
