#include <stdio.h>

#include "eratosthenes.h"

int filter(int n, int readfd, int writefd) {
    // TODO: Complete
	int div;
	while(read(readfd, &div, sizeof(int))>0){
		
		if (div % n != 0){
			if (write(writefd, &div, sizeof(int)) == -1){
				return 1;
			}
		}
	}
	return 0;
}

