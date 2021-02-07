/* Kornilov Nikita, M3102, 16.10.2020, lab 12, var. 8 */

#include <stdio.h>

void copyFile(FILE *from, FILE *to) {
	unsigned char c;
	
	while (fread(&c, sizeof(unsigned char), 1, from) == 1) {
		fwrite(&c, sizeof(unsigned char), 1, to);
	}
}

int main(int argc, char *argv[]) {
	FILE *file1, *file2, *file3;
	
	if (argc != 4) {
		printf("Need 3 arguments, got %i\nPress any key to exit...", argc - 1);
		getchar();
		return -1;
	}
	
	file1 = fopen(argv[1], "r");
	if (file1 == NULL) {
	    printf("Can't open file %s\nPress any key to exit...", argv[1]);
	    getchar();
	    return -1;
    }
    
	file2 = fopen(argv[2], "r");
	if (file2 == NULL) {
	    printf("Can't open file %s\nPress any key to exit...", argv[2]);
	    fclose(file1);
	    getchar();
	    return -1;
    }
	
	file3 = fopen(argv[3], "w");
	if (file3 == NULL) {
	    printf("Can't open file %s\nPress any key to exit...", argv[3]);
	    fclose(file1);
	    fclose(file2);
	    getchar();
	    return -1;
    }
	
	copyFile(file1, file3);
	copyFile(file2, file3);
	printf("Successfully copied files"); 
	
	fclose(file1);
	fclose(file2);
	fclose(file3); 
    
	return 0;
}
