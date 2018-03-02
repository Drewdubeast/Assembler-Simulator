//
//  main.c
//  Assembler-Simulator-XP
//
//  Created by Drew Wilken on 3/1/18.
//  Copyright © 2018 Drew Wilken. All rights reserved.
//

/*
 * TODO: since strtok already breaks up the string and puts null characters in between each one, then we can just use that
 * knowledge to take the addresses of each piece and then just add it to the main array.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char **split(char s[]);

char ***instructions;

int main() {
    
    char s[] = "lw 1 2 0 yo hello ";
    char **instr = split(s);
    
    int i = 0;
    int j = 0;
    /*
     char *test;
     char str[] = "hellooooooooo";
     printf("size of str: %i\n", sizeof(str));
     test = malloc(strlen(str) * sizeof(char));
     printf("size of test: %i\n", sizeof(test));
     test = "hello";
     
     printf("%s\n", test);
     
     printf("%s\n", instr[2]);
     */
    for(i = 0; i<10; i++) {
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



//
// Function to split the string into tokens
// in a string array
//
// uses strtok() to break it into the tokens
//
char **split(char s[]) {
    char *token;
    int i = 0;
    int count = 0;
    
    //get a count of tokens so we can allocate memory for them
    token = strtok (s," ");
    while (token != NULL)
    {
        count++;
        token = strtok (NULL, " ");
    }
    printf("Count: %i\n", count);
    //allocate memory for the str array so that it can be returned and not
    //just a reference to the main memory stack
    //char **instr = (char**)malloc(count * sizeof(char*));
    char **instr = malloc((count+1) * sizeof(char*));
    //char **instr = malloc(200);
    
    printf("instr size: %lu\n", sizeof(instr));
    
    
    //replace all tabs with spaces
    while(s[i] != '\0') {
        if(s[i]=='\t') {
            s[i] = ' ';
        }
        i++;
    }
    i=0;
    token = strtok (s," ");
    
    //go through the string and while it isn't null, add the token to the array
    while (token != NULL)
    {
        //allocate the memory needed for the size of the str
        instr[i] = malloc((strlen(token)+1) * sizeof(char));
        strcpy(instr[i++],token);
        printf("%s\n", instr[i-1]);
        token = strtok (NULL, " ");
    }
    return instr;
}
