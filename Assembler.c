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

int main() {
    
    //char s[] = "Push: lw 1   2 3 #hello man! what is up am i right alkadsj alsdfkjads fljm i right";
    //int tokinstr = countTokens(s);
    char **instr;
    
    int i=0;
    int j=0;
    int k=0;
    
    FILE *file;
    if ((file = fopen("run.a", "r"))) {
        printf("True\n");
    }
    else {
        printf("false\n");
        printf("Couldn't find the file\n");
        return 0;
    }
    
    //counts
    int lcount = 0;
    
    //current line in the input
    char line[200];
    
    //num lines in the file
    int flines = countLines(file);
    
    //our entire program array and allocate memory for the number of lines in the file
    char*** prgrm = malloc(flines * sizeof(char**));
    
    /*
     * Going through the file:
     * Take lines in and add the instructions to the program array
     */
    i=0;
    while(fgets(line, 255,  file)) {
        lcount++;
        prgrm[i] = split(line);
        
        i++;
    }
    
    /*
     * Next: FIRST PASS to replace labels with offsets
     * Second pass to actually pack the bits and check for formatting
     *
     *
     */
    i=0;
    j=0;
    k=0;
    while(i < lcount) {
        //if it's not a number or an opcode, it must be a label
        //When we find a label, loop through the program and see where the label is
        while(prgrm[i][j] != '\0') {
            
            //if the token we are on is not an opcode or a digit, it must be a label
            if((isop(prgrm[i][j]) == 0) && (isDigit(prgrm[i][j]) == 0)) {
                char* label = strdup(prgrm[i][j]); //set label equal to this token
                
                //loop through the rest of the program until label is found
                for
                    }
        }
    }
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
        if(opcodes[i] == c) {
            return 1;
        }
    }
    return 0;
}

//checks if the given string is a digit or not
int isDigit(char* c) {
    int i=0;
    for(i=0;i<strlen(c);i++) {
        if(c[i] > '9' || c[i] < '0' ) {
            return 0;
        }
    }
    return 1;
}
