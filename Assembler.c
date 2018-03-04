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
int countLines(FILE*);

int main() {
    
    char s[] = "Push: lw 1   2 3 #hello man! what is up am i right alkadsj alsdfkjads fljm i right";
    int tokinstr = countTokens(s);
    char **instr = split(s);
    
    //counts
    int countLines = 0;
    
    //current line in the input
    char** line;
    
    //our entire program array
    char*** prgrm;
    
    int i = 0;
    
    //opcodes
    char **opcodes = {"lw", "sw", "add", "nand", "beq", "jalr", "halt", "noop"};
    
    FILE *file;
    if ((file = fopen("run.a", "r"))) {
        printf("True");
    }
    else {
        printf("false\n");
        printf("Couldn't find the file\n");
        return 0;
    }
    /*
     * Going through the file:
     * Take lines in as
     *
     */
    while(fgets(line, 255,  file)) {
        countLines++;
        prgrm[i] = strdup(line);
        i++;
    }
    return 0;
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
    fclose(f);
    return lines;
}
