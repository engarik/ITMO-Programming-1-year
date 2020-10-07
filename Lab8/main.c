/* Kornilov Nikita, M3102, 5.10.2020, lab 8, var. 8 */

#include <stdio.h>
#include <string.h>

int main(void) {
	
	char string1[30];
    char string2[30];
    printf("Input first string: ");
    gets(string1);
    printf("Input second string: ");
    gets(string2);
    
    char tmp[30];
    strcpy(tmp, string1);
    printf("Concatenation: %s\n", strcat(tmp, string2));
    
    int cmpResult = strcmp(string1, string2);
    if (cmpResult != 0) {
    	printf("%s string is bigger\n", cmpResult > 0 ? "First" : "Second");
	}
    else {
    	printf("Strings are identical\n");
	}
    	
    	
    char c;
    printf("Input character: ");
    scanf("%c", &c);
    
    char* charPos = strchr(string1, c);
    if (charPos == NULL) {
    	printf("No such character in first string\n");
	}
    else {
    	printf("Pos of char in first string: %d\n", charPos - string1);
	}
    	
    charPos = strchr(string2, c);
    if (charPos == NULL) {
    	printf("No such character in second string\n");
	}
    else {
    	printf("Pos of char in second string: %d\n", charPos - string2);
	}
    	
    	
    	
    char* stringPos = strstr(string1, string2);
    if (stringPos == NULL) {
    	printf("No second string in first\n");
	}
    else {
    	printf("Pos of second string in first: %d\n", stringPos - string1);
	}


	int i, mlen = 0, clen = 0;
	for (i = 0; i < strlen(string1); i++) {
		char* charPos = strchr(string2, string1[i]);
		if (charPos == NULL) {
			clen++;
		}
		else {
			if (clen > mlen) {
				mlen = clen;
			}
			clen = 0;
		}
		if (i == strlen(string1) - 1 && clen > mlen) {
				mlen = clen;
		}
	}
	printf("Max length of fs segment without second is: %d\n", mlen);
	
	return 0;
}
