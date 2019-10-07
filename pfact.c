#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>

#include "eratosthenes.h"



void check_final_factor(int j, int n, int *factor1, int *factor2, int fd, int lst_primes[], int size){
	if (j == sqrt(n)){
		printf("%d %d %d\n", n, j, j);
		exit(0);
	}
	if (*factor1 == 0 && *factor2 == 0){
		printf("%d is prime\n", n);
		exit(0);
	}
	
	if (*factor1 * *factor2 == n){
		printf("%d %d %d\n", n, *factor1, *factor2);
                exit(0);
        }
	int x;
	while(read(fd, &x, sizeof(int))>0){
		for (int i = 0; i < size+1; i++){
			if (lst_primes[i] * x == n){
				printf("%d %d %d\n", n, lst_primes[i], x);
				exit(0);
			}
		}
	}
	
	printf("%d is not the product of two primes\n", n);
	exit(0);
}
void check_factor(int j, int n, int *factor1, int *factor2, int lst_primes[], int size){
	if (n == j){
		printf("%d is prime\n", n);
		exit(0);
	}
	if (*factor1 == 0 && *factor1 == *factor2){
		*factor1 = j;
	}
	else if (*factor1 != 0 && *factor2 == 0){
		*factor2 = j;
	}
	else if (*factor1 == 0 && *factor2 != 0){
		*factor1 = j;
	}
	else if (*factor1 * *factor2 == n){
		printf("%d %d %d\n", n, *factor1, *factor2);
		exit(0);
	}
	else{
		printf("%d is not the product of two primes\n", n);
		exit(0);
	}
	if (n == j * j){
		printf("%d %d %d\n", n, j, j);
		exit(0);
	}
	if (n % j == 0 && (n / j) % j == 0){
		printf("%d is not the product of two primes\n", n);
		exit(0);
	}
	if (*factor1 * *factor2 == n){
		printf("%d %d %d\n", n, *factor1, *factor2);
		exit(0);
	}
	
	int temp;
	for(int i = 0; i < size; i++){
		temp = n / lst_primes[i];
		if (temp % j == 0){
			printf("%d is not the product of two primes\n", n);
			exit(0);
		}
	}
}
int main(int argc, char *argv[]) {
    	if (argc == 2){
		for(int i = 0; i < strlen(argv[1]); i++){
			if(argv[1][i] >= '0' && argv[1][i] <= '9'){}
			else{
				fprintf(stderr, "Usage:\n\tpfact n\n");
				return 1;
			}
		}
	}else{
		fprintf(stderr, "Usage:\n\tpfact n\n");
		return 1;
	}
	
        
	int n = strtol(argv[1], NULL, 10);
	if (n < 2){
		 fprintf(stderr, "Usage:\n\tpfact n\n");
                return 1;

	}
	// Turning off sigpipe
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
		perror("signal");
		exit(1);
	}
	int j = 0;
	int result = 0;
	int arr[n][2];
	int status = 0;
	int *fds = arr[0];
	int fd[2];
	int factor1 = 0;
	int factor2 = 0;
	int lst_primes[n];
	if (pipe(fd) == -1){
		return 2;
	}
	int r = fork();
	if (r < 0){
		return 2;
	}
	else if (r > 0){

		if (close(fd[0]) == -1){
			return 2;
		}
		for(int i = 2; i < n+1; i++){				
			if (write(fd[1], &i, sizeof(int)) == -1){
				return 2;
			}	
        	}
		if (close(fd[1]) == -1){
			return 2;
		}
		
		wait(&status);
		if (WIFEXITED(status)){
			result = WEXITSTATUS(status);
		}
		if (result == 255){
			return 2;
		}	
		printf("Number of filters = %d\n", result);	
		exit(0);
		return 0;
	}else{
		int pid;
		int i = 0;
		int k = 0;
		if (close(fd[1]) == -1){
			return 2;
		}
		while(read(fd[0], &j, sizeof(int)) > 0){
	                if (n % j == 0){
                                lst_primes[k] = j;
				k +=1;
				check_factor(j, n, &factor1, &factor2, lst_primes, k);
                        }

			if (j >= sqrt(n)){
				check_final_factor(j, n, &factor1, &factor2, fd[0], lst_primes, k);
			}
					
			if ((pid = make_stage(j, fd[0], &fds)) == 255){
				return 2;
			}	
			if (pid == 0){
				
				if (close(fd[0]) == -1){
					return 2;
				}
				fd[0] = fds[0];
			}
			i+=1;
			fds = arr[i];
			if (pid > 0){
				wait(&status);
				if (WIFEXITED(status)){
					if (WEXITSTATUS(status) == 255){
						exit(255);
					}
					exit(WEXITSTATUS(status)+1);	
				}
			}
		}
	}
    // Your solution below ...
    return 0;
}
