#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#define N 64

void *busca_mayor( void *d );
void *busca_menor( void *d );
void *ordena_arreglo( void *d );
void *obtener_promedio( void *d );
int *reservar_memoria();
void llenar_arreglo(int *datos);
void imprimir_arreglo(int *datos);

int main() {
	pthread_t tid_may, tid_men, tid_ord, tid_pro;
	int *mayor, *menor, *arreglo_ordenado;
	float *promedio;
	int *datos;

	datos = reservar_memoria();
	llenar_arreglo( datos );
	imprimir_arreglo( datos );

	pthread_create( &tid_may, NULL, busca_mayor, (void *)datos );
	pthread_create( &tid_men, NULL, busca_menor, (void *)datos );
	pthread_create( &tid_ord, NULL, ordena_arreglo, (void *)datos );
	pthread_create( &tid_pro, NULL, obtener_promedio, (void *)datos );

	pthread_join( tid_may, (void *)&mayor  );
	pthread_join( tid_men, (void *)&menor );
	pthread_join( tid_ord, (void *)&arreglo_ordenado );
	pthread_join( tid_pro, (void *)&promedio );

	printf("El numero mayor es: %d\n\n", *mayor);
	printf("El numero menor es: %d\n\n", *menor);

	register int i;

	printf("Los datos ordenados son: \n");

	for ( i = 0; i < N; i++ ) {
		printf("%d ", arreglo_ordenado[i]);
		if ( i % 8 == 7 ) printf("\n");
	}

	printf("El promedio es: %f\n\n", *promedio);

	return 0;
}

int *reservar_memoria() {
	int *datos = (int *)malloc( N * sizeof(int) );

	return datos;
}

void llenar_arreglo(int *datos) {
	register int i;

	for ( i = 0; i< N; i++ ) {
		datos[i] = rand() % 256;
	}

}

void imprimir_arreglo(int *datos) {
	register int i;

	for ( i = 0; i< N; i++ ) {
		printf("%d ", datos[i]);
		if ( i%8 == 7  ) printf("\n");
	}

}


void *busca_mayor( void *d ) {
	register int i;
	int *datos = (int *)d;
	static int mayor;
	mayor = datos[0];

	for ( i = 1; i < N; i++ ) {
		if ( mayor < datos [i] ) mayor = datos[i];
	}

	pthread_exit( (void *)&mayor );
}

void *busca_menor( void *d ) {
	register int i;
	int *datos = (int *)d;
	static int menor;
	menor = datos[0];

	for ( i = 1; i < N; i++ ) {
		if ( menor > datos [i] ) menor = datos[i];
	}

	pthread_exit( (void *)&menor );
}

void *ordena_arreglo( void *d ) {
	register int i, j;
	int *datos = (int *)d;
	int *datos_ordenados = (int *)malloc(N * sizeof(int));

	for ( i = 0; i < N; i++ ) {
		datos_ordenados[i] = datos[i];
	}

	int aux;

	for ( i = 0; i < N; i++ ) {
		for ( j = i+1; j < N; j++) {
			if ( datos_ordenados[i] > datos_ordenados[j] ) {
				aux = datos_ordenados[i];
				datos_ordenados[i] = datos_ordenados[j];
				datos_ordenados[j] = aux;
			}
		}
	}

	pthread_exit( (void *)datos_ordenados );

}

void *obtener_promedio( void *d ) {
	register int i;
	int *datos = (int *)d;
	static float promedio;
	promedio = 0;

	for ( i = 0; i < N; i++) {
		promedio = promedio + datos[i];
	}

	promedio = promedio / N;

	pthread_exit( (void *)&promedio );
}

//Agregar hilo para ordenar y otro para promedio
