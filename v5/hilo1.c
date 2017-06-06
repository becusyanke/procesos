#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

void *sum_num( void * );
void *res_num( void * );
void *mul_num( void * );
void *div_num( void * );

int num1 = 60, num2 = 12;

int main() {
	pthread_t tid_sum, tid_res, tid_mul, tid_div;
	int *sum_res, *res_res, *mul_res, *div_res;

	pthread_create( &tid_sum, NULL, sum_num, NULL );
	pthread_create( &tid_res, NULL, res_num, NULL );
	pthread_create( &tid_mul, NULL, mul_num, NULL );
	pthread_create( &tid_div, NULL, div_num, NULL );

	pthread_join( tid_sum, (void *)&sum_res  );
	pthread_join( tid_res, (void *)&res_res  );
	pthread_join( tid_mul, (void *)&mul_res  );
	pthread_join( tid_div, (void *)&div_res  );

	printf("La suma es: %d\n", *sum_res);
	printf("La resta es: %d\n", *res_res);
	printf("La multiplicacion es: %d\n", *mul_res);
	printf("La division es: %d\n", *div_res);

	return 0;
}

void *sum_num( void *arg ) {
	static int res;

	res = num1 + num2;
	while (1);
	pthread_exit( (void *)&res );
}

void *res_num( void *arg ) {
	int *res = (int *)malloc( sizeof(int) );

	*res = num1 - num2;
	while (1);
	pthread_exit( (void *)res );
}

void *mul_num( void *arg ) {
	static int res;

	res = num1 * num2;
	while (1);
	pthread_exit( (void *)&res );
}

void *div_num( void *arg ) {
	static int res;

	res = num1 / num2;
	while (1);
	pthread_exit( (void *)&res );
}

//Para revisar los hilos utilizar comando:
//ps -eLf | grep hilo1
//ps -eLf nos da la informacion de los procesos
// | grep hilo1 busca en la cadena el texto especificado por grep
