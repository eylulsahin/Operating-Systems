//code taken from recitation 6 slides and modified
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
	
	int fd = open ("loremipsum.txt", O_RDONLY); // Open the file for reading
	struct stat s;
	size_t size;
	int status = fstat(fd, &s);
	size = s.st_size;
    int count = 0; //initialize count of char a to 0
	
	char *ptr = mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	if(ptr == MAP_FAILED){
		printf("Mapping Failed\n");
		return 1;
	}
	close(fd);
    
	if(status != 0){
		printf("Unmapping Failed\n");
		return 1;
	}
	
    for ( size_t i = 0 ; i<size; i++ ){ //traverse lines
		if( ptr[i] == 'a' )
            count++; //increment count when char is a
    }

	printf("In loremipsum.txt, ocurrence of character 'a' is %d", count ); //print count of char a
	return 0;
}
