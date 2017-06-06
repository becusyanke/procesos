#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	float num1 = 45, num2 = 13, suma = 0, resta = 0;
	pid_t pid, pidw;
	int status;


	pid = fork();

	if ( !pid ) {
		printf("Soy el proceso hijo con pid %d\n", getpid() );
		suma = num1 + num2;
		printf("La suma es: %f\n", suma);
		exit(0);
//		while (1);
	}
	else {
	//	sleep(10);
		printf("Soy el proceso padre con pid %d\n", getpid() );
		resta = num1 - num2;
		printf("La resta es: %f\n", resta);
		pidw = wait(&status);
		printf("Terminando el proceso %d\n", pidw);
//		while (1);
	}

	return 0;
}
