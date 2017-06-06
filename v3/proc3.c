#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 2
#define N 16

void proceso_hijo( int np, int *datos );
void proceso_padre( void );
int * reservar_memoria( void );
void llenar_arreglo(int *datos);
void imprimir_arreglo( int *datos );
int buscar_mayor( int *datos );
int buscar_menor( int *datos );

int main() {
	int *datos;
	register int np;
	pid_t pid;

	datos = reservar_memoria();
	llenar_arreglo( datos );
	imprimir_arreglo( datos );

	for ( np = 0; np < NUM_PROC; np++) {
		pid = fork();

		if ( pid == -1) {
			perror("Error al generar el proceso");
			exit (EXIT_FAILURE);
		} else if ( !pid ) {
			proceso_hijo( np, datos );
		}
	}

	proceso_padre();
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


void proceso_hijo( int np, int *datos ) {
	int mayor, menor;

	printf("Proceso %d con pid %d ejecutado\n", np, getpid() );

	if ( np == 0 ) {
		mayor = buscar_mayor( datos );
		exit( mayor );
	}
	else if ( np == 1 ) {
		menor = buscar_menor( datos );
		exit( menor );
	}
}

void proceso_padre( void ) {
	register int i;
	int numero;
	pid_t pid;

	for (i = 0; i < NUM_PROC; i++) {
		pid = wait( &numero );
		printf("Proceso con pid %d terminado con numero %d\n", pid, numero >> 8);
	}
//	while ( 1 );
}
