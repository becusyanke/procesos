#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 4
#define N 16

void proceso_hijo( int np, int *datos, int pipefd[] );
void proceso_padre( int pipefd[] );
int * reservar_memoria( void );
void llenar_arreglo(int *datos);
void imprimir_arreglo( int *datos );
int buscar_mayor( int *datos );
int buscar_menor( int *datos );
void ordena_arreglo( int *datos );
float obtener_promedio( int *datos );

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

void ordena_arreglo( int *datos ) {
	register int i, j, aux;

	for ( i = 0; i < N; i++ ) {
		for ( j = i+1; j < N; j++ ) {
			if ( datos[i] > datos[j] ) {
			aux = datos[i];
			datos[i] = datos[j];
			datos[j] = aux;
			}
		}
	}
}

float obtener_promedio( int *datos ) {
	register int i;
	float promedio = 0;

	for ( i = 0; i < N; i++ ) {
		promedio = promedio + datos[i];
	}

	return promedio / N;
}

void proceso_hijo( int np, int *datos, int pipefd[] ) {
	int mayor, menor;
	float promedio;

	printf("Proceso %d con pid %d ejecutado \n", np, getpid() );
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
	else if ( np == 2 ) {
		ordena_arreglo( datos );
		write( pipefd[1], datos, N*sizeof(int) );
		exit(np);
	}
	else if ( np == 3 ) {
		promedio = obtener_promedio( datos );
		write( pipefd[1], &promedio, sizeof(float) );
		exit(np);
	}
}

void proceso_padre( int pipefd[] ) {
	register int i, j;
	int numero, np, *datos;
	float promedio;
	datos = reservar_memoria();
	pid_t pid;

	close( pipefd[1] );

	for (i = 0; i < NUM_PROC; i++) {

		pid = wait( &np );
		np = np >> 8;
		printf("Proceso con pid %d \n", pid);

		if ( np == 0 ) {
			read( pipefd[0], &numero, sizeof(int) );
			printf("El numero mayor es: %d\n", numero);
		}
		else if ( np == 1 ) {
			read( pipefd[0], &numero, sizeof(int) );
			printf("El numero menor es: %d\n", numero);
		}
		else if ( np == 2 ) {
			read( pipefd[0], datos, N*sizeof(int) );
			printf("Arreglo ordenado...\n");

			for ( j = 0; j < N; j++ ) {
				printf("%d ", datos[j]);
				if ( ! ((j+1) % 8)  ) {
				printf("\n");
				}
			}

			free( datos );
		}
		else if ( np  == 3 ) {
			read( pipefd[0], &promedio, sizeof(float) );
			printf("El promedio es: %f \n", promedio);
		}

	}
}

//Ordenar elementos del arreglo y obtener el promedio de los datos
