#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(1)
#define SIZE 40

/*
Supported version ID3v2.3
Structure:
	fileHeader:
		ID3 marker 		-	3 bytes
		ID3 version 	- 	1 byte
		ID3 sub. ver.	-	1 byte
		ID3 flags		-	1 byte
		ID3 size 		-	4 bytes
		ID3 total		-		10 bytes
	frameHeader:
		frame id		-	4 bytes
		frame size 		-	4 bytes
		frame flags		-	2 bytes
		frame total		-		10 bytes

*/

void throwError(char errorText[]) {
	printf("Error: %s\n", errorText);
	exit(1);
}

unsigned int convertEndians(unsigned int val) { // Because of the processor arch need to convert in LittleEndian
    return ((val >> 24) 	& 	0x0000007F) 	| 	// byte 1
           ((val >> 8)		&	0x00007F00) 	|	// byte 2
           ((val << 8) 		& 	0x007F0000) 	|	// byte 3
           ((val << 24)		&	0x7F000000)		;	// byte 4
}

unsigned int fromMP3Size(unsigned int val) { // Only 7 bits in every byte are important
    return ((val) 			& 	0x0000007F) 	| 	// byte 1
           ((val >> 8) << 7	&	0x00003FA0) 	|	// byte 2
           ((val >> 16)<< 14& 	0x001FB000) 	|	// byte 3
           ((val >> 24)<< 21&	0x0FE00000)		;	// byte 4
}

unsigned int toMP3Size(unsigned int val) { // Back to 7 bits in byte
	return ((val) 			& 	0x0000007F) 	| 	// byte 1
           ((val << 1)		&	0x00007F00) 	|	// byte 2
           ((val << 2) 		& 	0x007F0000) 	|	// byte 3
           ((val << 3)		&	0x7F000000)		;	// byte 4
}

// Copy file to end
void copyFileE(FILE *from, FILE *to,unsigned int offsetFrom, unsigned int offsetTo) { 
	unsigned char c;
	fseek(from, offsetFrom, SEEK_SET);
	fseek(to, offsetTo, SEEK_SET);
	
	while (fread(&c, sizeof(unsigned char), 1, from) == 1) {
		fwrite(&c, sizeof(unsigned char), 1, to);
	}
}

// Copy part of file
void copyFileP(FILE *from, FILE *to,unsigned int offsetFrom, unsigned int offsetTo, int lenght) {
	unsigned char c;
	fseek(from, offsetFrom, SEEK_SET);
	fseek(to, offsetTo, SEEK_SET);
	
	while (fread(&c, sizeof(unsigned char), 1, from) == 1 && lenght > 0) {
		fwrite(&c, sizeof(unsigned char), 1, to);
		lenght--;
	}
}

union fileHeader {
    unsigned char buffer[10];

    struct {
        unsigned char marker[3];
        unsigned char version;
        unsigned char subVersion;
        unsigned char flags;
        unsigned int size;
    } data;
};

union frameHeader {
    unsigned char buffer[10];

    struct {
        unsigned char id[4];
        unsigned int size;
        unsigned short flags;
    } data;
};

// Add \0 to make char array a string
unsigned char* toString(char* notString, int length) {
	unsigned char* string = malloc(length * sizeof(char));
	memcpy(string, notString, length);
	string[length] = '\0';	
	return string;
}

// Parse frame data to string
char* parseFrameContents(char* frameContents, int length) {
	unsigned char* string;
	
	if (frameContents[0] == 1) { // UTF-16 LE decode
		int offset, i;
		if (frameContents[1] == 101) { // Check for COMM tag
			length = (length - 10) / 2;
			offset = 10;
		} else {
			length = (length - 3) / 2;
			offset = 3;
		}
		string = malloc((length) * sizeof(char));
		for (i = 0; i < length; i++) {
			string[i] = frameContents[2 * i + offset];
		}
		return string;
	} else if (frameContents[0] == 0) { // ISO-8859-1 decode for numbers and url
		string = malloc((length - 2) * sizeof(char));
		strncpy(string, frameContents + 1, length - 2);
		return toString(string, length - 2);
	} else {
		return toString(frameContents, length); // Something unkown
	}
}

// Parse string back to frame data
char* stringToTag(char* tagName, char* string, unsigned int* frameSizeNew) {
	unsigned int stringLength = strlen(string) + 1, isNumber = 1, i = 0;
	unsigned char* tag;
	
	while (string[i] != 0) {
		isNumber = isNumber && string[i] < 58 && string[i] > 47 ? 1 : 0; // Check if prop_value is number
		i++;
	}
	
	if (!isNumber) { // Encoding to UTF-16LE
		int offset;
		
		if (!strcmp(tag, "COMM")) { // Special prefix for COMM tag
			char COMMPrefix[10] = {1, 101, 110, 103, 255, 254, 0, 0, 255, 254};
			*frameSizeNew = 10 + 2 * stringLength;
			offset = 10;
			tag = calloc(*frameSizeNew, sizeof(char));
			memcpy(tag, COMMPrefix, 10);
		} else { // Others are just UTF-16LE prefixies
			char UTF16LEPrefix[3] = {1, 255, 254};
			*frameSizeNew = 2 * stringLength + 3;
			offset = 3;
			tag = calloc(*frameSizeNew, sizeof(char));
			memcpy(tag, UTF16LEPrefix, 3);
		}
		for (i = 0; i < stringLength;i++) { 
			tag[offset + 2 * i] = string[i];
		}
		return tag;
	} else { // ISO-8859-1 encode for numbers
		tag = calloc(strlen(string) + 2, sizeof(char));
		*frameSizeNew = strlen(string) + 2;
		strncpy(tag + 1, string, strlen(string));
		return tag;
	}
}

void show(char* fileName) {
    FILE* file = fopen(fileName, "rb"); // Open file for read in binary
    
    if (file == NULL) {
    	perror("Error while opening file"); // Check for success open
    	exit(0);
	}

    fseek(file, 0, SEEK_SET); // Search for the begining of the file

    union fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file); // Reading file header
    
    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size)); // Reading the lenght of tag size
	
    printf(
		"%sv2.%d(%d) Tag size:%d bytes\n", toString(fileHeader.data.marker, 3), fileHeader.data.version, fileHeader.data.subVersion, tagSize
	); // Printing file header
	
	if (fileHeader.data.version != 3) { // Check for supported verison
		throwError("Unsupported version of ID3. Only ID3v2.3 is supported");
	}


    while (ftell(file) < tagSize + 10) { // Reading all the tag data
        union frameHeader frameHeader;
        fread(frameHeader.buffer, sizeof(char), 10, file); // Reading header of frame

        unsigned int frameSize = convertEndians(frameHeader.data.size); // Getting frame length
        
        if (frameSize == 0) { // Chekc for empty frame
        	break;
		}

        unsigned char* frameContents = malloc(frameSize * sizeof(char)); // Reading frame data
        fread(frameContents, sizeof(char), frameSize, file);
        	        
		// Printing frame name and it's data
		printf("%s: %s \n", toString(frameHeader.data.id, 4), parseFrameContents(frameContents, frameSize));
	
        free(frameContents); // free memory
    }
    fclose(file); // Closing file
}

void get(char* fileName, char* tagName) {
	FILE* file = fopen(fileName, "rb"); // Open file for read in binary
    
    if (file == NULL) {
    	perror("Error while opening file"); // Check for success open
    	exit(0);
	}
	
	fseek(file, 0, SEEK_SET); // Search for the begining of the file

    union fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file); // Reading file header

    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size)); // Reading the lenght of tag size
	
    printf(
		"%sv2.%d(%d) Tag size:%d\n", toString(fileHeader.data.marker, 3), fileHeader.data.version, fileHeader.data.subVersion, tagSize
	); // Printing file header
	
	if (fileHeader.data.version != 3) { // Check for supported verison
		throwError("Unsupported version of ID3. Only ID3v2.3 is supported");
	}
	
	int searchFlag = 1;
	while (ftell(file) < tagSize + 10 && searchFlag) { // Reading frame data while looking for tag till EOF or if found
        union frameHeader frameHeader;
        
        fread(frameHeader.buffer, sizeof(char), 10, file); // Reading header of frame
        
        unsigned int frameSize = convertEndians(frameHeader.data.size); // Getting frame length
        
        if (frameSize == 0) { // Chekc for empty frame
	        	break;
			}
			
        if (!strcmp(tagName,toString(frameHeader.data.id, 4))) { // If found tag
        	searchFlag = 0;
        	
			unsigned char* frameContents = malloc(frameSize * sizeof(char)); // Reading frame data
			
	        fread(frameContents, sizeof(char), frameSize, file);
	        
			// Printing frame name and it's data
			printf("%s: %s\n", toString(frameHeader.data.id, 4), parseFrameContents(frameContents, frameSize));
			free(frameContents);
			break;
		}
		fseek(file, frameSize, SEEK_CUR); // go to next frame
    }
    if (searchFlag) { // Tag wasn't found
    	printf("No such tag\n");
	}
    fclose(file); // Closing file
	
}

void set(char* fileName, char* propName, char* propValue) {
	printf("BEFORE MODIFICAITON\n\n");
	show(fileName);
	printf("\n\nMODIFICAITON\n\n");
	FILE* file = fopen(fileName, "rb"); // Open file for read in binary
    
    if (file == NULL) {
    	perror("Error while opening file"); // Check for success open
    	exit(0);
	}

    fseek(file, 0, SEEK_SET); // Search for the begining of the file

    union fileHeader fileHeader;
    fread(fileHeader.buffer, sizeof(char), 10, file); // Reading file header

    unsigned int tagSize = fromMP3Size(convertEndians(fileHeader.data.size)); // Reading the lenght of tag size
	
    printf(
		"%sv2.%d(%d) Tag size:%d bytes\n", toString(fileHeader.data.marker, 3), fileHeader.data.version, fileHeader.data.subVersion, tagSize
	); // Printing file header
	
	if (fileHeader.data.version != 3) { // Check for supported verison
		throwError("Unsupported version of ID3. Only ID3v2.3 is supported");
	}
	
	int searchFlag = 1;
 	unsigned int frameSize;
    unsigned int frameSizeNew;
    unsigned int frameChangePos;
    unsigned char* newTag;
	
	
	while (ftell(file) < tagSize + 10 && searchFlag) { // Reading tag data while looking for tag till EOF
        union frameHeader frameHeader;
        fread(frameHeader.buffer, sizeof(char), 10, file); // Reading header of tag frame
        frameSize = convertEndians(frameHeader.data.size); // Getting frame length
        if (frameSize == 0) { // Chekc for empty frame
	        	break;
			}
        if (!strcmp(propName, toString(frameHeader.data.id, 4))) { // If found fileTag
        	searchFlag = 0;
        	newTag = stringToTag(propName, propValue, &frameSizeNew);
        	frameChangePos = ftell(file) - 10;
        	//printf("pos:%d\n", frameChangePos);
			break;
		}
		fseek(file, frameSize, SEEK_CUR); // go to next frame
    }
    
    if (searchFlag) { // Tag wasn't found
    	printf("No such tag. Editing allowed on for existing tags\n");
	} else { // Write it in new file
		char* newFileName = calloc(sizeof(fileName) + 3, sizeof(char));
    	strncpy(newFileName + 3, fileName, sizeof(fileName));
    	newFileName[0] = 'm';
    	newFileName[1] = 'd';
    	newFileName[2] = '_';
		FILE* out = fopen(newFileName, "wb"); // Open file for write in binary
		//printf("frame size: old:(%d) new(%d)\n", frameSize, frameSizeNew);
		//printf("file size: old:(%d) new(%d)\n", tagSize, tagSize - frameSize + frameSizeNew);
		printf("TAG: %s was succesfuly modifed", propName);
		fseek(file, 0, SEEK_SET);
	    fseek(out, 0, SEEK_SET);
	    
	    fwrite(fileHeader.buffer, sizeof(char), 6, out); // File header
	    
	    unsigned int value = convertEndians(toMP3Size(tagSize - frameSize + frameSizeNew)); // New tag size
	    fwrite(&value, sizeof(int), 1, out);
	    copyFileP(file, out, 10, 10, frameChangePos);
	    
	    // Change frame size
	    fseek(out, ftell(out) - 6, SEEK_SET);
	    value = convertEndians(frameSizeNew);
	    fwrite(&value, sizeof(int), 1, out);
	    fseek(out, ftell(out) + 2, SEEK_SET);
	    
	    int i = 0;
	    for (i = 0; i < frameSizeNew;i++) {
	    	fwrite(&newTag[i], sizeof(char), 1, out); // new Tag
		}
		copyFileE(file, out, 10 + frameChangePos + frameSize, 10 + frameChangePos + frameSizeNew); // all the other things

	
	    fclose(out);
	    printf("\n\nAFTER MODIFICAITON\n\n");
	    show(newFileName);

	}
    fclose(file); // Closing file

}

// Show test
// --filepath=song.mp3 --show
// Modified
// --filepath=md_song.mp3 --show

// Get test
// --filepath=song.mp3 --get=TYER
// --filepath=song.mp3 --get=COMM
// --filepath=song.mp3 --get=TPE1

// Set test (show() is called before and after changing for debug reasons)
// --filepath=song.mp3 --set=TYER value=2000
// --filepath=song.mp3 --set=COMM value=AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
// --filepath=song.mp3 --set=COMM value=A
// --filepath=song.mp3 --set=WOAF value=yandex.ru

void main(int argc, char* argv[]) {
    char* filepath = strpbrk(argv[1], "=") + 1;
    if (!strcmp(argv[2], "--show")) {
        show(filepath); // Show all tags
    } else if (argv[2][2]=='g' && argv[2][3]=='e' && argv[2][4]=='t') {
        get(filepath, strpbrk(argv[2], "=") + 1); // Get current tag
    } else if (argv[2][2]=='s' && argv[2][3]=='e' && argv[2][4]=='t') {
    	//printf("%s %s", strpbrk(argv[3], "=") + 1, strpbrk(argv[3], "=") + 1);
        set(filepath, strpbrk(argv[2], "=") + 1, strpbrk(argv[3], "=") + 1); // Set to prop_name prop_value
    } else {
    	throwError("Unkown command");
	}

}
