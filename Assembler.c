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

char **split(char s[]);
int countTokens(char* s);

char ***instructions;

int main() {
    
    char s[] = "Push: lw 1  2 3 #hello man!";
    int tokinstr = countTokens(s);
    char **instr = split(s);
    
    int i = 0;
    
    for(i = 0; i<tokinstr; i++) {
        printf("%s\n", instr[i]);
    }
    
    
    /*
     char lines[100][100];
     int i = 0;
     
     FILE *file;
     file = fopen("run.a", "r");
     //char *s = "a b c";
     //takes the lines from the file and puts it into 'lines' array
     while(fgets(lines[i], 100,  file)) {
     split(lines[i]);
     i++;
     }*/
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
    
    //replace all tabs with spaces
    while(s[i] != '\0') {
        if(s[i]=='\t') {
            s[i] = ' ';
        }
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

