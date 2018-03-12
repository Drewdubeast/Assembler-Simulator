//
//  Simulator.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/11/18.
//  Copyright © 2018 Drew Wilken. All rights reserved.
//

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int countLines(FILE *f);

int main(int argc, char **argv) {
    //variables for each slot in the register...
    int reg[8];
    int mem[65535];
    
    //indices
    int i=0;
    int j=0;
    int k=0;
    
    //allocate a line as 10 characters at max
    //char* line = malloc(sizeof(char) * 10);
    char line[15];
    
    //run-mode
    bool fileout = false;
    
    //File handling
    FILE *file = NULL;
    FILE *out = NULL;
    
    argc = 2;
    argv[1] = "run.mc";
    
    //file handling
    if (argc == 1) {
        printf("No file input!\n");
        return 0;
    }
    else if (argc == 2) {
        fileout = false;
        if (!(file = fopen(argv[1], "r"))) {
            printf("Couldn't find the file: %s\n", argv[1]);
            return 0;
        }
    }
    else if (argc == 3) {
        fileout = true;
        if (!(file = fopen(argv[1], "r"))) {
            printf("Couldn't find the file: %s\n", argv[1]);
            return 0;
        }
        if (!(out = fopen(argv[2], "w+"))) {
            printf("Couldn't find the file: %s\n", argv[2]);
            return 0;
        }
    }
    else {
        printf("Too many input arguments! Must have a maximum of two.\nEnter one file name argument to print output to stdout\nEnter two file names to output to second file.\n");
        return 0;
    }
    
    //file lines
    int lines = countLines(file);
    
    //program array
    uint32_t *prgrm = malloc(sizeof(uint32_t)*lines);
    
    for(i=0;fgets(line, sizeof(line), file) != NULL;i++) {
        prgrm[i] = atoi(line);
        printf("%" PRIu32 "\n", prgrm[i]);
    }
    fclose(file);
    return 0;
}

int countLines(FILE* f) {
    int lines = 0;
    int c;
    
    if (f == NULL) {
        return 0;
    }
    while((c = fgetc(f)) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    fseek(f, 0, SEEK_SET); //seek back to the beginning of the file
    //fclose(f);
    return lines;
}

