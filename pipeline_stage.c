#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "eratosthenes.h"
#include <stdio.h>

pid_t make_stage(int n, int read_fd, int **fds) {
    // TODO: Complete
	int *fd = *fds;
	if (pipe(fd) == -1){
		return 1;
	}
	int r = fork();
	if (r > 0){
		if (close(fd[0]) == -1){
			exit(255);
		}
		
		filter(n, read_fd, fd[1]);

		if (close(read_fd) == -1){
			exit(255);
		}
		
		if (close(fd[1]) == -1){
			exit(255);
		}
		return r;
	}
	else if (r == 0){
		if (close(fd[1])==-1){
			exit(255);
		}
		return 0;
	}
	else{
		exit(255);
	}	
	
	return 0;
}
