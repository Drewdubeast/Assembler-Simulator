//
//  main.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/1/18.
//  Copyright © 2018 Drew Wilken. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **split(char s[]);

int main() {
    
    char s[] = "lw 1 2 0 hello there man";
    
    char **instr = split(s);
    
    int i =0;
    for(i=0;i<10;i++) {
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

char **split(char s[]) {
    char *token;
    int i = 0;
    //allocate memory for the str array so that it can be returned and not
    //just a reference to the main memory stack
    char **instr = malloc(5 * sizeof(char*));
    for (int i = 0 ; i < 5; ++i)
        instr[i] = malloc(10 * sizeof(char));
    
    //replace all tabs with spaces
    while(s[i] != '\0') {
        if(s[i]=='\t') {
            s[i] = ' ';
        }
        i++;
    }
    
    i=0;
    token = strtok (s," ");
    while (token != NULL)
    {
        instr[i++] = token;
        //printf("%s\n", instr[i-1]);
        token = strtok (NULL, " ");
    }
    for (i = 0; i < 10; i++) {
        //printf("%s\n", instr[i]);
    }
    return instr;
}

