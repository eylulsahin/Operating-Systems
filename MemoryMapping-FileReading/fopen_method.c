//code taken from recitation 6 slides and modified
#include <stdio.h>
#include <stdlib.h>
//using fopen to open & read file
int main() {
	
	char file_name[25] = "loremipsum.txt";
	char buffer[40]; //larger buffer is initialized
    FILE *fp;
	fp = fopen(file_name, "r"); // read mode
    int count = 0; //initialize count of char a to 0
    
	while( fgets(buffer,30,(FILE*)fp ) ){
        for(int i = 0; buffer[i]!=0; i ++){ //traverse buffer
			if(buffer[i] == 'a')
                count++; //increment count when char is a
        }
    }
        
	fclose(fp); //close file
	printf("In loremipsum.txt, ocurrence of character 'a' is %d", count ); //print count of char a
	return 0;
}
