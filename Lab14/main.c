/* Kornilov Nikita, M3102, 4.12.2020, lab 14 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

void throwError(char errorText[]) {
	printf("Error: %s\n", errorText);
	exit(1);
}

unsigned int convertEndians(unsigned int val) {
    return ((val >> 24) 	& 	0x000000FF) 	| 	// byte 1
           ((val >> 8)		&	0x0000FF00) 	|	// byte 2
           ((val << 8) 		& 	0x00FF0000) 	|	// byte 3
           ((val << 24)		&	0xFF000000)		;	// byte 4
}

int getDigits(int val) {
	int digits = 0;
	if (val == 0) {
		return 1;
	}
	while (val > 0) {
		digits++;
		val /= 10;
	}
	return digits;
}

void parseArgs(int argc, char *argv[], char** inputFileName, char** outputDirName, int* maxIter, int* dumpFreq) {
	int i;
	
	for (i = 1; i < argc; i++) {
			if (!strcmp(argv[i], "--input")) {
				*inputFileName = argv[++i];
			}
			else if (!strcmp(argv[i], "--output")) {
				*outputDirName = argv[++i];
			}
			else if (!strcmp(argv[i], "--max_iter")) {
				i++;
				if (argv[i][0] == '-') {
					throwError("max_iter can't be negative");
				}
            	*maxIter = atol(argv[i]);
			}
			else if (!strcmp(argv[i], "--dump_freq")) {
				i++;
            	*dumpFreq = atol(argv[i]);
				if (argv[i][0] == '-' || *dumpFreq > *maxIter) {
					throwError("dump_freq can't be negative or more than max_iter");
				}
			} else {
				throwError("Unkown parameter");
			}
		}
	
	if (*inputFileName == NULL || *outputDirName == NULL) {
		throwError("missing necessary arguments input_file or output_dir");
	}
	    
    printf("Game parameters:\n input_file: %s \n output_dir: %s \n max_iter: %i \n dump_freq: %i \n", *inputFileName, *outputDirName, *maxIter, *dumpFreq);
}

union bmpHeader {
    unsigned char buffer[62];

    struct {
    	// BITMAPFILEHEADER
        unsigned short id;			// 2 bytes
        unsigned int size;			// 4 bytes
        unsigned int reserved;		// 4 bytes
        unsigned int imageOffset;	// 4 bytes
        							// 14 total
        // BITMAPINFO ver 3
        int biSize;					// 4 bytes
        int biWidth;				// 4 bytes
        int biHeight;				// 4 bytes
        short biPlanes;				// 2 bytes
        short biBitCount;			// 2 bytes	
        int biCompression;			// 4 bytes
        int biSizeImage;			// 4 bytes
        int biXPelsPerMeter;		// 4 bytes
        int biYPelsPerMeter;		// 4 bytes
        int biClrUsed;				// 4 bytes
        int biClrImportant;			// 4 bytes
        							// 40 total
		unsigned char cl1_rgbBlue;		// 1 byte
        unsigned char cl1_rgbGreen;		// 1 byte
        unsigned char cl1_rgbRed;		// 1 byte
        unsigned char cl1_rgbReserved;	// 1 byte
		unsigned char cl2_rgbBlue;		// 1 byte
        unsigned char cl2_rgbGreen; 	// 1 byte
        unsigned char cl2_rgbRed;		// 1 byte
        unsigned char cl2_rgbReserved;	// 1 byte
										// 8 total							
    } data;							// 62 all 
};

int** mapAlloc(int width, int height) {
	int i;
    int **map = (int **)malloc(height * sizeof(int *));
    if (map == NULL) {
		perror("Error while allocating map");
	}
    
    for (i = 0; i < height; i++) {
        map[i] = (int *)malloc(width * sizeof(int));
        if (map[i] == NULL) {
			perror("Error while allocating map");
		}
    }
    
    return map;
}

void mapFree(int** map, int height) {
	int y;
    for (y = 0; y < height; y++) {
        free(map[y]);
    }
    free(map);
}

void mapCopy(int** from, int** to, int width, int height) {
	int x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            to[y][x] = from[y][x];
        }
    }
    mapFree(from, height);
}

void dumpMap(int** map, union bmpHeader bmpHeader, char* outputDirName, int iteration) {
	FILE *outputFile;
	mkdir(outputDirName);
	
	int outputFileNameLength = strlen(outputDirName) + 4 + getDigits(iteration);
	char* outputFileName = calloc(outputFileNameLength + 1, sizeof(char));
	
	strncpy(outputFileName, outputDirName, strlen(outputDirName));
	itoa(iteration, outputFileName + strlen(outputDirName), 10);
	strcat(outputFileName, ".bmp");

	outputFile = fopen(outputFileName, "wb");
	
	if (outputFile == NULL) {
		perror("Error while opening outputFile");
	}
	
	fwrite(&bmpHeader, sizeof(char), 62, outputFile);
	
	// DUMP MAP CODE
	
	int width = bmpHeader.data.biWidth;
    int height = bmpHeader.data.biHeight;
    
    int x, y, i = width % 32, j, buffer;
    
    for (y = 0; y < height; y++) {
    	buffer = 0;
    	for (x = 0; x < width / 32; x++) {
    		buffer = 0;
    		for (j = 0; j < 32; j++) {
    			buffer |= !map[y][x * 32 + j] << (32 - j - 1);
			}
			buffer = convertEndians(buffer);
			fwrite(&buffer, sizeof(int), 1, outputFile);
		}
		buffer = 0;
		for (x = width / 32 * 32; x < width; x++) {
    		buffer |= !map[y][x] << (32 - x % 32 - 1);
		}
		buffer = convertEndians(buffer);
		fwrite(&buffer, sizeof(int), 1, outputFile);
	}
		
	fclose(outputFile);
}

int** getMap(char* inputFileName, char* outputDirName, int* width, int* height, union bmpHeader *bmpHeader) {
	FILE *inputFile;
	inputFile = fopen(inputFileName, "rb");

	if (inputFile == NULL) {
		perror("Error while opening inputFile");
	}
	
    if (!fread((*bmpHeader).buffer, sizeof(char), 62, inputFile)) {
		perror("Error reading file:");
	}
	
	if ((*bmpHeader).data.biSize != 40 || (*bmpHeader).data.biClrUsed != 0 || (*bmpHeader).data.biBitCount != 1) {
    	throwError("Trying to open not a monocrome .bmp file!");
    }

    *width = (*bmpHeader).data.biWidth;
    *height = (*bmpHeader).data.biHeight;

    int** map = mapAlloc(*width, *height);

	int i, x, y, buffer;
	
	
	for (y = 0; y < *height; y++) {
		i = 0;
		for (x = 0; x < *width; x++) {
			if (i % 32 == 0) {
				i = 0;
				fread(&buffer, sizeof(int), 1, inputFile);
				buffer = convertEndians(buffer);
			}
			map[y][x] = !((buffer >> (32 - i - 1)) & 1);
			i++;
		}
	}

    fclose(inputFile);
	return map;
}

int** getNextMapGeneration(int** map, int** mapAlt, int width, int height) {
	int neighbours, x, y;
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			neighbours = 0;
			if (map[(y + height + 1) % height][(x + width - 1) % width]) neighbours++;
			if (map[(y + height + 1) % height][(x + width) % width]) neighbours++;
			if (map[(y + height + 1) % height][(x + width + 1) % width]) neighbours++;
			
			if (map[(y + height) % height][(x + width - 1) % width]) neighbours++;
			if (map[(y + height) % height][(x + width + 1) % width]) neighbours++;
			
			if (map[(y + height - 1) % height][(x + width - 1) % width]) neighbours++;
			if (map[(y + height - 1) % height][(x + width) % width]) neighbours++;
			if (map[(y + height - 1) % height][(x + width + 1) % width]) neighbours++;
			
			mapAlt[y][x] = (neighbours == 2 && map[y][x]) || (neighbours == 3);
		}
	}
	return mapAlt;
}

void StartGame(int argc, char *argv[]) {
	char* inputFileName = NULL;
    char* outputDirName = NULL;
    int maxIter = -1;
    int dumpFreq = 1;
    int width, height;
    
    parseArgs(argc, argv, &inputFileName, &outputDirName, &maxIter, &dumpFreq);
    
    union bmpHeader bmpHeader;
    
    int** map = getMap(inputFileName, outputDirName, &width, &height, &bmpHeader);
    
    dumpMap(map, bmpHeader, outputDirName, 0);
    
    int i, dumps = 1;
    dumpMap(map, bmpHeader, outputDirName, 0);
    
    for (i = 0; i < maxIter; i++) {
    	int** mapAlt = mapAlloc(width, height);
		mapAlt = getNextMapGeneration(map, mapAlt, 
		width, height);
		
		mapCopy(mapAlt, map, width, height);
		
		if (i % dumpFreq == 0) {
    		dumpMap(map, bmpHeader, outputDirName, dumps++);
		}
	}
	mapFree(map, height);
	printf("Game finished\nTotal dumps: %d", dumps);	
}

void main(int argc, char *argv[]) {
	// --input input_file.bmp --output dir_name --max_iter 1 --dump_freq 1
	StartGame(argc, argv);
}
