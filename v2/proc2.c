#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROC 8

void proceso_hijo( int np );
void proceso_padre( void );


int main() {
	register int np;
	pid_t pid;

	for ( np = 0; np < NUM_PROC; np++) {
		pid = fork();

		if ( pid == -1) {
			perror("Error al generar el proceso");
			exit (EXIT_FAILURE);
		} else if ( !pid ) {
			proceso_hijo( np );
		}
	}

	proceso_padre();

	return 0;
}

void proceso_hijo( int np ) {
	printf("Proceso %d con pid %d ejecutado\n", np, getpid() );
	exit( 0 );
}


void proceso_padre( void ) {
	register int i;
	int status;
	pid_t pid;

	for (i = 0; i < NUM_PROC; i++) {
		pid = wait( &status );
		printf("Proceso con pid %d terminado\n", pid);
	}
//	while ( 1 );
}
