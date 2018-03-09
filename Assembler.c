//
//  main.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/1/18.
//  Copyright © 2018 Drew Wilken and Nathan Taylor. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Function Prototypes
 */
char **split(char s[]);
int countTokens(char* s);
int countLines(FILE* f);
int isop(char* c);
int isDigit(char* c);

/*
 * MAIN FUNCTION
 */
int main() {
    
    //indices
    int i=0;
    int j=0;
    int k=0;
    int x=0;
    
    //File handling
    FILE *file;
    if ((file = fopen("run.a", "r"))) {}
    else {
        printf("Couldn't find the file\n");
        return 0;
    }
    
    //counts
    int lcount = 0;
    
    //current line in the input
    char line[200];
    
    //num lines in the file
    int flines = countLines(file);
    
    //label arrays
    char** labelArray = malloc(flines * sizeof(char*));
    
    //printf("isop of sw: %d\n", isop("sw0"));
    
    //Token count array for each instruction
    int tokensPerLine[flines];
    
    //our entire program array and allocate memory for the number of lines in the file
    char*** prgrm = malloc(flines * sizeof(char**));
    
    /*
     * Going through the file:
     * Take lines in and add the instructions to the program array
     */
    i=0;
    while(fgets(line, 255,  file)) {
        //TODO: check for white space first or label, if neither, then return
        lcount++;
        tokensPerLine[i] = countTokens(line);
        prgrm[i] = split(line);
        i++;
    }
    
    printf("tokens per line %d", tokensPerLine[0]);
    /*
     * Next: FIRST PASS to replace labels with offsets
     * Second pass to actually pack the bits and check for formatting
     */
    i=0;
    j=0;
    k=0;
    
    //First pass through, finding labels and storing indices
    while(i<lcount) {
        char* firstToken = prgrm[i][0];
        if((isop(firstToken) == 0) && (isDigit(firstToken) == 0)) { //if it's a label
            labelArray[i] = strdup(firstToken); //add label to label array
        }
        putchar('\n');
        i++;
    }
    
    
    /*
     * SECOND PASS:
     *
     
     
     
     *
     *
     */
}



/*
 * Splits a string into tokens and returns a pointer to a str array
 */
char **split(char s[]) {
    char *token;
    int i = 0;
    int count = countTokens(s);
    
    char **instr = malloc((count+1) * sizeof(char*));
    
    //get a count of tokens so we can allocate memory for them
    token = strtok (s," ");
    while (token != NULL)
    {
        //since token reference keeps changing and is static, use strdup instead.
        instr[i] = strdup(token);
        token = strtok (NULL, " ");
        
        i++;
    }
    return instr;
}

/*
 * Returns the amount of tokens in the str
 */
int countTokens(char* s) {
    int i = 0;
    int count = 0;
    char lastChar = ' ';
    
    while(s[i] != '\0'){
        if(lastChar == ' ' && s[i] != ' ') {
            count++;
        }
        lastChar = s[i];
        i++;
    }
    return count;
}

//counts the lines in a given file pointer
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

//checks if the given string is an opcode or not
int isop(char* c) {
    //opcodes
    char *opcodes[] = {"lw", "sw", "add", "nand", "beq", "jalr", "halt", "noop"};
    int i=0;
    for(i=0;i<7;i++) {
        if(strcmp(opcodes[i],c) == 0) {
            return 1;
        }
    }
    return 0;
}

//checks if the given string is a digit or not
int isDigit(char* s) {
    int i=0;
    for(i=0;i<strlen(s);i++) {
        if(s[i] > '9' || s[i] < '0' ) {
            return 0;
        }
    }
    return 1;
}
